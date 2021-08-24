from json import load
from utils import *


@ensure_storage_exist
def load_storage() -> Dict[str, List[str]]:
    with open(get_storage_path(), 'r') as f:
        storage = load(f)
    return storage


def main():
    argument_parser = init_argument_parser()
    arguments = argument_parser.parse_args()
    storage = load_storage()
    if arguments.val:
        storage[arguments.key] = storage.get(arguments.key, [])
        storage[arguments.key].append(arguments.val)
        write_to_storage(storage)
    else:
        print(', '.join(storage.get(arguments.key, [])))


if __name__ == '__main__':
    main()
