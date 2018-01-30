INCLUDE_DIR := ..
CFLAGS := -I$(INCLUDE_DIR) \
          -fPIC -shared -Wall

INCLUDES := sharedLibEvents.h \
            sharedLibManifest.h \
            sharedLibTypes.h

INCLUDE_DEPS := $(addprefix $(INCLUDE_DIR)/,$(INCLUDES))
$(info $(INCLUDE_DEPS))

all: libtrig.so libpulse.so

libtrig.so: trig.c $(INCLUDE_DEPS)
	gcc $(CFLAGS) -o $@ trig.c -lm

libpulse.so: pulse.c $(INCLUDE_DEPS)
	gcc $(CFLAGS) -o $@ pulse.c

.PHONY: clean
clean:
	rm libtrig.so libpulse.so

