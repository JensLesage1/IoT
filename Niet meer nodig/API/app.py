from flask import Flask, request, jsonify

app = Flask(__name__)

# Simuleer een database voor opslag van gegevens
personen = []

@app.route('/personen', methods=['POST'])
def add_persoon():
    data = request.get_json()
    personen.append(data)
    return "Persoon toegevoegd!", 201

@app.route('/personen', methods=['GET'])
def get_personen():
    return jsonify(personen)

# Voeg andere routes toe voor het bijwerken en verwijderen van gegevens

if __name__ == '__main__':
    app.run(debug=True)
