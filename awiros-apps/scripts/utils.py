class ERROR:
    pass


class ENGYN_CONFIG:
    SRC_DIR = 'apps'
    HEADER_EXT = 'h'
    CPP_EXT = 'cpp'
    DEFAULT_HEADER = "scripts/default_app.h"
    DEFAULT_CPP = "scripts/default_app.cpp"
    DEFAULT_APP_STEM = "default_app"
    CONSTS_FILE = "consts.h"
    HANDLEACS_FILE = "handle_acs.cpp"
    CONVERSION_FILE = "conversion_layer.h"
    CONSTS_APP_PREFIX = "FUNC_"

def touch(filepath):
    """
    TODO: Check if file already exists, if yes ask before overwriting
    """
    return 0


def parse(filepath):
    with open(filepath, 'r') as f:
        contents = f.read()
        return contents


def write_content(content, filepath):
    with open(filepath, 'w') as f:
        f.write(content)

    return 0

def locate_element_with_substr(arr, substr, last_occ = False):
    idx = None
    for i in range(len(arr)):
        if substr in arr[i]:
            idx = i
            if(last_occ == False):
                return idx

    return idx

def parse_content_to_list(filepath):
    content = parse(filepath)
    content = content.strip().split('\n')
    return content

