CMD=mk_kdc mk_apeer mk_bpeer

all: $(CMD)

mk_kdc:
	cd kdc && $(MAKE)
mk_apeer:
	cd apeer && $(MAKE)
mk_bpeer:
	cd bpeer && $(MAKE)
clean:
	cd kdc && make clean
	cd apeer && make clean
	cd bpeer && make clean
	cd include && make clean
	rm -f *~