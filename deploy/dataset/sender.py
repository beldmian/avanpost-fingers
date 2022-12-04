import base64
import requests
import os

for file in os.listdir("./Real/"):
    with open("./Real/"+file, "rb") as image_file:
        encoded_string = base64.b64encode(image_file.read())
        print(file.split(".")[0])
        resp = requests.post("http://localhost:8080/add_fingerprint", json={
            "name": file.split(".")[0],
            "fingerprint_image": encoded_string.decode()
            })
