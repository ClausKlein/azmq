
.PHONY: all
all: build
	cmake -B $< -S . -G Ninja # TODO -D ZMQ_ROOT=../libzmq/stage
	ninja -C $<
	cd $< && ctest --rerun-failed --output-on-failure --timeout 10

build:
	mkdir -p build
