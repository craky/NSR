set term pdf
set output "time.pdf"

set style data linespoints

#set key top left reverse Left
#set key autotitle columnhead
#set key title "Legend"
#set key box width 1 height 1
#set logscale y
unset key

set ylabel "acceleration"
set xlabel "proc count"

plot "time.dat" using 1:3 with points linewidth 4
