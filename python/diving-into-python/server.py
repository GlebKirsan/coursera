import asyncio


metrics = {}
error_message = b"error\nwrong data\n\n"
class ClientServerProtocol(asyncio.Protocol):
    def connection_made(self, transport):
        self.transport = transport
        print(transport.get_extra_info())
    
    def put(self, data):
        self.transport.write(b"ok\n\n")
        key, value, timestamp = data.split()
        if key not in metrics:
            metrics[key] = []
        if (value, timestamp) not in metrics[key]:
            metrics[key].append((value, timestamp))

    def get(self, key):
        message = b"ok\n"
        if key == '*':
            for key, values in metrics.items():
                for value in values:
                    message += f"{key} {value[0]} {value[1]}\n".encode()
        elif key not in metrics:
            pass
        else:
            for value in metrics[key]:
                message += f"{key} {value[0]} {value[1]}\n".encode()
        message += b'\n'
        self.transport.write(message)

    def data_received(self, data):
        decoded_data = data.decode()
        key, payload = decoded_data.split(' ', 1)
        payload = payload.strip()
        if key == "put":
            self.put(payload)
        elif key == "get":
            self.get(payload)
        else:
            self.transport.write(error_message)


def run_server(host, port):
    loop = asyncio.get_event_loop()
    cor = loop.create_server(
        ClientServerProtocol,
        host, port
    )

    server = loop.run_until_complete(cor)
    try:
        loop.run_forever()
    except KeyboardInterrupt:
        pass

    server.close()
    loop.run_until_complete(server.wait_closed())
    loop.close()