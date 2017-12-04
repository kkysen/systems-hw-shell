import os


def document_headers(fn: str='headers.md') -> None:
    with open(fn, 'w') as out:
        for path in get_source_files():
            for header in get_headers(path):
                out.write('```doxygen\n{}\n```\n\n'.format(header))
        

def get_source_files(dir: str='.'):
    for dirpath, dirnames, filenames in os.walk(dir):
        for fn in filenames:
            path = os.path.join(dirpath, fn)
            if '.c' in path or '.h' in path:
                yield path


def get_headers(fn: str):
    end_char = ';' if '.h' in fn else '{'
    with open(fn) as f:
        text = f.read()
        i = 0
        while True:
            i = text.find('/**', i)
            if i == -1:
                break
            j = text.find(end_char, i)
            yield text[i:j].strip() + ';'
            i += 1


if __name__ == '__main__':
    document_headers()