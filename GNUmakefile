
.PHONY: all
all: build
	cmake -B $< -S . -G Ninja # TODO -D ZMQ_ROOT=../libzmq/stage
	ninja -C $<
	run-clang-tidy.py -p $< -checks='modernize-*,-modernize-use-trailing-return-type,misc-*,-misc-non-private-member-variables-in-classes,cert-*,-llvmlibc-*,-altera-*,' doc/examples #XXX azmq
	cd $< && ctest --rerun-failed --output-on-failure --timeout 17

build:
	mkdir -p build
