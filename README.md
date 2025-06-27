# Servidor_ESP32_Encendido_leds_remoto
 Proyecto de servidor web para el encendido de leds usando esp32 c3 mini. En este proyecto tenemos los códigos para el servidor web del esp32 que permite el encendido y apagado de leds de forma inalámbrica y el envío del status del led de forma remota a un dashbaord de python anywhere.

 El circuito usado es 
 - 
 ![imagen](https://github.com/user-attachments/assets/fea6dd00-2568-45e3-9b3d-257bab78ae82)

 Ejemplo de los eventos registrados
 
<img src="https://github.com/raymundosoto/Servidor_ESP32_Encendido_leds_remoto/blob/main/image.png" width="500 height= 400">

- Para probar la publicación de status usa en Powershell de windows cambiando el device_id
```
 curl.exe -X POST "raymundoss.pythonanywhere.com/update_status" `
     -H "Content-Type: application/json" `
     -d '{\"device_id\": \"Raymundo\", \"led1_status\": true, \"led2_status\": false}'
```

