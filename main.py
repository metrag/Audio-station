import io
import socket
import wave

SAMPLERATE=8000 #частота дискретизации
CHANNELS = 1 #моно режим
SAMPLEWIDTH = 2 #16 бит - одна амплитуда

#запуск сервера
HOST = '172.20.10.4' 
PORT = 8686

SERVER_ADDRESS = (HOST, PORT)

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(SERVER_ADDRESS)
server_socket.listen(1)

print('server is running...')

audio_data = []
c = 0 #количество служебных символов

#приём данных
while True:
    connection, address = server_socket.accept()
    data = connection.recv(1024)

    #вывод полученного сообщения
    print(data)

        
connection.close()
