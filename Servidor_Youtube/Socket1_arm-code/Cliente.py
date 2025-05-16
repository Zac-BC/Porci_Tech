import socket
import threading

def recibir_mensajes(clientesocket):
    """Función para recibir mensajes sin bloquear el envío."""
    while True:
        try:
            response = clientesocket.recv(1024).decode("utf-8")
            if response:
                print(f"\nRecibido: {response}")
        except:
            print("Conexión cerrada.")
            clientesocket.close()
            break

# Configuración del cliente
clientesocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientesocket.connect(("192.168.137.70", 8089))

# Iniciar hilo para recibir mensajes sin bloquear el envío
threading.Thread(target=recibir_mensajes, args=(clientesocket,), daemon=True).start()

while True:
    texto = input("Escribe algo: ")
    clientesocket.send(texto.encode("utf-8"))