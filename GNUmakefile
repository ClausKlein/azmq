all: ../build
	cmake -B $< -S . -G Ninja # TODO -D ZMQ_ROOT=../../stage
	ninja -C $<
	cd $< && ctest --rerun-failed --output-on-failure
