import socket
import time
import threading

def daytime_server():
    # Создаем UDP сокет
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_address = ('localhost', 13)
    
    try:
        # Биндим сокет к порту 13
        sock.bind(server_address)
        print(f"Daytime сервер запущен на {server_address}")
        print("Ожидание запросов...")
        
        while True:
            # Ждем входящие данные
            data, address = sock.recvfrom(4096)
            print(f"Получен запрос от {address}")
            
            # Формируем ответ с текущим временем
            current_time = time.strftime('%d %b %Y %H:%M:%S %Z', time.localtime())
            response = f"{current_time}\r\n"
            
            # Отправляем ответ
            sock.sendto(response.encode(), address)
            print(f"Отправлен ответ: {response.strip()}")
            
    except Exception as e:
        print(f"Ошибка: {e}")
    finally:
        sock.close()

if __name__ == "__main__":
    daytime_server()