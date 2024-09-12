const express = require('express');
const { MongoClient } = require('mongodb');
const app = express();
const port = 3000;

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// MongoDB connection string (replace with your actual connection string)
const uri = 'mongodb://localhost:27017/your_database';

app.post('/submitForm', async (req, res) => {
  try {
    const client = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });
    await client.connect();

    const database = client.db('your_database');
    const collection = database.collection('user_information');

    // Extract form data
    const userData = {
      houseNumber: req.body.houseNumber,
      address1: req.body.address1,
      address2: req.body.address2,
      landmark: req.body.landmark,
      phoneNumber: req.body.phoneNumber,
      email: req.body.email,
      newPassword: req.body.newPassword,
    };

    // Insert data into the collection
    await collection.insertOne(userData);

    res.status(201).send('Data added to the database');
  } catch (error) {
    console.error('Error:', error);
    res.status(500).send('Internal Server Error');
  } finally {
    client.close();
  }
});

app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});
