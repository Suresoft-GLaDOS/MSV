if [ $# -ne 1 ]; 
then
    echo "Usage: $0 <benchmark|all>"
    exit 1
fi
benchmark=$1
/root/project/MSV/scripts/run.sh checkout $1
/root/project/MSV/scripts/run.sh search $1
/root/project/MSV/scripts/run.sh check $1