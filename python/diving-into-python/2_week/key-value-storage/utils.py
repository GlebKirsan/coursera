import os
import tempfile
from argparse import ArgumentParser
from typing import Dict, List
from json import dump


def init_argument_parser():
    argument_parser = ArgumentParser()
    argument_parser.add_argument('--key', required=True, type=str)
    argument_parser.add_argument('--val', required=False, type=str)
    return argument_parser


def get_storage_path():
    return os.path.join(tempfile.gettempdir(), 'storage.data')


def ensure_storage_exist(func):
    def wrapper() -> Dict[str, List[str]]:
        storage_path = get_storage_path()
        if not os.path.exists(storage_path):
            write_to_storage({})
        return func()

    return wrapper


def write_to_storage(data: Dict[str, List[str]]):
    with open(get_storage_path(), 'w') as f:
        dump(data, f)
