# Распознавание отпечатка пальца

## Запуск
Для запуска предполагается использование Docker и команды `docker-compose up --build`. После этого на `localhost:8080` становится доступно HTTP API,
а на `localhost:7070` - веб-интерфейс взаимодействия.

Для загрузки датасета для сравнения извлеките архив с датасетом в папку `deploy/dataset` и запустите там python-скрипт `sender.py`

## HTTP API
- `POST /get_owner` - получение предполагаемого владельца отпечатка
  - Запрос - json-объект
  ```
  {
    "fingerprint_image": "<base64-encoded image>"
  }
  ```
  - Ответ - json-объект
  ```
  {
    "ID": 12,
    "Name": "309__M_Right_little_finger",
    "Minutias": [...]
  }
  ```
- `POST /add_fingerprint` - добавленеи отпечатка в базу
  - Запрос - json-объект
  ```
  {
    "Name": "309__M_Right_little_finger",
    "fingerprint_image": "<base64-encoded image>"
  }
  ```
  - Ответ - json-объект
  ```
  {
    "ok": ture,
  }
  ```


