all:
	@cd source &&make -j8 && cd ..
	time ./render --name graph.ppm -x 400 -y 200 --sample-rate 100 --recursive-depth 50
	@code graph.ppm