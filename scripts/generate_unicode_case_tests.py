#!/usr/bin/env python

test_file_form = '''\
// Warning! This file is autogenerated.
#include <boost/text/case_mapping.hpp>

#include <gtest/gtest.h>


using namespace boost::text;

{0}
'''

test_form = '''\
TEST(case_mapping, {0:03})
{{
{1}
}}
'''

test_block = '''\
    {{ // {0} <-> {1}
{2}
{3}
    }}
'''


condition_prefixes = {
    'Final_Sigma': {
        'valid': [
            ['0345', '0345'], # 0345 is cased and case-ignorable
            ['0069'],         # 0069 is cased only
            ['0069', '0345'],
            ['0069', '1DFE']  # 1DFE is case-ignorable only
        ],
        'invalid': [
            ['0345'],
            ['1DFE'],
            ['1DFE', '1DFE'],
            ['']
        ]
    },
    'After_Soft_Dotted': {
        'valid': [
            ['006A', '006A'], # 006A is soft-dotted (and thus also ccc=0)
            ['006A', '07F3']  # 07F3 is ccc=230 and not soft-dotted
        ],
        'invalid': [
            ['006A'],
            ['07F3'],
            ['07F3', '07F3'],
            ['']
        ]
    },
    'More_Above': {           # none
        'valid': [''],
        'invalid': ['']
    },
    'Not_Before_Dot': {       # none
        'valid': [''],
        'invalid': ['']
    },
    'After_I': {
        'valid': [
            ['0049', '0049'], # 'I' (ccc=0)
            ['0049', '0737']  # 0737 is ccc=220
        ],
        'invalid': [
            ['0049'],
            ['004A'],         # 'J' (ccc=0)
            ['']
        ]
    }
}
condition_suffixes = {
    'Final_Sigma': {
        'valid': [
            ['0345'],
            ['1dfe'],
            ['1dfe', '1dfe'],
            ['']
        ],
        'invalid': [
            ['0345', '0345'],
            ['0069'],
            ['0345', '0069'],
            ['1dfe', '0069']
        ]
    },
    'After_Soft_Dotted': {    # none
        'valid': [''],
        'invalid': ['']
    },
    'More_Above': {
        'valid': [
            ['0737', '07F3'], # 0737 is ccc=220, 07F3 is ccc=230
            ['07F3']
        ],
        'invalid': [
            ['0737', '0737'],
            ['0737', '006A'], # 006A is ccc=0
            ['006A', '07F3'],
            ['']
        ]
    },
    'Not_Before_Dot': {
        'valid': [
            ['0737', '0737'],
            ['0737', '006A'], # 006A is ccc=0
            ['006A', '0307'],
            ['']
        ],
        'invalid': [
            ['0737', '0307'], # 0737 is ccc=220
            ['0307']
        ]
    },
    'After_I': {              # none
        'valid': [''],
        'invalid': ['']
    }
}

def to_array(name, l):
    return 'std::vector<uint32_t> {}({{{}}});'.format(
        name, ', '.join(map(lambda x: '0x' + x, l))
    )

def decls(name_1, l_1, name_2, l_2):
    return '''\
        {}
        {}
        std::vector<uint32_t> result;
'''.format(to_array(name_1, l_1), to_array(name_2, l_2))

def checks(name_1, name_2):
    return '''\
        to_{1}({0}, std::back_inserter(result));
        EXPECT_EQ(result, {1});
        result.clear();
        to_{0}({1}, std::back_inserter(result));
        EXPECT_EQ(result, {0});
'''.format(name_1, name_2)

def case_mapping_tests(special_casing):
    tests = []
    test_idx = 0

    lines = open(special_casing, 'r').readlines()
    for line in lines:
        line = line[:-1]
        if not line.startswith('#') and len(line) != 0:
            fields = map(lambda x: x.strip(), line.split(';'))
            lower = fields[1].strip().split(' ')
            if lower == ['']:
                lower = []
            title = fields[2].strip().split(' ')
            if title == ['']:
                title = []
            upper = fields[3].strip().split(' ')
            if upper == ['']:
                upper = []
            conditions_ = []
            if 3 < len(fields) and '#' not in fields[4]:
                conditions_ = fields[4].strip().split(' ')
            if len(lower):
                if len(title):
                    pass # TODO
                if len(upper):
                    if len(conditions_) == 0:
                        block = test_block.format(
                            'lower', 'upper',
                            decls('lower', lower, 'upper', upper),
                            checks('lower', 'upper')
                        )
                        test = test_form.format(test_idx, block)
                        test_idx += 1
                        tests.append(test)
            if len(title) and len(upper):
                pass # TODO

    cpp_file = open('case_mapping.cpp', 'w')
    cpp_file.write(test_file_form.format('\n'.join(tests)))

case_mapping_tests('SpecialCasing.txt')
