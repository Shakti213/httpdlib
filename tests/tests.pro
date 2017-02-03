TEMPLATE = subdirs

SUBDIRS = \
    string_util

run_target.target = run_all
run_target.commands =@set MAKEFLAGS=$(MAKEFLAGS)$$escape_expand(\n\t)
for(s, SUBDIRS) {
    message(test_$$s)
    run_target.commands+=cd $$s && $(MAKE) -f Makefile run$$escape_expand(\n\t)
}

QMAKE_EXTRA_TARGETS = run_target
