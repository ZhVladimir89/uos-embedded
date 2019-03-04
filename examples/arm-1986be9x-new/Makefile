TARGET		= $(CURDIR)
OS		= $(shell cd ../..; pwd)
include target.cfg

TEST		= test-ds7505
TESTS		= test-debug.hex test-task.hex test-timer.hex philosophers.hex
TESTS		+= test-latency.hex test-tswitch.hex
TESTS		+= test-gpio-output.hex test-button.hex test-intr.hex
TESTS		+= test-can-output.hex test-can-input.hex
TESTS		+= test-ds7505.hex

OUTS		= $(TESTS:%.hex=%.elf)

# Using MILPROG utility.
PROG            = milprog

all:		$(OUTS) $(TESTS) $(TEST).dis $(TEST).bin
		$(SIZE) $(OUTS)

$(OUTS) $(TESTS): startup.o libuos.a

clean:
		rm -rf *~ *.[oasi] *.elf *.lst *.dis *.bin *.map .deps $(TESTS) $(MODULES) udpping

probe:
		$(PROG)

load:		$(TEST).hex
		$(PROG) $<

proxy:
		openocd -f openocd.tcl

gdb:		$(TEST).elf
		arm-elf-gdb -x gdbinit $(TEST).elf

insight:	$(TEST).elf
		arm-elf-insight -x gdbinit $(TEST).elf &

udpping:	udpping.c
		gcc -Wall -O $< -lpthread -o $@

include $(OS)/sources/rules.mak
