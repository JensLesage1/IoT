document.addEventListener("DOMContentLoaded", function () {
  const inCountSpan = document.getElementById("inCount");
  const outCountSpan = document.getElementById("outCount");
  const currentCountSpan = document.getElementById("currentCount");

  // Function to update counts
  function updateCounts(message) {
    const counts = message.split(", ");
    const inCount = parseInt(counts[0].split(": ")[1]);
    const outCount = parseInt(counts[1].split(": ")[1]);
    const currentCount = parseInt(counts[2].split(": ")[1]);

    inCountSpan.textContent = inCount;
    outCountSpan.textContent = outCount;
    currentCountSpan.textContent = currentCount;
  }

  // Fetch initial counts from the server
  fetch("/api/data")
    .then((response) => response.json())
    .then((data) => {
      updateCounts(
        `people_entered: ${data.people_entered}, people_left: ${data.people_left}, current_count: ${data.current_count}`
      );
    })
    .catch((error) => console.error("Error fetching initial data:", error));

  // MQTT setup
  const host = window.location.hostname; // Raspberry Pi IP address
  const port = 9001; // MQTT Websockets port
  const client = mqtt.connect(`mqtt:${host}:${port}`);

  client.on("connect", () => {
    client.subscribe("arduino/jannesiscool", (err) => {
      if (!err) {
        console.log("Subscribed to topic");
      } else {
        console.error("Failed to subscribe to topic:", err);
      }
    });
  });

  client.on("message", (topic, message) => {
    updateCounts(message.toString());
  });
});
