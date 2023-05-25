# MSV
An APR tool for C/C++ programs, based on Prophet.\
Repair with meta-program, search with Casino.

## Dependencies
* LLVM/Clang 11
* Python 3.6+
* zlib
* libncurses
* pthread

## Docker Image
Run `docker pull kyj1411/msv` to get docker image.

## Manual Build
Just run `build.sh`.

## Run MSV
`python3 msv-runner.py [options] <src-dir> <work-dir> <msv-path>`

* `<src-dir>`: root directory of project
* `<work-dir>`: working directory. work directory of project will be generated in here.
* `<msv-path>`: root directory of MSV

Every paths should be absolute path!

### Options
* `-r`: run MSV and generate meta-program under `<work-dir>/<project>-workdir/src`
* `-s`: use specific FL result file instead of prophet/SPR's FL. only works with `-r`.

## Example programs
Example programs are in `examples/engagement1`.

