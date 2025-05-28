const express = require("express");
const bodyParser = require("body-parser");
const admin = require("firebase-admin");

const app = express();
const port = 3000; 

const serviceAccount = require("./serviceAccountKey.json");

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://jiit-rfid-default-rtdb.firebaseio.com"
});

const database = admin.database(); 


app.use(bodyParser.json());

app.post("/addCard", async (req, res) => {
  const { uid } = req.body;

  if (!uid) {
    return res.status(400).json({ error: "UID is required" });
  }

  try {

    const ref = database.ref("card"); 

    await ref.set({ id: uid });

    res.status(200).json({ message: "Card ID saved successfully" });
  } catch (error) {
    console.error("Error inserting card UID into Firebase Realtime Database:", error);
    res.status(500).json({ error: "Failed to save data to Realtime Database" });
  }
});

app.listen(port, "0.0.0.0", () => {
  console.log(`Server running on http://0.0.0.0:${port}`);
});
