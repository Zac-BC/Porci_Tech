import socket
import json

class ServidorSocket:
    DIRECCION = "192.168.100.128"
    PUERTO = 65440

    def __init__(self):
        print("Dentro del servidor")
        servidor = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        servidor.bind((self.DIRECCION, self.PUERTO))
        servidor.listen(0)
        print(f"Escuchando en la dirección {self.DIRECCION} : {self.PUERTO}")

        cliente_socket, cliente_direccion = servidor.accept()
        print(f"Aceptando conexión de : {cliente_direccion[0]}:{cliente_direccion[1]}")

        while True:
            request = cliente_socket.recv(45)
            request = request.decode("utf-8")

            if request.lower() == "cerrar":
                cliente_socket.send("cerrada".encode("utf-8"))
                break

            try:
                # Separar la información recibida en diferentes categorías
                data_dict = json.loads(request)
                temperatura = data_dict["Temperatura"]
                humedad = data_dict["Humedad"]
                ph = data_dict["PH"]

                # Imprimir los valores
                print(f"Temperatura: {temperatura}")
                print(f"Humedad: {humedad}")
                print(f"PH: {ph}")
            except json.JSONDecodeError as e:
                print(f"Error al decodificar JSON: {e}")

        cliente_socket.close()
        print("Conexión cerrada")

def main():
    servidor = ServidorSocket()

if __name__ == "__main__":
    main()