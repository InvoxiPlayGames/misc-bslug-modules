MODULES += disable-ssl-validation
MODULES += usb-gecko-console
MODULES += vi-overscan

Q      ?= @
LOG    ?= @echo $@

all: $(addsuffix _module,$(MODULES))

%_module:
	$Q$(MAKE) -C $*

.PHONY: all