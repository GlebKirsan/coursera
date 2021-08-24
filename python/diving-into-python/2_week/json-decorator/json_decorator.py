from json import dumps
from functools import wraps


def to_json(func):
    @wraps(get_data)
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        return dumps(result)

    return wrapper


@to_json
def get_data():
    return {
        'data': 42
    }


print(get_data())
