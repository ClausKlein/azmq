
.PHONY: all diestclean
all: build
	cmake -B $< -S . -G Ninja -D ZMQ_ROOT=../libzmq/stage
	ninja -C $<
	cd $< && ctest --rerun-failed --output-on-failure --timeout 17

build:
	mkdir -p $@

distclean:
	rm -rf build
