import tkinter as tk
import serial
from tkinter import font as tkFont
import serial.tools.list_ports
from tkinter import ttk

class ArduinoController:
    def __init__(self):
        self.arduino = self.conectar_arduino()
        self.sensor_states = {
            "puerta": True,
            "poste": True,
            "sala": True,
            "cocina": True
        }
        self.light_states = {
            "puerta": False,
            "poste": False,
            "sala": False,
            "cocina": False
        }
    
    def conectar_arduino(self):
        try:
            puertos = serial.tools.list_ports.comports()
            for puerto in puertos:
                if "Arduino" in puerto.description or "CH340" in puerto.description:
                    arduino = serial.Serial(puerto.device, 9600, timeout=1)
                    print(f"✨ Conectado a {puerto.device}")
                    return arduino
            print("⚠ Modo simulación (Arduino no detectado)")
            return None
        except Exception as e:
            print(f"🔥 Error de conexión: {str(e)}")
            return None
    
    def enviar_comando(self, tipo, dispositivo, accion):
        comando = f"{tipo}_{dispositivo}:{accion}\n" if tipo == "luz" else f"{dispositivo}:{accion}\n"
        
        # Actualizar estados
        if tipo == "sensor":
            self.sensor_states[dispositivo] = (accion == "ON")
        else:
            self.light_states[dispositivo] = (accion == "ON")
        
        if self.arduino:
            try:
                self.arduino.write(comando.encode())
                print(f"📤 Enviado: {comando.strip()}")
            except Exception as e:
                print(f"⚠ Error al enviar: {str(e)}")
                self.arduino = self.conectar_arduino()
        else:
            print(f"🖥 Simulación: {comando.strip()}")

def crear_interfaz(controller):
    root = tk.Tk()
    root.title("🌟 Control de Sensores Premium")
    root.geometry("800x600")
    root.configure(bg="#2c3e50")
    
    # Estilo moderno
    style = ttk.Style()
    style.theme_use('clam')
    style.configure('TFrame', background='#2c3e50')
    style.configure('TLabel', background='#2c3e50', foreground='#ecf0f1', font=('Helvetica', 12))
    style.configure('Title.TLabel', font=('Helvetica', 16, 'bold'))
    style.configure('Sensor.TButton', font=('Helvetica', 10), padding=10)
    style.configure('Light.TButton', font=('Helvetica', 10), padding=8)
    style.map('On.TButton', background=[('active', '#27ae60'), ('!active', '#2ecc71')])
    style.map('Off.TButton', background=[('active', '#c0392b'), ('!active', '#e74c3c')])
    
    # Marco principal con efecto de tarjeta
    main_frame = ttk.Frame(root, style='TFrame', padding=(20, 20))
    main_frame.pack(expand=True, fill="both")
    
    # Título con icono
    ttk.Label(main_frame, text="⚡ Control de Sensores Inteligente", style='Title.TLabel').pack(pady=(0, 20))
    
    # Sección de Sensores (con pestañas)
    notebook = ttk.Notebook(main_frame)
    notebook.pack(fill="both", expand=True, padx=10, pady=10)
    
    # Pestaña de Sensores
    sensor_tab = ttk.Frame(notebook)
    notebook.add(sensor_tab, text="🧭 Control de Sensores")
    
    # Grid para sensores
    for i, (sensor, estado) in enumerate(controller.sensor_states.items()):
        frame = ttk.Frame(sensor_tab, style='TFrame')
        frame.grid(row=i, column=0, pady=10, padx=20, sticky="ew")
        
        icon = "🚪" if sensor == "puerta" else "💡" if sensor == "poste" else "🛋" if sensor == "sala" else "🍳"
        ttk.Label(frame, text=f"{icon} Sensor {sensor.capitalize()}", style='TLabel').pack(side="left")
        
        btn_frame = ttk.Frame(frame, style='TFrame')
        btn_frame.pack(side="right")
        
        btn_on = ttk.Button(btn_frame, text="ACTIVAR", style='On.TButton',
                           command=lambda s=sensor: controller.enviar_comando("sensor", s, "ON"))
        btn_on.pack(side="left", padx=5)
        
        btn_off = ttk.Button(btn_frame, text="DESACTIVAR", style='Off.TButton',
                            command=lambda s=sensor: controller.enviar_comando("sensor", s, "OFF"))
        btn_off.pack(side="left")
        
        # Estado inicial
        if estado:
            btn_on.state(['pressed', 'disabled'])
        else:
            btn_off.state(['pressed', 'disabled'])
    
    # Pestaña de Luces
    light_tab = ttk.Frame(notebook)
    notebook.add(light_tab, text="💡 Control de Luces")
    
    # Grid para luces
    light_icons = {"puerta": "🚪", "poste": "🪔", "sala": "🛋", "cocina": "🍳"}
    for i, (luz, estado) in enumerate(controller.light_states.items()):
        frame = ttk.Frame(light_tab, style='TFrame')
        frame.grid(row=i//2, column=i%2, pady=15, padx=20, sticky="nsew")
        
        icon = light_icons[luz]
        ttk.Label(frame, text=f"{icon} Luz {luz.capitalize()}", style='TLabel').pack()
        
        btn_frame = ttk.Frame(frame, style='TFrame')
        btn_frame.pack(pady=5)
        
        btn_on = ttk.Button(btn_frame, text="ENCENDER", style='On.TButton',
                           command=lambda l=luz: controller.enviar_comando("luz", l, "ON"))
        btn_on.pack(side="left", padx=5)
        
        btn_off = ttk.Button(btn_frame, text="APAGAR", style='Off.TButton',
                            command=lambda l=luz: controller.enviar_comando("luz", l, "OFF"))
        btn_off.pack(side="left")
        
        # Estado inicial
        if estado:
            btn_on.state(['pressed', 'disabled'])
        else:
            btn_off.state(['pressed', 'disabled'])
    
    # Panel de estado
    status_frame = ttk.Frame(main_frame, style='TFrame')
    status_frame.pack(fill="x", pady=(15, 0))
    
    ttk.Label(status_frame, text="🔄 Estado: ", style='TLabel').pack(side="left")
    connection_status = ttk.Label(status_frame, 
                                text="Conectado" if controller.arduino else "Modo simulación", 
                                style='TLabel')
    connection_status.pack(side="left")
    
    # Efectos hover
    def on_enter(e):
        e.widget['background'] = '#3498db'
    
    def on_leave(e):
        e.widget['background'] = '#2980b9'
    
    # Botón de actualización con efecto
    refresh_btn = tk.Button(status_frame, text="🔄 Actualizar", bg="#2980b9", fg="white",
                           relief="flat", bd=0, padx=15, pady=5,
                           command=lambda: actualizar_estados(controller, root))
    refresh_btn.pack(side="right")
    refresh_btn.bind("<Enter>", on_enter)
    refresh_btn.bind("<Leave>", on_leave)
    
    return root

def actualizar_estados(controller, window):
    print("♻ Actualizando estados...")
    window.destroy()
    crear_interfaz(controller).mainloop()

if __name__ == "__main__":
    print("🚀 Iniciando interfaz de control...")
    controller = ArduinoController()
    app = crear_interfaz(controller)
    app.mainloop()