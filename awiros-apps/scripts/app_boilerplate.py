# System imports
import os
import sys
import time
import utils
import argparse

# Local imports
from utils import ENGYN_CONFIG


parser = argparse.ArgumentParser(description = "This is a utility to create boilerplate apps")

create_args = parser.add_argument_group("create options")

create_args.add_argument('-c', '--create', action = 'store_true')

delete_args = parser.add_argument_group("delete options")


class app_generator:

    def __init__(self, app_name, app_id, app_type):
        self.app_name = app_name
        self.app_id   = app_id
        self.app_type = app_type
        self.err      = 0

        # Get filestem, remove awiros from front
        if(self.app_name.startswith('awiros_')):
            self.app_stem = app_name[7:]


    def init(self):

        print("Generating boilerplate ..")
        self.gen_app_files()
        self.entry_in_app_registry()
        #self.integrate_app_with_engyn()
        self.build()
        self.run()
        self.cleanup()
        print("Done!")


    def gen_app_files(self):

        status = self.create_header_file()
        if (status != 0):
            self.err_exit("create_header_file")

        status = self.create_cpp_file()
        if (status != 0):
            self.err_exit("create_cpp_file")

    def entry_in_app_registry(self):

    	with open(ENGYN_CONFIG.SRC_DIR+'/app-registry.txt', 'a') as file:
    	    file.write(str(self.app_id)+' '+self.app_stem)


    def create_header_file(self):

        os.mkdir(ENGYN_CONFIG.SRC_DIR+'/'+self.app_stem)

        filepath = os.path.join(ENGYN_CONFIG.SRC_DIR, self.app_stem ,"{}.{}".format(self.app_stem, ENGYN_CONFIG.HEADER_EXT))

        # create empty
        status = utils.touch(filepath)
        if(status != 0):
             self.err = status
             return status

        # parse content
        content = utils.parse(ENGYN_CONFIG.DEFAULT_HEADER)

        # inject class name
        content = content.replace(ENGYN_CONFIG.DEFAULT_APP_STEM, self.app_stem)\
                         .replace(ENGYN_CONFIG.DEFAULT_APP_STEM.upper(), self.app_stem.upper())

        # parse content
        status = utils.write_content(content, filepath)
        if(status != 0):
            self.err = status

        return status


    def create_cpp_file(self):

        # create empty
        filepath = os.path.join(ENGYN_CONFIG.SRC_DIR, self.app_stem,"{}.{}".format(self.app_stem, ENGYN_CONFIG.CPP_EXT))
        status = utils.touch(filepath)

        if(status != 0):
             self.err = status
             return status

        # parse content
        content = utils.parse(ENGYN_CONFIG.DEFAULT_CPP)

        # inject class name
        content = content.replace(ENGYN_CONFIG.DEFAULT_APP_STEM, self.app_stem)

        # parse content
        status = utils.write_content(content, filepath)

        if(status != 0):
            self.err = status

        return status


    def integrate_app_with_engyn(self):

        status = self.parse_consts()
        if (status != 0):
            self.err_exit("parse_consts")

        status = self.parse_handleacs()
        if (status != 0):
            self.err_exit("parse_handleacs")

        status = self.parse_conversion_layer()
        if (status != 0):
            self.err_exit("parse_conversion_layer")


    def parse_consts(self):
        filepath = os.path.join(ENGYN_CONFIG.SRC_DIR, ENGYN_CONFIG.CONSTS_FILE)
        # parse
        content = utils.parse_content_to_list(filepath)

        # locate closing line of struct
        idx = utils.locate_element_with_substr(content, '};')

        func_name = ENGYN_CONFIG.CONSTS_APP_PREFIX + self.app_stem.upper()

        # add app number
        content.insert(idx - 1, "  {} = {},".format(func_name, self.app_id))

        # consolidate
        content = '\n'.join(map(str, content))

        # write back
        status = utils.write_content(content, filepath)
        if (status != 0):
            self.err = status

        return status


    def parse_handleacs(self):
        filepath = os.path.join(ENGYN_CONFIG.SRC_DIR, ENGYN_CONFIG.HANDLEACS_FILE)

        # parse
        content = utils.parse_content_to_list(filepath)

        # locate closing of swith case
        idx = utils.locate_element_with_substr(content, 'default:')

        func_name = ENGYN_CONFIG.CONSTS_APP_PREFIX + self.app_stem.upper()

        # add switch case
        content.insert(idx - 1, "\tbreak;".expandtabs(8))
        content.insert(idx - 1,
                "\tconv_layer_obj.start_app<{}>();".format(self.app_name).expandtabs(8))
        content.insert(idx - 1, "\tcase {}:".format(func_name).expandtabs(4))
        content.insert(idx - 1, "")

        # consolidate
        content = '\n'.join(map(str, content))

        # write back
        status = utils.write_content(content, filepath)
        if (status != 0):
            self.err = status

        return status


    def parse_conversion_layer(self):
        filepath = os.path.join(ENGYN_CONFIG.SRC_DIR, ENGYN_CONFIG.CONVERSION_FILE)

        # parse
        content = utils.parse_content_to_list(filepath)

        # locate
        idx = utils.locate_element_with_substr(content, '#include', last_occ = True)

        # add header include
        content.insert(idx + 1, '#include "{}/{}.{}"'.format(ENGYN_CONFIG.SRC_DIR, self.app_stem, ENGYN_CONFIG.HEADER_EXT))

        # consolidate
        content = '\n'.join(map(str, content))

        # write back
        status = utils.write_content(content, filepath)
        if (status != 0):
            self.err = status

        return status


    def build(self):
        return 1


    def run(self):
        return 1


    def cleanup(self):
        return 1


    def perror(self, msg):
        print("error: {}".format(msg))


    def err_exit(self, msg):
        self.perror(msg)
        self.cleanup()
        quit()


if __name__ == "__main__":

    """
    Get necessary variables from user
    """

    app_name = "awiros_" + raw_input("Enter app name: awiros_")
    app_id = input("Enter app ID: ")
    app_type = raw_input("Enter app type (Defualt: basic): ")

    if(len(app_type) == 0):
        app_type = "basic"

    app = app_generator(app_name, app_id, app_type)
    #app = app_generator("awiros_test", 1000, "basic")
    app.init()
