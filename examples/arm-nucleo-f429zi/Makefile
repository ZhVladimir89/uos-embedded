TARGET		= $(CURDIR)
OS		= $(shell cd ../..; pwd)
include target.cfg

TEST		= test-adc
TESTS		+= test-led.bin
TESTS		+= test-debug.bin
TESTS		+= test-task.bin
TESTS		+= test-timer.bin
TESTS		+= test-tswitch.bin
TESTS		+= test-latency.bin
TESTS		+= philosophers.bin
TESTS		+= test-button.bin
TESTS		+= test-button-intr.bin
TESTS		+= test-gpio-output.bin
TESTS		+= test-uart.bin
TESTS		+= test-rtc.bin
TESTS		+= test-flash.bin
TESTS		+= test-adt7461.bin
TESTS		+= test-ds7505.bin
TESTS		+= test-mic184.bin
TESTS		+= test-prog-flash.bin
TESTS		+= test-adc.bin
TESTS		+= test-adc-interleaved.bin
OUTS		= $(TESTS:%.bin=%.elf)

all:		$(OUTS) $(TESTS) $(TEST).dis
		$(SIZE) $(OUTS)

$(OUTS) $(TESTS): startup.o libuos.a

clean:
		rm -rf *~ *.[oasi] *.elf *.lst *.dis .deps $(TESTS) $(MODULES)

load:		$(TEST).bin
		st-flash write $< 0x08000000

include $(OS)/sources/rules.mak
