from flask import Flask, request, jsonify, render_template
import paho.mqtt.client as mqtt

app = Flask(__name__)

# MQTT settings
mqtt_server = "127.0.0.1"
mqtt_topic = "arduino/jannesiscool"

# Global variables to store counts
people_entered = 0
people_left = 0
current_count = 0

# MQTT callback functions
def on_connect(client, userdata, flags, rc, properties):
    print("Connected with result code " + str(rc))
    client.subscribe(mqtt_topic)

def on_message(client, userdata, msg):
    global people_entered, people_left, current_count
    message = msg.payload.decode()
    # Assuming message format: "people_entered: 5, people_left: 3, current_count: 2"
    data = dict(item.split(": ") for item in message.split(", "))
    people_entered = int(data.get('people_entered', 0))
    people_left = int(data.get('people_left', 0))
    current_count = int(data.get('current_count', 0))
    print(f"people_entered: {people_entered}, people_left: {people_left}, current_count: {current_count}")

@app.route('/api/data', methods=['GET', 'POST'])
def receive_data():
    global people_entered, people_left, current_count

    if request.method == 'POST':
        people_entered = int(request.form.get('people_entered', 0))
        people_left = int(request.form.get('people_left', 0))
        current_count = int(request.form.get('current_count', 0))
        print(f"people_entered: {people_entered}, people_left: {people_left}, current_count: {current_count}")
        return jsonify({"status": "success"}), 200

    if request.method == 'GET':
        return jsonify({
            "people_entered": people_entered,
            "people_left": people_left,
            "current_count": current_count
        }), 200

@app.route("/")
def index():
    return render_template("index.html", people_entered=people_entered, people_left=people_left, current_count=current_count)

client = mqtt.Client(transport='websockets')
client.on_connect = on_connect
client.on_message = on_message

client.connect(mqtt_server, 9001, 60)

if __name__ == "__main__":
    client.loop_start()
    app.run(host="0.0.0.0", port=5678, debug=True)
