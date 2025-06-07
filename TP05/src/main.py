import time
import threading
import math
import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

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

        self.update_thread = threading.Thread(target=self.update_loop, daemon=True)
        self.update_thread.start()

    def create_widgets(self):
        self.main_frame = ttk.Frame(self.root)
        self.main_frame.grid(row=0, column=0, padx=20, pady=20)
        self.root.title("Signal Selector")

        self.label = ttk.Label(self.main_frame, text="Select a signal to plot:")
        self.label.grid(row=0, column=0, columnspan=2, pady=(0, 10))

        self.signal1_button = ttk.Button(self.main_frame, text="Signal 1", command=self.select_signal1)
        self.signal1_button.grid(row=1, column=0, padx=10)

        self.signal2_button = ttk.Button(self.main_frame, text="Signal 2", command=self.select_signal2)
        self.signal2_button.grid(row=1, column=1, padx=10)

        self.status_label = ttk.Label(self.main_frame, text="No signal selected.")
        self.status_label.grid(row=2, column=0, columnspan=2, pady=(10, 0))

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
        self.start_signal(1)

    def select_signal2(self):
        self.start_signal(2)

    def start_signal(self, signal_id):
        self.active_signal = signal_id
        self.signal_data.clear()
        self.time_data.clear()
        self.start_time = time.time()
        self.status_label.config(text=f"Signal {signal_id} selected and started.")

    def update_loop(self):
        while self.running:
            if not self.paused:
                t = time.time() - self.start_time
                self.time_data.append(t)

                if self.active_signal == 1:
                    val = math.sin(2 * math.pi * 1 * t)
                else:
                    val = math.cos(2 * math.pi * 1 * t)

                self.signal_data.append(val)

                max_len = 500
                if len(self.time_data) > max_len:
                    self.time_data = self.time_data[-max_len:]
                    self.signal_data = self.signal_data[-max_len:]

                self.update_plot()

            time.sleep(self.update_interval)

    def update_plot(self):
        self.line.set_data(self.time_data, self.signal_data)
        if self.time_data:
            self.ax.set_xlim(max(0, self.time_data[-1] - 5), self.time_data[-1])
        self.ax.set_ylim(-1.5, 1.5)
        self.canvas.draw()

if __name__ == "__main__":
    root = tk.Tk()
    app = signalsApp(root)
    root.mainloop()