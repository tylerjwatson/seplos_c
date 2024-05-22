all: seplosd

library/libseplos.a: .PHONY
	(cd library; make);

commands/seplos/seplos: library/libseplos.a
	(cd commands/seplos; make)

seplosd: library/libseplos.a
	(cd seplosd; make)

install: seplosd
	(cd seplosd; make install)

.PHONY:
