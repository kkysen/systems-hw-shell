import os


def generate_makefile(fn: str = 'Makefile') -> None:
    with open(fn, 'r') as f:
        lines = f.readlines()
    start_line_num = -1
    end_line_num = -1
    for i, line in enumerate(lines):
        if '# start generate_objects()' in line:
            start_line_num = i
        if '# end generate_objects()' in line:
            end_line_num = i

    if start_line_num != -1 and end_line_num != -1:
        lines[start_line_num + 1:end_line_num] = generate_objects()

    with open(fn, 'w') as f:
        f.writelines(lines)


def generate_objects():
    yield '\n'
    objects = []
    for object_name in get_object_names():
        object = '{}.o'.format(object_name)
        if 'test' not in object:
            objects.append(object)
        yield '{0}: \n\t$(CC) -c {1}.c -o {0}\n\n'.format(object, object_name)
    yield 'all: clean {0}\n'.format(' '.join(objects))
    yield '\t$(CC) -o $(OUT) {0}\n\n'.format(' '.join(o for o in objects if o != 'chars.o'))


def get_object_names(dir: str = '.'):
    for dirpath, dirnames, filenames in os.walk(dir):
        dirpath = dirpath[2:]
        if (len(dirpath) > 0 and dirpath[0] == '.') or 'cmake' in dirpath:
            continue
        for fn in filenames:
            path = os.path.join(dirpath, fn)
            if path[-2:] == '.c':
                yield path[:-2]


if __name__ == '__main__':
    generate_makefile()
