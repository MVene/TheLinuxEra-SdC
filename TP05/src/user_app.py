import os
import fcntl
import time
import matplotlib
matplotlib.use('TkAgg') # O 'Qt5Agg' si TkAgg no funciona
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import struct

# Constantes de ioctl
TP_IOCTL_MAGIC_CHAR = 'k'
TP_IOCTL_MAGIC_VAL = ord(TP_IOCTL_MAGIC_CHAR)
TP_SELECT_SIGNAL1 = (TP_IOCTL_MAGIC_VAL << 8) | 0
TP_SELECT_SIGNAL2 = (TP_IOCTL_MAGIC_VAL << 8) | 1

# Archivo del driver
DEVICE_PATH = "/dev/tp_driver"

class SignalReader:
    def __init__(self, device_path):
        # Abre en modo lectura/escritura para asegurar que ioctl funciona bien
        self.device = os.open(device_path, os.O_RDWR)
        self.current_signal_num = 1
        self.time_data = []
        self.signal_data = []
        self.start_time = time.time()

    def select_signal(self, signal_num):
        if signal_num == 1:
            fcntl.ioctl(self.device, TP_SELECT_SIGNAL1)
            self.current_signal_num = 1
        elif signal_num == 2:
            fcntl.ioctl(self.device, TP_SELECT_SIGNAL2)
            self.current_signal_num = 2
        else:
            raise ValueError("Signal number must be 1 or 2")
        self.time_data = []
        self.signal_data = []
        self.start_time = time.time()
        print(f"Signal {signal_num} selected via ioctl. Next read will be from this signal.")

    def read_signal(self):
        # ¡¡¡ELIMINAR ESTA LÍNEA!!!
        # os.lseek(self.device, 0, os.SEEK_SET) 

        value_bytes = os.read(self.device, 1)
        if not value_bytes:
            # Si el driver ya no tiene datos para enviar (EOF), read devuelve un byte vacío
            # Esto ocurrirá si my_read en el driver devuelve 0.
            # Como tu driver ahora siempre devuelve 1 byte si *off == 0 (gracias al ioctl que reinicia f->f_pos),
            # esto no debería ser un problema con una sola lectura de 1 byte.
            # Sin embargo, si la aplicación intentara leer más de 1 byte sin que el driver lo soporte,
            # o si el driver leyera más de 1 byte de golpe.
            return None 
        
        try:
            return int(value_bytes.decode('ascii'))
        except ValueError:
            print(f"Warning: Could not convert '{value_bytes.decode('ascii')}' to int. Driver might be sending unexpected data.")
            return None

# ... (resto del código de graficación, no cambia) ...

# Inicialización
reader = SignalReader(DEVICE_PATH)

# Modificar el setup del gráfico para que el título refleje la señal actual
fig, ax = plt.subplots()
line, = ax.plot([], [], 'b.-', label="Señal")
ax.set_ylim(-0.2, 1.2)
ax.set_xlim(0, 10)
ax.set_xlabel("Tiempo (s)")
ax.set_ylabel("Valor de la señal")
ax.set_title(f"Lectura de Señal {reader.current_signal_num}")
ax.legend()

signal_toggle_interval = 10 # segundos
last_toggle_time = time.time()

def update(frame):
    global last_toggle_time

    current_elapsed_time = time.time() - reader.start_time
    if time.time() - last_toggle_time >= signal_toggle_interval:
        if reader.current_signal_num == 1:
            print("\n*** Cambiando a Señal 2 ***")
            reader.select_signal(2)
            ax.set_title("Lectura de Señal 2")
            line.set_label("Señal 2")
        else:
            print("\n*** Cambiando a Señal 1 ***")
            reader.select_signal(1)
            ax.set_title("Lectura de Señal 1")
            line.set_label("Señal 1")
        ax.legend()
        last_toggle_time = time.time()

    signal_value = reader.read_signal()
    if signal_value is not None:
        reader.time_data.append(current_elapsed_time)
        reader.signal_data.append(signal_value)

    display_window = 10 # segundos
    if reader.time_data and reader.time_data[-1] > display_window:
        min_time_visible = reader.time_data[-1] - display_window
        # Elimina datos más antiguos que la ventana de visualización
        i = 0
        while i < len(reader.time_data) and reader.time_data[i] < min_time_visible:
            i += 1
        reader.time_data = reader.time_data[i:]
        reader.signal_data = reader.signal_data[i:]
    
    if reader.time_data:
        ax.set_xlim(reader.time_data[0], reader.time_data[-1] + 1)
    else:
        ax.set_xlim(0, display_window)

    line.set_data(reader.time_data, reader.signal_data)
    return line,

# Inicialmente, selecciona la señal 1
reader.select_signal(1)

ani = animation.FuncAnimation(fig, update, interval=1000, blit=False, cache_frame_data=False)
plt.show()