MCU	=	atmega328p

CC		=	avr-gcc
COPY	=	avr-objcopy
FLASHER	=	avrdude 

# clock osilator in schema
F_CPU		=	F_CPU=16000000UL
# arduino is programmer fond in preambule module 00
PROG		=	arduino
# baud is a unit of transmission speed fond in preambule module 00
BAUD		=	115200
# default Port usb find in doc arduino
PORT		=	/dev/ttyUSB0

NAME = main
DIR_BIN = bin

SRC = main.c

HEX_NAME = $(addprefix $(addprefix $(DIR_BIN)/, $(NAME)), .hex)
BIN_NAME = $(addprefix $(addprefix $(DIR_BIN)/, $(NAME)), .bin)

OBJ := $(SRC:%.c=$(DIR_BIN)/%.o)

all: hex flash


$(DIR_BIN)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) -D$(F_CPU) -O2 -mmcu=$(MCU) -c $< -o $@

$(BIN_NAME): $(OBJ)
	$(CC) -D$(F_CPU) -O2 -mmcu=$(MCU) -o $@ $<

hex: $(BIN_NAME)
# ihex format code hex
	$(COPY) -O ihex $^ $(HEX_NAME)

flash:
# avrdude man
	$(FLASHER) -p $(MCU) -P $(PORT) -b$(BAUD) -c$(PROG) -U flash:w:$(HEX_NAME)


clean:
	rm -rf $(DIR_BIN)

re: fclean	all

.PHONY: all hex flash clean fclean

