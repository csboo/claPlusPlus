project('claplusplus', 'cpp',
  default_options: [
    'cpp_std=c++23',
    'warning_level=3',
    # 'werror=true',
    'optimization=g',
    'debug=true',
    'b_ndebug=if-release'
  ]
)

# Add include directory for headers
include_dir = include_directories('include')

# Build executable directly from all necessary sources
executable('claPlusPlus',
  sources: [
    'src/old_main.cpp',
    'src/Arg.cpp',
    'src/Parser.cpp'
  ],
  include_directories: include_dir,
  install: false
)

# project('claplusplus', 'cpp',
#   version : '0.0.3',
#   default_options : [
#     'cpp_std=c++23',
#     'warning_level=3',
#     'optimization=g',
#     'debug=true',
#     'b_ndebug=if-release'
#   ]
# )

# # Include directory
# include_dir = include_directories('include')

# # Library
# source_dir = files(
#   'src/Arg.cpp',
#   'src/Parser.cpp',
#   'src/utils.cpp'
# )

# claplusplus_lib = static_library('claplusplus',
#   sources: source_dir,
#   include_directories: include_dir,
#   install: true
# )
 
# # Install headers
# install_headers(
#   'include/Arg.hpp',
#   'include/Parser.hpp',
#   subdir: 'claplusplus'
# )

# executable('app',
#   sources : 'src/old_main.cpp',
#   include_directories: include_dir,
#   link_with: claplusplus_lib,
#   install : false
# )
