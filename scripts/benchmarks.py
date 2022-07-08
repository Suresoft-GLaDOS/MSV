BENCHMARKS=[
"gmp-13421",
"gmp-14167", # Build fail

"gzip-118a107f2d3ae5",
"gzip-39a362ae9d9b00",
"gzip-884ef6d16c6", # correct
"gzip-f17cbd13a1d0a7", # correct

"libtiff-tests-1563270c^1-1563270c",
"libtiff-tests-2e42d63f", # correct
"libtiff-tests-bdba15c", # Meta-program fail
"libtiff-tests2-0a36d7f", # correct
"libtiff-tests-eec7ec0", # correct
"libtiff-tests2-3edb9cd", # correct
"libtiff-tests2-371336d^1-371336d", # correct
"libtiff-tests2-e8a47d4^1-e8a47d4", # correct
"libtiff-tests2-eec4c06^1-eec4c06",

"lighttpd-1914", # correct
"lighttpd-1807",
"lighttpd-1949",
"lighttpd-2226",
"lighttpd-2259",
"lighttpd-2662", # correct
"lighttpd-2786", # Meta-program fail

"php-09273098521913a", # correct
"php-09b990f",
"php-1056c57f", # correct
"php-109b8e9",
"php-1d984a7", # correct
"php-1f49902",
"php-1a8b87d",
"php-2adf58", # correct
"php-1e91069", # correct
"php-2e206e07",
"php-2e25ec9",
"php-2a6968",
"php-3acdca", # correct
"php-453c954^1-453c954", # correct
"php-4756d7", # No patch in search space
"php-5a8c917", # correct
"php-77ed819",
"php-5bb0a44", # No patch in search space
"php-821d716", # correct
"php-8ba00176", # correct
"php-8deb11c",
"php-991ba131",
"php-93f65cd",
"php-b84967d", # correct
"php-b9b1fb1",
"php-c2fe893", # No patch in search space
"php-d1d61ce",
"php-ecb9d80",
"php-ee83270", # correct
"php-cebe2a213^1-cebe2a213", # No patch in search space
"php-f2329f1",
"php-f455f8^1-f455f8", # correct
"php-efcb9a71", # correct

"python-69474-69470",
"python-69372-69368",
"python-69710", # No patch in search space
"python-69224",
"python-69935",
"python-70023-70019",
"python-70101-70098", # No patch in search space
"python-70059-70056",

"wireshark-2d2815b^1-2d2815b",
"wireshark-9a2ba01",
"wireshark-b51eba6", # Meta-program fail
"wireshark-464486e^1-464486e",

"libtiff-tests-8a184dc",
'libtiff-tests-740a6187',
'libtiff-tests-e779b6cd',
'wireshark-6d801a205',
'wireshark-03d7987b',
]

FAILED_BENCHMARKS=[
"gmp-14167", # Build fail
"libtiff-tests-bdba15c", # Meta-program fail
"lighttpd-2786", # Meta-program fail
"php-4756d7", # No patch in search space
"php-5bb0a44", # No patch in search space
"php-c2fe893", # No patch in search space
"php-cebe2a213^1-cebe2a213", # No patch in search space
"python-69710", # No patch in search space
"python-70101-70098", # No patch in search space
"wireshark-b51eba6", # Meta-program fail
"libtiff-tests-8a184dc",
'libtiff-tests-740a6187',
'libtiff-tests-e779b6cd',
'wireshark-6d801a205',
'wireshark-03d7987b',
]

BENCHMARKS_URL=[
"gmp-13421",
"gmp-14167", # Build fail

"gzip-118a107f2d3ae5",
"gzip-39a362ae9d9b00",
"gzip-884ef6d16c6", # correct
"gzip-f17cbd13a1d0a7", # correct

"libtiff-tests-1563270c^1",
"libtiff-tests-2e42d63f", # correct
"libtiff-tests-bdba15c", # Meta-program fail
"libtiff-tests2-0a36d7f", # correct
"libtiff-tests-eec7ec0", # correct
"libtiff-tests2-3edb9cd", # correct
"libtiff-tests2-371336d^1", # correct
"libtiff-tests2-e8a47d4^1", # correct
"libtiff-tests2-eec4c06^1",

"lighttpd-1914", # correct
"lighttpd-1807",
"lighttpd-1949",
"lighttpd-2226",
"lighttpd-2259",
"lighttpd-2662", # correct
"lighttpd-2786", # Meta-program fail

"php-09273098521913a", # correct
"php-09b990f",
"php-1056c57f", # correct
"php-109b8e9",
"php-1d984a7", # correct
"php-1f49902",
"php-1a8b87d",
"php-2adf58", # correct
"php-1e91069", # correct
"php-2e206e07",
"php-2e25ec9",
"php-2a6968",
"php-3acdca", # correct
"php-453c954^1-453c954", # correct
"php-4756d7", # No patch in search space
"php-5a8c917", # correct
"php-77ed819",
"php-5bb0a44", # No patch in search space
"php-821d716", # correct
"php-8ba00176", # correct
"php-8deb11c",
"php-991ba131",
"php-93f65cd",
"php-b84967d", # correct
"php-b9b1fb1",
"php-c2fe893", # No patch in search space
"php-d1d61ce",
"php-ecb9d80",
"php-ee83270", # correct
"php-cebe2a213^1-cebe2a213", # No patch in search space
"php-f2329f1",
"php-f455f8^1-f455f8", # correct
"php-efcb9a71", # correct

"python-69474",
"python-69372",
"python-69710", # No patch in search space
"python-69224",
"python-69935",
"python-70023",
"python-70101-70098", # No patch in search space
"python-70059-70056",

"wireshark-2d2815b^1",
"wireshark-9a2ba01",
"wireshark-b51eba6", # Meta-program fail
"wireshark-464486e^1",

"libtiff-bug-2008-12-30-362dee5-565eaa2", # Meta-program fail
'libtiff-bug-2007-07-08-bd2f947-ccc10c7',
'libtiff-bug-2007-11-23-82e378c-cf05a83',
'wireshark-bug-35419-35414',
'wireshark-bug-37171-37170',
]

def get_subject(benchmark):
    if 'gmp' in benchmark:
        return 'gmp'
    elif 'gzip' in benchmark:
        return 'gzip'
    elif 'libtiff' in benchmark:
        return 'libtiff'
    elif 'lighttpd' in benchmark:
        return 'lighttpd'
    elif 'php' in benchmark:
        return 'php'
    elif 'python' in benchmark:
        return 'python'
    elif 'wireshark' in benchmark:
        return 'wireshark'
    assert False and f'Unknown benchmark: {benchmark}'

def get_workdir(benchmark:str):
    first_dash=benchmark.find('-')
    if benchmark.startswith('php-bug'):
        return benchmark+'/'+benchmark+'-workdir'
    else:
        return benchmark[:first_dash]+'-case'+benchmark[first_dash:]+'/'+benchmark+'-workdir'

def get_test_dir(benchmark:str):
    workdir=get_workdir(benchmark)
    test_dir=''
    with open(f'/root/project/MSV-experiment/benchmarks/{get_subject(benchmark)}/{workdir}/repair.conf','r') as f:
        lines=f.readlines()
        for line in  lines:
            if 'test_dir' in line:
                test_dir=line.split('=')[1].strip()
                break
    
    assert test_dir!=''
    return test_dir