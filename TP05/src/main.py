import os
import time
import threading
import queue
import math  # Needed for signal generation like sin, cos
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

# Archivo del driver
DEVICE_PATH = "/dev/tp_driver"

class signalsApp:
    def __init__(self, root):
        self.root = root
        self.running = True  
        self.paused = False  
        self.signal_data = []  
        self.time_data = []  
        self.start_time = time.time()  

        self.active_signal = 1
        self.update_interval = 0.02  
        
        self.create_widgets()  

        # self.update_thread = threading.Thread(target=self.update_loop, daemon=True)
        # self.update_thread.start()

    def create_widgets(self):

         # Main frame
        self.main_frame = ttk.Frame(root)
        self.main_frame.grid(row=0, column=0, padx=20, pady=20)
        self.main_frame.grid(row=0, column=0)
        self.root.title("Signal Selector")
        # Label
        self.label = ttk.Label(self.main_frame, text="Select a signal to plot:")
        self.label.grid(row=0, column=0, columnspan=2, pady=(0, 10))

        # Signal Buttons||
        self.signal1_button = ttk.Button(self.main_frame, text="Signal 1", command=self.select_signal1)
        self.signal1_button.grid(row=1, column=0, padx=10)

        self.signal2_button = ttk.Button(self.main_frame, text="Signal 2", command=self.select_signal2)
        self.signal2_button.grid(row=1, column=1, padx=10)

        # Status label
        self.status_label = ttk.Label(self.main_frame, text="No signal selected.")
        self.status_label.grid(row=2, column=0, columnspan=2, pady=(10, 0))

        # Matplotlib Figure setup
        self.fig = Figure(figsize=(5, 3), dpi=100)
        self.ax = self.fig.add_subplot(111)
        self.ax.set_title("Live Signal")
        self.ax.set_xlabel("Time (s)")
        self.ax.set_ylabel("Amplitude")

        self.line, = self.ax.plot([], [], lw=2)

        self.canvas = FigureCanvasTkAgg(self.fig, master=self.main_frame)
        self.canvas.draw()
        self.canvas.get_tk_widget().grid(row=3, column=0, columnspan=2, pady=10)
        

    def select_signal1(self):
        self.status_label.config(text="Signal 1 selected.")
        self.start_signal()

    def select_signal2(self):
        self.status_label.config(text="Signal 2 selected.")
        self.start_signal()

    def start_signal(self, signal_id):
        self.signal_data.clear()
        self.time_data.clear()
        self.start_time = time.time()
        self.status_label.config(text=f"Signal {signal_id} selected and started.")

    def update_loop(self):
        while self.running:
            if not self.paused:
                t = time.time() - self.start_time
                self.time_data.append(t)

                # Generate signal based on selected type
                L_sim = 10/self.update_interval

                if len(self.time_data) > L_sim:
                    self.signal_data.append[-L_sim:]

                self.update_plot()  # Refresh the figure
            time.sleep(self.update_interval)

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

    def update_plot(self):
        self.line.set_data(self.time_data, self.signal_data)

        # Limit x-axis to last 5 seconds
        if self.time_data:
            self.ax.set_xlim(max(0, self.time_data[-1] - 5), self.time_data[-1])
        self.ax.set_ylim(-1.5, 1.5)

        self.canvas.draw()

if __name__ == "__main__":
    root = tk.Tk()
    app = signalsApp(root)
    # root.protocol("WM_DELETE_WINDOW", app.stop)
    root.mainloop()