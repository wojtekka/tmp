MCU = attiny2313
NAME = player
OBJS = debug.o spi.o mmc.o player.o vs1001k.o
PROG = stk200
SUDO = 

CFLAGS = -Wall -Os -mmcu=$(MCU) -Wstrict-prototypes -DF_CPU=6000000L

default:	$(NAME).bin $(NAME).hex $(NAME).lst $(NAME)-remove

$(NAME).bin:	$(NAME)
	avr-objcopy -j .text -j .data -O binary $(NAME) $(NAME).bin
	@chmod -x $(NAME).bin

$(NAME).hex:	$(NAME)
	avr-objcopy -j .text -j .data -O ihex $(NAME) $(NAME).hex
	@sed '/^:......05/d' < $(NAME).hex > $(NAME).hex-; mv -f $(NAME).hex- $(NAME).hex

$(NAME).lst:	$(NAME)
	avr-objdump -h -S -D --stabs --disassemble-zeroes $(NAME) > $(NAME).lst
	avr-nm $(NAME) -n >> $(NAME).lst

.PHONY:	$(NAME)-remove

$(NAME)-remove:
	@rm -f $(NAME)

$(NAME):	$(OBJS)
	avr-gcc $(CFLAGS) $(OBJS) -o $(NAME)

%.o:	%.c	Makefile
	avr-gcc $(CFLAGS) $< -c -o $@
	@avr-gcc $(CFLAGS) $< -S 2> /dev/null

.PHONY:	clean

clean:
	rm -f $(NAME) *.s *.hex *.bin *.o *.lst *~ core

.PHONY:	poke

poke:
	$(SUDO) uisp -dt_sck=1 -dprog=$(PROG) --erase --upload if=$(NAME).hex --verify

.PHONY:	zip

zip:
	zip -r tmp-`date +'%Y%m%d'`.zip README Makefile *.c *.h spiread
