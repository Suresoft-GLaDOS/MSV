# Original Prophet
Original Prophet compatible with LLVM/Clang 11.

### Difference with original replication
1. Update LLVM/Clang version to 11.
2. Fix benchmarks dependencies and scripts.
3. Update Python scripts to use Python 3.

## Dependencies
* LLVM/Clang 11
* zlib
* libncurses
* libexplain

## How to run
1. Build Prophet:
```
aclocal
libtoolize --force
autoheader
autoconf
automake --add-missing
./configure
make
make install
```
2. Use benchmarks from original replication: https://www.cs.toronto.edu/~fanl/program_repair/
3. Modify `single_case_timeout` in repair.conf (original replicated benchmarks may have too low timeout to condition synthesis)
4. Run
```
prophet -r <path-to-workdir> -first-n-loc <top-n-fl-location> -feature-para <path-to-feature-file> -timeout <hour> [optional option...]
```
to run prophet.

### Important Options
* `-r <path-to-workdir>`: Path to work directory that contains repair.conf. For replicated benchmarks, `<subject>-case-<version>/<subject>-<version>-workdir`.
* `-first-n-loc <top-n-fl-location>`: Use top n location from fault localization. Optional, default 5000. Prophet paper used 200.
* `-feature-para <path-to-feature-file>`: Path to feature file. Usually `MSV/crawler/para-<subject>.out`. For gmp, gzip and lighttpd, `MSV/crawler/para-all.out`. If you use `-replace-ext` option, `MSV/crawler/para-rext-<subject>.out` or `MSV/crawler/para-rext-all.out`.
* `-timeout <hour>`: Set timeout in hour. Default, infinite (until all `-first-n-loc` tried`).

### Optional Options
* `-replace-ext`: Use replace extension.
* `-init-only`: Only run fault localization, and terminate.
* `-skip-verify`: Skip initial test verification (Remove passed failing test, failed passing test)
* `-consider-all`: Use all suspicious file, not only specified file in repair.conf
