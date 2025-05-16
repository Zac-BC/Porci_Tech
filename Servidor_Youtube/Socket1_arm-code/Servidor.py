import socket
import threading

# Lista para almacenar conexiones de clientes
clients = []

def manejar_cliente(connection, address):
    """Función para manejar cada cliente en un hilo separado."""
    print(f"Cliente conectado desde {address}")
    while True:
        try:
            buf = connection.recv(1024)
            if not buf:
                break
            mensaje = buf.decode("utf-8")
            print(f"Recibido de {address}: {mensaje}")

            # Enviar el mensaje a todos los clientes conectados
            for client in clients:
                if client != connection:  # No enviarlo al remitente
                    client.send(buf)

        except:
            break

    print(f"Cliente {address} desconectado.")
    clients.remove(connection)
    connection.close()

# Configuración del servidor
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind(("192.168.137.70", 8089))
serversocket.listen(5)

print("Servidor iniciado...")

while True:
    connection, address = serversocket.accept()
    clients.append(connection)
    
    # Crear un nuevo hilo para manejar al cliente
    hilo_cliente = threading.Thread(target=manejar_cliente, args=(connection, address))
    hilo_cliente.start()