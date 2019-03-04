TARGET		= $(CURDIR)
OS		= $(shell cd ../..; pwd)
include target.cfg

TEST		= test-timer
ifeq (,$(findstring USEC_TIMER,$(CFLAGS)))
TESTS		= test-debug.hex test-lcd.hex test-task-lcd.hex tetris.hex
TESTS		+= test-timer.hex test-timer-lcd.hex test-latency.hex test-tswitch.hex
TESTS		+= phil-lcd.hex phil-uart.hex test-can.hex
TESTS		+= test-mac.hex test-eth.hex test-tcp.hex test-udp.hex test-telnetd.hex
TESTS		+= test-led.hex test-iwdt.hex test-dma.hex test-irq.hex 
TESTS		+= test-hid.hex test-adc.hex usb-loader.hex
#TESTS		+= test-flash.hex test-dac-dma.hex test-fat32-fw.hex
else
TESTS		+= test-timer.hex
endif

OUTS		= $(TESTS:%.hex=%.elf) #udpping

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
