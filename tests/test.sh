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

../a.out > result <<EOF
(1)
EOF
if ! diff result expected12; then
  echo error in test 12
  exit 1
fi

../a.out > result <<EOF
(1*2)^3
EOF
if ! diff result expected13; then
  echo error in test 13
  exit 1
fi

../a.out > result <<EOF
dx1+dx2
EOF
if ! diff result expected14; then
  echo error in test 14
  exit 1
fi

../a.out > result <<EOF
x=55
EOF
if ! diff result expected15; then
  echo error in test 15
  exit 1
fi

../a.out > result <<EOF
x=y=5
EOF
if ! diff result expected16; then
  echo error in test 16
  exit 1
fi

../a.out > result <<EOF
x=y=5
1+2*3
EOF
if ! diff result expected17; then
  echo error in test 17
  exit 1
fi

../a.out > result <<EOF
x=(y=5)+15
EOF
if ! diff result expected18; then
  echo error in test 18
  exit 1
fi

../a.out > result <<EOF
(y=5)
EOF
if ! diff result expected19; then
  echo error in test 19
  exit 1
fi

../a.out > result <<EOF
1+2-3
EOF
if ! diff result expected20; then
  echo error in test 20
  exit 1
fi

../a.out > result <<EOF
5*3/2
EOF
if ! diff result expected21; then
  echo error in test 21
  exit 1
fi

../a.out > result <<EOF
2^-3*5
EOF
if ! diff result expected22; then
  echo error in test 22
  exit 1
fi

../a.out > result <<EOF
5/2|2+5
EOF
if ! diff result expected23; then
  echo error in test 23
  exit 1
fi

../a.out > result <<EOF
x=y=z=du&-2
EOF
if ! diff result expected24; then
  echo error in test 24
  exit 1
fi

../a.out > result <<EOF
123<=456
EOF
if ! diff result expected25; then
  echo error in test 25
  exit 1
fi

../a.out > result <<EOF
(1<2)&(8!=9)
EOF
if ! diff result expected26; then
  echo error in test 26
  exit 1
fi

../a.out > result <<EOF
dx==dy
EOF
if ! diff result expected27; then
  echo error in test 27
  exit 1
fi

../a.out > result <<EOF
y>x|45>=45
EOF
if ! diff result expected28; then
  echo error in test 28
  exit 1
fi

../a.out > result <<EOF
if 1==true
x=0
endif
EOF
if ! diff result expected29; then
  echo error in test 29
  exit 1
fi

rm result*
echo Passed.
