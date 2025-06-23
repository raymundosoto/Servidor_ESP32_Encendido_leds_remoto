from flask import Flask, render_template, request, jsonify
from datetime import datetime
import json
import os

app = Flask(__name__)

# Archivo para almacenar los datos
DATA_FILE = 'led_status_data.json'

# Inicializar el archivo de datos si no existe
if not os.path.exists(DATA_FILE):
    with open(DATA_FILE, 'w') as f:
        json.dump([], f)

@app.route('/')
def dashboard():
    # Leer los datos almacenados
    with open(DATA_FILE, 'r') as f:
        data = json.load(f)
    
    # Obtener solo los últimos 10 registros para mostrar
    recent_data = data[-10:] if len(data) > 10 else data
    recent_data.reverse()  # Mostrar los más recientes primero
    
    return render_template('index.html', data=recent_data)

@app.route('/update_status', methods=['POST'])
def update_status():
    # Obtener los datos JSON de la solicitud
    request_data = request.get_json()
    
    # Validar los datos recibidos
    if not all(key in request_data for key in ['device_id', 'led1_status', 'led2_status']):
        return jsonify({'status': 'error', 'message': 'Datos incompletos'}), 400
    
    # Agregar marca de tiempo
    request_data['timestamp'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    
    # Leer los datos existentes
    with open(DATA_FILE, 'r') as f:
        data = json.load(f)
    
    # Agregar los nuevos datos
    data.append(request_data)
    
    # Guardar los datos actualizados
    with open(DATA_FILE, 'w') as f:
        json.dump(data, f, indent=2)
    
    return jsonify({'status': 'success'})

if __name__ == '__main__':
    app.run(debug=True)