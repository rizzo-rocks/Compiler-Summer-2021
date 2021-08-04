../a.out > result <<EOF
22+33
EOF
if ! diff result expected1; then
  echo error in test 1
  exit 1
fi

../a.out > result <<EOF
2*(3+5)
EOF
if ! diff result expected2; then
  echo error in test 2
  exit 1
fi


../a.out > result <<EOF
-2
EOF
if ! diff result expected3; then
  echo error in test 3
  exit 1
fi


../a.out > result <<EOF
--2
EOF
if ! diff result expected4; then
  echo error in test 4
  exit 1
fi

../a.out > result <<EOF
-(2+3)+5
EOF
if ! diff result expected5; then
  echo error in test 5
  exit 1
fi

../a.out > result <<EOF
5+-(2+3)
EOF
if ! diff result expected6; then
  echo error in test 6
  exit 1
fi

../a.out > result <<EOF
1/2-3
EOF
if ! diff result expected7; then
  echo error in test 7
  exit 1
fi

../a.out > result <<EOF
(-(1+2)/3)
EOF
if ! diff result expected8; then
  echo error in test 8
  exit 1
fi

../a.out > result <<EOF
-((1--2)*3)
EOF
if ! diff result expected9; then
  echo error in test 9
  exit 1
fi

../a.out > result <<EOF
2&3
EOF
if ! diff result expected10; then
  echo error in test 10
  exit 1
fi

../a.out > result <<EOF
(1&2)|(3&4)
EOF
if ! diff result expected11; then
  echo error in test 11
  exit 1
fi

rm result*
echo Passed.
