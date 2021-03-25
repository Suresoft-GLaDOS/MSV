if [ -e php-case-efcb9a71 ]
then
	mv php-case-efcb9a71 1-php-case-efcb9a71
fi

if [ -e php-case-2adf58 ]
then
	mv php-case-2adf58 1-php-case-2adf58
fi

if [ -e 1-php-case-f455f8^1-f455f8 ]
then
	mv 1-php-case-f455f8^1-f455f8 php-case-f455f8^1-f455f8
fi

if [[ $1 = "-o"  ]];
then
	../tests/scripts/reproduce.py --prophet --bug-file --skip-build php-307562-307561 &> result.log
else
	../tests/scripts/reproduce.py --prophet --bug-file --skip-build php-307562-307561
fi
