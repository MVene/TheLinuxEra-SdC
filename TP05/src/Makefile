# Nombre del módulo del kernel (sin la extensión .c)
obj-m := tp_sensor_driver.o

# Ruta a los encabezados del kernel.
# KDIR ?= /lib/modules/$(shell uname -r)/build
# La línea anterior funciona bien en la mayoría de los sistemas,
# pero si tienes problemas con la Raspberry Pi, a veces es más robusto
# apuntar directamente a los encabezados de Raspberry Pi OS.
# Puedes probar con la primera línea comentada primero, y si hay problemas,
# intentar con la segunda.
# Para Raspberry Pi, a veces es:
# KDIR ?= /usr/src/linux-headers-$(shell uname -r)
KDIR ?= /lib/modules/$(shell uname -r)/build

# Directorio actual
PWD := $(shell pwd)

# Regla 'all' es la predeterminada al ejecutar 'make'
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Regla 'clean' para limpiar los archivos generados
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean