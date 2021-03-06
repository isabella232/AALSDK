#!/usr/bin/env python

#######################################################################
# generate_ase_environment.py : Generate AFU paths, include directories as a
# Makefile snippet for ASE builds
#
# Author: Rahul R Sharma <rahul.r.sharma@intel.com>
#
# Run: python generate_ase_environment.py <AFU sources Parent Directory>
# 
# After running script, you will see: 
# * A list of VHDL files
# * A list of {System}Verilog files
# * afu_common.mk: Prepares variables pointing to the file lists and
#   gueses include directories. The ASE files' absolute paths are also
#   calculated
# * A synopsys_sim.setup file, applicable to VCS builds, QUESTA will
#   ignore this
# 
# Mar 2014                RRS                 Original version
# Oct 2014                RRS                 Python 3.3 compatibility
#                                             Version check added
#######################################################################

# Future-proofing against Python 3 syntax changes in 'print'
from __future__ import print_function
import ase_functions
import os, re, sys
reload(sys)
sys.setdefaultencoding('utf8')

if sys.version_info < (2, 7):
    import commands
else:
    import subprocess 

### Supported file extensions
# USERs may modify this if needed
VLOG_EXTENSIONS = [ ".svh", ".sv", ".vs", ".v" ] 
VHD_EXTENSIONS = [ ".vhd", ".vhdl" ]

VHDL_FILE_LIST = os.environ['PWD'] + "/vhdl_files.list"
VLOG_FILE_LIST = os.environ['PWD'] + "/vlog_files.list"
TOOL_BRAND     = "VCS"

# Forbidden characters
SPECIAL_CHARS='\[]~!@#$%^&*(){}:;+$\''

##########################################################
###                                                    ###
###        DO NOT MODIFY BELOW THIS COMMENT BLOCK      ###
###                                                    ###
##########################################################
# Global variables
arg_list = []
tolowarg_list = []
valid_dirlist = []

special_chars_in_path = 0

#################### Run command and get string output #######################
def commands_getoutput(cmd):
    if sys.version_info < (2,7): 
        return commands.getoutput(cmd)
    else:
        byte_out = subprocess.check_output(cmd.split())
        str_out = byte_out.decode("utf-8")
        return str_out

############################# Print Help #####################################
def show_help():
    ase_functions.begin_red_fontcolor()
    print("INCORRECT command, CORRECT (required) usage is:")
    print("python generate_ase_environment.py <dir 1> [dir 2] ... [dir n] [-t <VCS|QUESTA>]")
    print("")
    print("Required switches => ")
    print("     <dir1>           => Atleast one sources directory is required")
    print("     [dir2]...[dir n] => Other optional directories with sources")
    print("")
    print("Optional switches => ")
    print("     -h,--help        => Show this help message") 
    print("     -t,--tool        => Enter tool type as 'VCS' or 'QUESTA'")
    print("")
    ase_functions.end_red_fontcolor()

##################### Check if file is package ###############################
# def file_is_package(filepath):
#     vlog_syntax = open(filepath.replace("\n","").readlines()
#     for vline in vlog_syntax:
        

########################### Has duplicates ###################################
def has_duplicates(word_list):
    dup_flag = False
    unique_words = set(word_list)
    for each_word in unique_words:
        count = word_list.count(each_word)
        if count > 1:
            print("Duplicate found - ", each_word)
            dup_flag = True
    return dup_flag

######################## Close file and exit #################################
def print_instructions():
    print("")
    ase_functions.begin_green_fontcolor()
    print("NOTES TO USER => ")
    print("* This script assumes File Extensions: ")
    print("  * VHDL : .vhd")
    print("  * V/SV : .sv .vs .v")
    print("  * If you use arbitrary extensions, please edit this script to reflect them, and re-run the script")
    print("* See ase_sources.mk and check for correctness")
    print("* See if DUT_INCDIR has all the locations mentioned")
    print("  * If a directory is missing, append it separated by '+' symbol")
    ase_functions.end_green_fontcolor()
    print("")


######################## Script begins here #################################
### Argument checking ###
#############################################################################
print("####################################################################")
print("#                                                                  #")
print("#           Xeon(R) + FPGA Accelerator Abstraction Layer           #")
print("#                 AFU Simulation Environment (ASE)                 #")
print("#                                                                  #")
print("####################################################################")
if len(sys.argv) == 1:
    show_help()
    sys.exit(0)
else:
    # Take in argument list, cleanse it to lower case for sanity
    arg_list = list(sys.argv)
    tolowarg_list = []
    for arg in arg_list:
        tolowarg_list.append( str(arg).lower() )
    # print ("ARGUMENTS : ", tolowarg_list )
    # If help was requested
    if "-h" in tolowarg_list:
        show_help()
        sys.exit(0)
    elif "--help" in tolowarg_list:
        show_help()
        sys.exit(0)
    #######################################################################
    # Select Tool brand
    if "--tool" in arg_list: 
        tool_opt_position = tolowarg_list.index("--tool")
        tool_type = arg_list[tool_opt_position + 1]
    elif "-t" in arg_list: 
        tool_opt_position = tolowarg_list.index("-t")
        tool_type = arg_list[tool_opt_position + 1]
    else:
        tool_type = "VCS"
    if (tool_type not in ["VCS", "QUESTA"]):
        print ("ILLEGAL : Tool Name ", tool_type, " was supplied, DEFAULT to 'VCS'") 
        TOOL_BRAND = "VCS"
    else:
        TOOL_BRAND = tool_type
    print("\nTool Brand : ", TOOL_BRAND)
    #######################################################################
    # Prepare list of candidate directories
    print ("Valid directories supplied => "); 
    str_dirlist = ""
    special_chars_in_path = 0
    for loc in arg_list:
        loc = os.path.abspath(loc)        
        if set(SPECIAL_CHARS).intersection(loc):
            special_chars_in_path = 1
        if (os.path.isdir(str(loc))):            
            valid_dirlist.append(loc)
            str_dirlist = str_dirlist + loc + "/ "
            print ("\t",loc)
    # If no legal directories, error out
    if len(valid_dirlist) == 0:
        ase_functions.begin_red_fontcolor()
        print("No Valid source directories were specified ... please re-run script with legal directory name")
        show_help()
        ase_functions.end_red_fontcolor()
        sys.exit(0)


########################################################
### Write Makefile snippet ###
########################################################
fd = open("ase_sources.mk", "w")
# Print Information in ase_sources.mk 
fd.write("####################################################################\n")
fd.write("#                                                                  #\n")
fd.write("# Xeon(R) + FPGA AFU Simulation Environment                        #\n")
fd.write("# File generated by AALSDK/ase/scripts/generate_ase_environment.py #\n")
fd.write("#                                                                  #\n")
fd.write("####################################################################\n")
fd.write("\n")

########################################################
# Check if VHDL files exist, populate if any
########################################################
print("")
print("Finding VHDL files ... ")
str = ""
vhdl_filepaths = ""
for extn in VHD_EXTENSIONS:
    str = str + commands_getoutput("find -L " + str_dirlist + " -type f -name *" + extn)
    if len(str) != 0:
        str = str + "\n"
if len(str.strip()) != 0:
    open(VHDL_FILE_LIST, "w").write(str)
    vhdl_filepaths = str
    print("DUT_VHD_SRC_LIST = " + VHDL_FILE_LIST)
    fd.write("DUT_VHD_SRC_LIST = " + VHDL_FILE_LIST + " \n\n")
else:
    print("No VHDL files were found !")

########################################################
# Check if V/SV files exist, populate if any
########################################################
print("")
print("Finding {System}Verilog files ... ")
str = ""
vlog_filepaths = ""
cmd = ""
for extn in VLOG_EXTENSIONS:
    cmd = "find -L " + str_dirlist + " -type f -name *pkg*" + extn
    str = str + commands_getoutput(cmd)
    if len(str) != 0:
        str = str + "\n"
for extn in VLOG_EXTENSIONS:
    cmd = "find -L " + str_dirlist + " -type f -name *" + extn + " -not -name *pkg*" + extn
    str = str + commands_getoutput(cmd)
    if len(str) != 0:
        str = str + "\n"
if len(str) != 0:
    open(VLOG_FILE_LIST, "w").write(str)
    vlog_filepaths = str
    print("DUT_VLOG_SRC_LIST = " + VLOG_FILE_LIST)
    fd.write("DUT_VLOG_SRC_LIST = " + VLOG_FILE_LIST + " \n\n")
else:
    print("No {System}Verilog files were found !")

########################################################
# Recursively find and add directory locations for VH
########################################################
print("")
print("Finding include directories ... ")
str = commands_getoutput("find -L " + str_dirlist + " -type d")
str = str.replace("\n", "+")
if len(str) != 0:
    print("DUT_INCDIR = " + str)
    fd.write("DUT_INCDIR = " + str + "\n\n")

#############################################
# Find ASE HW files ###
#############################################
pwd = commands_getoutput("pwd").strip()
# ase_hw_files_str = ""
# ase_hw_files_str = commands_getoutput ("find -L " + pwd + "/hw/ -name *pkg*.v -or -name *pkg*.sv")
# ase_hw_files_str = ase_hw_files_str + commands_getoutput ("find -L " + pwd + "/hw/ -name *.v -or -name *.sv -not -name *pkg*sv -not -name *pkg*v")
# if len(ase_hw_files_str) == 0:
#     ase_functions.begin_red_fontcolor()
#     print("ERROR: ASE files could not be found, please ensure you run this script in the <AALSDK>/ase directory\n")
#     print("ERROR: generate_ase_environment.py script will now exit.\n")
#     ase_functions.end_red_fontcolor()
#     sys.exit(0)
# ase_hw_files_str = ase_hw_files_str.replace("\n", " \\\n\t")
# ase_hw_files_str = ase_hw_files_str[:-1]
# fd.write("ASEHW_FILE_LIST = \\\n\t")
# fd.write(ase_hw_files_str)
# fd.write("\n\n")

#################################################################
# Recursively find and add directory locations for ASE HW files
#################################################################
# print("")
# print("Finding include directories ... ")
# str = commands_getoutput("find -L " + pwd + "/hw/ -type d")
# str = str.replace("\n", "+")
# if len(str) != 0:
#     print("ASE_INCDIR = " + str)
#     fd.write("ASE_INCDIR = " + str + "\n\n")

#############################################
# Find ASE SW files
#############################################
# ase_sw_files_str = commands_getoutput ("find -L " + pwd + "/sw/ -name *.c ! -name app_backend.c")
# ase_sw_files_str = ase_sw_files_str.replace("\n", " \\\n\t")
# ase_sw_files_str = ase_sw_files_str[:-1]
# fd.write("ASESW_FILE_LIST = \\\n\t")
# fd.write(ase_sw_files_str)
# fd.write("\n\n")

#############################################
# Set up ASE_TOP
#############################################
fd.write("ASE_TOP = ase_top")
fd.write("\n\n")

#############################################
# Update SIMULATOR
#############################################
fd.write("SIMULATOR ?= ")
fd.write(TOOL_BRAND)

fd.write("\n\n")
fd.close()

#############################################
# Write tool specific scripts
#############################################
if tool_type == "VCS":
    print ("Generating VCS specific Runtime TCL scripts")
    ### Write Synopsys Setup file& TCL script ###
    open("synopsys_sim.setup", "w").write("WORK > DEFAULT\nDEFAULT : ./work\n")
    open("vcs_run.tcl", "w").write("dump -depth 0 \ndump -aggregates -add / \nrun \nquit\n")
elif tool_type == "QUESTA":
    ### Generate .DO file ###
    print ("Generating Modelsim specific scripts")
    open("vsim_run.tcl", "w").write("add wave -r /* \nrun -all\n")

#############################################
# Print special character message
#############################################
if (special_chars_in_path == 1):
    ase_functions.begin_red_fontcolor()
    print("Special characters found in path name --- RTL simulator tools may have trouble deciphering paths")
    ase_functions.end_red_fontcolor()

#############################################
# Module repetition check
#############################################
vhdl_filepaths = vhdl_filepaths.replace("\n", " ").split()
vlog_filepaths = vlog_filepaths.replace("\n", " ").split()

all_filepaths = vhdl_filepaths + vlog_filepaths
module_namelist = []

for filepath in all_filepaths:
    file_content = open(filepath).readlines()    
    for line in file_content:
        strip_line = line.strip()
        if strip_line.startswith("//"):
            continue
        elif strip_line.startswith("module"):
            words = strip_line.split()
            modname = words[1]
            module_namelist.append(modname)

ase_functions.begin_red_fontcolor()
if (has_duplicates(module_namelist) == True):
    print("\n")
    print("Duplicate module names were found in the RTL file lists.")
    print("Please remove them manually as RTL compilation is expected to FAIL !")    
ase_functions.end_red_fontcolor()

#############################################
# Print instructions
#############################################
print_instructions()

