set terminal x11 
set output "test.x11"
unset border
unset zeroaxis
unset key
unset xtics
unset ytics
plot '-' with points pointsize 6
set label "1" at 6.00, 3.75
set label "2" at 10.52, 1.16
set label "3" at 8.79, -3.03
set label "4" at 3.21, -3.03
set label "5" at 1.48, 1.16
set arrow from 6.00,4.00 to 6.00,4.00 nohead
set arrow from 10.28,1.24 to 6.00,4.00 nohead
set arrow from 1.72,1.24 to 6.00,4.00 nohead
set arrow from 6.00,4.00 to 10.28,1.24 nohead
set arrow from 10.28,1.24 to 10.28,1.24 nohead
set arrow from 8.65,-3.24 to 10.28,1.24 nohead
set arrow from 10.28,1.24 to 8.65,-3.24 nohead
set arrow from 8.65,-3.24 to 8.65,-3.24 nohead
set arrow from 3.35,-3.24 to 8.65,-3.24 nohead
set arrow from 8.65,-3.24 to 3.35,-3.24 nohead
set arrow from 3.35,-3.24 to 3.35,-3.24 nohead
set arrow from 1.72,1.24 to 3.35,-3.24 nohead
set arrow from 6.00,4.00 to 1.72,1.24 nohead
set arrow from 3.35,-3.24 to 1.72,1.24 nohead
set arrow from 1.72,1.24 to 1.72,1.24 nohead
6.00 4.00 label "1"
10.28 1.24 label "2"
8.65 -3.24 label "3"
3.35 -3.24 label "4"
1.72 1.24 label "5"
