all:
	cd source && make && cd ..
	./render > graph.ppm
	code graph.ppm