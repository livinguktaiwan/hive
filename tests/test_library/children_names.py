class NameAlreadyInUse(Exception):
    pass


class ChildrenNames:
    def __init__(self, name_base):
        self.names = []
        self.name_base = name_base
        self.next_name_number = 0

    def create_name(self):
        name = f'{self.name_base}{self.next_name_number}'
        self.names.append(name)
        self.next_name_number += 1
        return name

    def register_name(self, name):
        if name in self.names:
            raise NameAlreadyInUse()
