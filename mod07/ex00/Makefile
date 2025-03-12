MCU	=	atmega328p

CC		=	avr-gcc
COPY	=	avr-objcopy
FLASHER	=	avrdude 

# clock osilator in schema
F_CPU		=	16000000UL
# arduino is programmer fond in preambule module 00
PROG		=	arduino
# baud is a unit of transmission speed fond in preambule module 00
BAUD		=	115200
# default Port usb find in doc arduino
PORT		=	/dev/ttyUSB0


NAME = main
DIR_BIN = bin

SRC_DIR		=	src/
SRC_FILE	=	main.c	\
	 			uart.c	\
				i2c.c
SRC = $(addprefix $(SRC_DIR), $(SRC_FILE));
	  
INCLUDE = -I./inc
CFLAGS	=	-DF_CPU=$(F_CPU) -O2 -mmcu=$(MCU) $(INCLUDE)

HEX_NAME = $(addprefix $(addprefix $(DIR_BIN)/, $(NAME)), .hex)
BIN_NAME = $(addprefix $(addprefix $(DIR_BIN)/, $(NAME)), .bin)

OBJ := $(SRC:%.c=$(DIR_BIN)/%.o)

all: hex flash


$(DIR_BIN)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

hex: $(BIN_NAME)
# ihex format code hex
	$(COPY) -O ihex $^ $(HEX_NAME)

flash:
# avrdude man
	$(FLASHER) -p $(MCU) -P $(PORT) -b$(BAUD) -c$(PROG) -U flash:w:$(HEX_NAME)


clean:
	rm -rf $(DIR_BIN)

re: fclean	all

screen: all
	screen $(PORT) $(BAUD)

.PHONY: all hex flash clean fclean

