if [ -e php-case-f455f8^1-f455f8 ]
then
	mv php-case-f455f8^1-f455f8 1-php-case-f455f8^1-f455f8
fi

if [ -e php-case-2adf58 ]
then
	mv php-case-2adf58 1-php-case-2adf58
fi

if [ -e 1-php-case-efcb9a71 ]
then
	mv 1-php-case-efcb9a71 php-case-efcb9a71
fi

if [[ $1 = "-o" ]];
then
	../tests/scripts/reproduce.py --prophet php-310011-310050 &> result.log
else
	../tests/scripts/reproduce.py --prophet php-310011-310050
fi
