TARGET		= $(CURDIR)
OS		= $(shell cd ../..; pwd)
include target.cfg

TEST		= test-uart
TESTS		+= test-led.bin
TESTS		+= test-debug.bin
TESTS		+= test-task.bin
TESTS		+= test-tswitch.bin
TESTS		+= test-latency.bin
TESTS		+= test-button.bin
TESTS		+= test-button-intr.bin
TESTS		+= test-delay.bin
TESTS		+= test-timer.bin
TESTS       += test-timer-period.bin
TESTS       += test-adt7461.bin
TESTS       += test-uart.bin
#TESTS       += test-uart-stream.bin

ifneq (,$(findstring NSEC_TIMER,$(CFLAGS)))

    ifneq (,$(findstring RTC_TIMER,$(CFLAGS)))
        TESTS       += test-rtc-alarm.bin
    endif
    
    TESTS		+= test-gpio-output.bin

else

    TESTS		+= philosophers.bin
    TESTS		+= test-usbdev.bin
    TESTS		+= test-hid.bin
    TESTS		+= test-prog-flash.bin
    TESTS		+= test-timer-timeout.bin
    TESTS		+= test-several-signals.bin
    TESTS		+= test-several-signals2.bin
    TESTS       += test-flash.bin
    TESTS       += test-fat32-fw.bin
    TESTS       += test-fat32-fw-file-out.bin
    TESTS       += read-temp.bin
    TESTS       += test-ds7505.bin
    TESTS       += test-ds7505-nodma.bin
    TESTS       += test-mic184.bin
    
endif


OUTS		= $(TESTS:%.bin=%.elf)
HEXES		= $(TESTS:%.bin=%.hex)

all:		$(OUTS) $(TESTS) $(HEXES) $(TEST).dis
		$(SIZE) $(OUTS)

$(OUTS) $(TESTS): startup.o libuos.a

clean:
		rm -rf *~ *.[oasi] *.elf *.lst *.dis *.map .deps $(TESTS) $(HEXES) $(MODULES)

load:		$(TEST).bin
		st-flash --reset write $< 0x08000000

include $(OS)/sources/rules.mak
