import { createSignal, createEffect } from "solid-js"

const API_URL = "http://0.0.0.0:8080"

function App() {
  const [name, setName] = createSignal("")
  const [file, setFile] = createSignal("")
  const [settingFile, setSettingFile] = createSignal(false)
  createEffect(async () => {
    if (!settingFile()) return
    let resp = await fetch(API_URL+"/get_owner",{
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({fingerprint_image: file()})
    }).then(resp => resp.json())
    setName(resp.Name)
    setSettingFile(false)
    setFile(null)
  })
  return (
    <div>
      <h1>Загрузка файла отпечатка</h1>
      <input id="uploadInput" type="file" multiple onInput={(e) => {
        let reader = new FileReader();
        reader.readAsDataURL(e.target.files[0])
        reader.onload = e => {
            setFile(e.target.result.split(',')[1])
        }
      }}/>
      <button onClick={(e) => setSettingFile(true)}>Загрузить</button>
      <h2>{name}</h2>
    </div>
  );
}

export default App;
