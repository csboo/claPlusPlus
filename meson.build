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
include_dir = include_directories('include', 'include/arg')

# Build executable directly from all necessary sources
executable('claPlusPlus',
  sources: [
    'src/example.cpp',
    'src/Parser.cpp',
    'src/BaseArg.cpp'
  ],
  include_directories: include_dir,
  install: false
)

parser_lib = static_library('clap_parser', 
  sources: ['src/Parser.cpp', 'src/BaseArg.cpp'],
  include_directories: include_dir,
  install: false
)

tests = [
  'test_priority.cpp',
  'test_flag.cpp',
  'test_combo.cpp',
  'parse_types.cpp',
  'type_parse_expectedfail.cpp'
]

test_bins = []
foreach test : tests
  name = test.replace('.cpp', '')
  bin = executable(
     name,
     'tests/' + test,
     link_with: parser_lib,
     include_directories: include_dir,
     install: false
  )
  test_bins += [bin]
endforeach

# register a bunch of test cases
# TODO basic tests for constructing stuff
# 1) priority tests (default value '1')
test('priority_default', test_bins[0],
     args: [], 
     env: ['EXPECTED=1'])
test('priority_auto_env', test_bins[0],
     args: [], 
     env: ['TEST_PRIORITY_VAL=2', 'EXPECTED=2'])
test('priority_from_env', test_bins[0],
     args: [], 
     env: ['VAL=3', 'TEST_PRIORITY_VAL=2', 'EXPECTED=3'])
test('priority_manual', test_bins[0],
     args: ['--val', '4'], 
     env: ['VAL=3', 'TEST_PRIORITY_VAL=2', 'EXPECTED=4'])

# 2) flag tests
test('flag_absent', test_bins[1],
     args: [], 
     env: ['EXPECTED=0'])
test('flag_present', test_bins[1],
     args: ['--opt'], 
     env: ['EXPECTED=1'])

# 3) combo tests
test('combo_default', test_bins[2],
     args: [], 
     env: ['EXPECTED_V=10', 'EXPECTED_B=0'])
test('combo_flag_first', test_bins[2],
     args: ['--flag', '--val', '7'],
     env: ['EXPECTED_V=7', 'EXPECTED_B=1'])
test('combo_value_first', test_bins[2],
     args: ['--val', '8', '--flag'],
     env: ['EXPECTED_V=8', 'EXPECTED_B=1'])
test('combo_env_and_manual', test_bins[2],
     args: ['--val', '9'],
     env: ['VAL=4', 'TEST_COMBO_VAL=5', 'EXPECTED_V=9', 'EXPECTED_B=0'])
   
# 4) type parse tests
test('parse_types', test_bins[3])

# 5) type parse tests
test('parse_types_overflow', test_bins[4])
