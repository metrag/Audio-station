import wave
import io

SAMPLERATE=8_000 # Частота дискретизации, Гц
CHANNELS = 1 # Моно
SAMPLEWIDTH = 2 # 16 бит


test_data = [
    b'2226', b'1505', b'1367', b'1168', b'1617',
    b'1168', b'1250', b'1337', b'1431', b'1179',
    b'1521', b'1388', b'1201', b'1287', b'1390',
    b'1456', b'1022', b'0',    b'1962', b'2029',
    b'2928', b'2197', b'371',  b'2362', b'0',
    b'0',    b'1347', b'2927', b'2928', b'1763',
    b'2928', b'2926', b'2922', b'2919', b'1419',
    b'1375', b'1399', b'1393', b'1407', b'1377',
    b'1451', b'1392', b'1335', b'1196', b'1298']

audio_data = b""

for block in test_data:
    amplitude = int(str(block, encoding='utf-8'))
    audio_data += amplitude.to_bytes(2, byteorder='little', signed=True)


with wave.open('output.wav', mode='wb') as file:
    file.setframerate(SAMPLERATE)
    file.setnchannels(CHANNELS)
    file.setsampwidth(SAMPLEWIDTH)
    file.writeframes(audio_data)