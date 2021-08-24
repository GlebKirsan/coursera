import os
import csv
from typing import Dict, Union, Type


class CarBase:
    required = []

    def __init__(self, brand, photo_file_name, carrying):
        self.brand = brand
        self.photo_file_name = photo_file_name
        self.carrying = carrying

    @classmethod
    def get_from_dict(cls, data: Dict[str, str]):
        parameters = [data[parameter] for parameter in cls.required]
        return cls(*parameters)

    def get_photo_file_ext(self) -> str:
        return os.path.splitext(self.photo_file_name)[1]


class Car(CarBase):
    required = ['photo_file_name', 'brand', 'carrying',
                'passenger_seats_count']

    def __init__(self, brand, photo_file_name, carrying,
                 passenger_seats_count):
        super().__init__(brand, photo_file_name, carrying)
        self.passenger_seats_count = passenger_seats_count

    def __repr__(self):
        return f'Car(brand={self.brand}, ' \
               f'photo_file_name={self.photo_file_name}, ' \
               f'carrying={self.carrying}, ' \
               f'passenger_seats_count={self.passenger_seats_count})'


class Truck(CarBase):
    required = ['brand', 'photo_file_name', 'carrying', 'body_whl']

    def __init__(self, brand, photo_file_name, carrying, body_whl):
        super().__init__(brand, photo_file_name, carrying)
        self.body_length, self.body_width, self.body_height = \
            self.parse_whl(body_whl)

    def get_body_volume(self) -> float:
        return self.body_width * self.body_height * self.body_length

    @staticmethod
    def parse_whl(body_whl):
        length, width, height = (float(c) if c.isdigit() else 0.0
                                 for c in body_whl.split("x", 2))
        return length, width, height

    def __repr__(self):
        dimensions = (self.body_length, self.body_width, self.body_height)
        return f'Truck(brand={self.brand}, ' \
               f'photo_file_name={self.photo_file_name}, ' \
               f'carrying={self.carrying}, ' \
               f'(length, width, height)={dimensions})'


class SpecMachine(CarBase):
    required = ['brand', 'photo_file_name', 'carrying', 'extra']

    def __init__(self, brand, photo_file_name, carrying, extra):
        super().__init__(brand, photo_file_name, carrying)
        self.extra = extra

    def __repr__(self):
        return f'SpecMachine(brand={self.brand}, ' \
               f'photo_file_name={self.photo_file_name}, ' \
               f'carrying={self.carrying}, ' \
               f'extra={self.extra})'


def get_car_list(csv_filename):
    car_list = []
    string_to_car: Dict[str, Type[Union[Truck, Car, SpecMachine]]] = \
        {'truck': Truck,
         'car': Car,
         'spec_machine': SpecMachine}
    with open(csv_filename, 'r') as csv_file:
        csv_reader = csv.DictReader(csv_file, delimiter=';')
        for row in csv_reader:
            try:
                car_class = string_to_car[row['car_type']]
                car_list.append(car_class.get_from_dict(row))
            except Exception:
                pass
    return car_list


if __name__ == '__main__':
    print(get_car_list('cars.csv'))
