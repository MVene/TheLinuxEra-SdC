obj-m := tp_sensor_driver.o

KDIR ?= /lib/modules/$(shell uname -r)/build

# Directorio actual
PWD := $(shell pwd)

# Regla 'all' es la predeterminada al ejecutar 'make'
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Regla 'clean' para limpiar los archivos generados
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean