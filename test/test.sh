#!/bin/sh

echo 1 &&  ./a.out <  1.in >  1.out #2>/dev/null
if diff 1.out 1.ans; then
	echo "Accepted"
else
	echo "Wrong Answer"
fi

echo 2 &&  ./a.out <  2.in >  2.out #2>/dev/null
if diff 2.out 2.ans; then
	echo "Accepted"
else
	echo "Wrong Answer"
fi

echo 3 &&  ./a.out <  3.in >  3.out #2>/dev/null
if diff 3.out 3.ans; then
	echo "Accepted"
else
	echo "Wrong Answer"
fi

echo 4 &&  ./a.out <  4.in >  4.out #2>/dev/null
if diff 4.out 4.ans; then
	echo "Accepted"
else
	echo "Wrong Answer"
fi

echo 5 &&  ./a.out <  5.in >  5.out #2>/dev/null
if diff 5.out 5.ans; then
	echo "Accepted"
else
	echo "Wrong Answer"
fi
