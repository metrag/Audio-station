import os
import socket
import wave
from datetime import datetime

SAMPLERATE=8000 #частота дискретизации
CHANNELS = 1 #моно режим
SAMPLEWIDTH = 2 #16 бит - одна амплитуда

#запуск сервера
HOST = '192.168.88.244'
PORT = 8686

SERVER_ADDRESS = (HOST, PORT)

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(SERVER_ADDRESS)
server_socket.listen(1)

print('server is running...')

#приём данных
while True:

    print("find data...")
    audio_data = []
    work = False
    audio = b""
    c = 0 #количество служебных символов

    while True:
        connection, address = server_socket.accept()
        data = connection.recv(1024)

        #отлов служебных символов и запись данных
        if data == b'#':
            c += 1
            # work = True
            print("________")
            continue
        if c == 1:
            audio_data.append(data)
        elif c == 2:
            break
        
    connection.close()

    print("\nmake waw file...")

    #собираем байтовую строку для записи
    for a in audio_data:
        amplitude = int(str(a, encoding='utf-8'))
        audio += amplitude.to_bytes(2, byteorder='little', signed=True)

    #запись файла (по времени)
    with wave.open(f'{datetime.now().strftime("%Y-%m-%d_%H-%M-%S")}.wav', mode='wb') as file:
        file.setframerate(SAMPLERATE) #установка частоты
        file.setnchannels(CHANNELS)
        file.setsampwidth(SAMPLEWIDTH)
        file.writeframes(audio)

    print("ok!")
    
    #запись команды
    com = ' '
    while com not in ['n','q']:
        com = input("\n\n\nn - new data\nq - exit\ninput: ")
        if com == 'n':
            os.system("clear")
        elif com == 'q':
            exit()