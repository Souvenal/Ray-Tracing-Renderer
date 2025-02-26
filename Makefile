all:
	cd source && make && cd ..
	time  ./render > graph.ppm 5000
	code graph.ppm