project('UDP Example','cpp',version: '0.1.0')

cpp_args = []

inc = include_directories('include')

# Define the source files
# src = ['src/logManager.cpp']

# Subdirectories
subdir('src')
subdir('tools')

### Custom target to run clang-format
##clang_format = find_program('clang-format', required: true)
##fmt_cmd = [clang_format, '-i', 'tools/*.cpp']
##run_target('format', command : fmt_cmd)