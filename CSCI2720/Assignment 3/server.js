// Name: Lai Man Hin    SID: 1155136167
const express = require('express');
const app = express();
var mongoose = require('mongoose');
let serverURL = "mongodb+srv://stu112:p814727-@csci2720.m2qbq.mongodb.net/stu112"
mongoose.connect(serverURL);
const db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error:'));
// success
db.once('open', function () {
    console.log("Connection is open...");
    /* further actions which depends on db... */
    });
// Defining Event and Location schema
const Schema = mongoose.Schema;
const EventSchema = mongoose.Schema({
    eventId: { type: Number, required: true, unique: true},
    name: { type: String, required: true },
    // loc is a objectID, referenced to Location
    loc: { type: Schema.Types.ObjectId, ref: 'Location' },
    quota: { type: Number }
});
const LocationSchema = mongoose.Schema({
    locId: { type: Number, required: true, unique: true},
    name: { type: String, required: true },
    quota: { type: Number }
});
const Event = mongoose.model('Event', EventSchema);
const Location = mongoose.model('Location', LocationSchema);
// Use the cors middleware for Express to enable submission from local HTML forms.
const cors = require('cors'); app.use(cors());

// Q1 here
/* 
When the server receives this request, look up the event with the
given event ID from the database. Then print the event name,
location ID, location name, event quota on separate lines, as shown in
the illustration on the right. The location quota is not necessary.
All field names and string values should be quoted with double
quotes, like JSON. Extra spacing is allowed, and brackets/commas do no need to be in separate lines.
If the given event ID is not found, output an understandable message in the response body with
status code 404. All the response should be sent as HTTP responses with content type text/plain.
*/
app.get('/event/:eventId', (req,res) => {
    const result1 = Event.find(
    {eventId: req.params['eventId']}).populate('loc');

    //result1.select('eventId name loc');

    result1.exec((err, e) => {
    if (err)
    {   
        res.send(err);
    }
    else {
        if (e.length != 0)
        {
            res.set('Content-Type', 'text/plain');
            let mymessage = '{\n"eventId": ' + e[0].eventId + ',\n"name": "' + e[0].name + '",\n"loc":\n{\n"locId": ' + e[0].loc.locId + ',\n"name": "' + e[0].loc.name + '"\n},\n"quota": ' + e[0].quota + '\n}';
            res.send(mymessage);
        }
        else
        {
            res.set('Content-Type', 'text/plain');
            res.status(404);
            res.send("Cannot find the corresponding event");
        }
    }
    });
    
});

// Q2 here
/*
When the server receives this request, it should use the parameters submitted in the HTTP request
body to create a new event in the database. A simple HTML form like this can be used:
http://www.cse.cuhk.edu.hk/~chuckjee/2720lab8/form.html
However, instead of letting the user decide the event ID, your code should look into the database to
find the current maximum event ID, e.g. x. Assign x+1 as the new event ID. The form should ask the
user for the location ID only but not the location name. A lookup should be done to check if the
location quota is larger than or equal to the new event quota. If not, the event should not be created,
and an error message should be responded to the user. No special error status code is needed.
If the event is created successfully, respond to the user with the event details as in the GET response.
Add a header field Location containing the URL of the created event. Use HTTP status code 201.
*/

// This module is for parsing the content in a request body (installed with npm)
const bodyParser = require('body-parser');
// Use parser to obtain the content in the body of a request
app.use(bodyParser.urlencoded({extended: false}));

app.post('/event', (req, res) => {
    // Your Q2 code here
    let name = req.body['eventName'], eventLoc = parseInt(req.body['eventLoc']), eventQuota = parseInt(req.body['eventQuota']);
    let locIDSearch = Location.findOne({locId: eventLoc}), locID;
    locIDSearch.exec((err, e) => {
        if (err) {
            res.send(err);
        }
        else
        {
            locID = e._id;
            if (!isNaN(e.quota) && !isNaN(eventQuota) && e.quota < eventQuota)
            {
                res.set('Content-Type', 'text/plain');
                res.send("The location do not have so much quota!");
            }
            else
            {   
                // We start the find maxNum x, we start the x to be 1 initially
                let maxNum = 0;
                Event.findOne({}).sort({eventId: -1}).exec((err, e) => {
                    if (err) {
                        res.send(err);
                    }
                    else
                    {
                        if (e)
                            maxNum = e.eventId;
                        maxNum += 1;
                        Event.create({
                            eventId: maxNum,
                            name: name,
                            loc: locID,
                            quota: isNaN(eventQuota) ? null : eventQuota
                        }, (err, e) => {
                            if (err)
                            {
                                res.set('Content-Type', 'text/plain');
                                res.send(err);
                            }
                            else
                            {
                                res.set('Content-Type', 'text/plain');
                                res.status(201);
                                res.send(e);
                            }
                        });
                    }
                });
            }
        }
    });
    
});

// Q3 here
/*
If the event ID is found, the event should be removed from the database. Send a response with
status code 204, and nothing in the body. If not found, show an error message in the body with
status code 404.

You may utilize the software Postman (https://www.postman.com) for making DELETE requests to
your Express server during your development. Of course, you may also build user pages to send it
using JavaScript. This JS is not part of the assignment submission.
*/
app.delete('/event/:eventID', (req, res) => {
    // Your Q3 code here
    Event.find({ eventId: req.params['eventID']}).remove((err, e) => {
        if (err) {
            res.send(err);
        }
        else {
            if (e.deletedCount == 0)
            {
                res.set('Content-Type', 'text/plain');
                res.status(404);
                res.send("Corresponding Event not found");
            }
            else
            {
                res.status(204);
                res.send();
            }
        }});
});

// Q4 here
/*
List all the events available, with details formatted similar to the first
illustration on the right
*/
app.get('/event', (req, res) => {
    //res.send(Event);
    const result = Event.find(
        {}
    ).populate('loc');
    result.exec((err, e) => {
        if (err)
            res.send(err);
        else {
            res.set('Content-Type', 'text/plain');
            let mymessage = '[';
            e.forEach ((e) => {
                mymessage += '\n{\n"eventId": ' + e.eventId + ',\n"name": "' + e.name + '",\n"loc":\n{\n"locId": ' + e.loc.locId + ',\n"name": "' + e.loc.name + '"\n},\n"quota": ' + e.quota + '\n}\n,';
            });
            mymessage = mymessage.slice(0, -1);
            if (mymessage == '')
                mymessage = '[ ]';
            else
                mymessage += ']';
            res.send(mymessage);
        }
    });
});

// Q5 here
/*
Show the details for this location ID, with details formatted similar to
the second illustration on the right. Respond with an error message if
the location ID is not found with status code 404
*/
app.get('/loc/:locID', (req, res) => {
    // Your Q5 code here
    const result = Location.findOne(
      {locId: parseInt(req.params['locID'])}  
    );
    result.exec((err, e) => {
        if (err)
            res.send(err);
        else {
            if (e) {
                res.set('Content-Type', 'text/plain');
                mymessage = '{\n"locId": ' + e.locId + ',\n"name": "' + e.name + '",\n"quota": ' + e.quota + '\n}';
                res.send(mymessage);
            }
            else {
                res.set('Content-Type', 'text/plain');
                res.status(404);
                mymessage = 'error: locId not found';
                res.send(mymessage);
            }
        }
    });
});

// Q6, Q7 here
/*
List all locations available, with details formatted similarly to multiple events.
*/
app.get('/loc', (req, res) => {
    // Your Q6, Q7 code here
    let number = 0;
    if (req.query['quota'])
        number = parseInt(req.query['quota']);
    const result = Location.find(
        {quota: { $gte: number}}  
      );
      result.exec((err, e) => {
          if (err)
              res.send(err);
          else {
              if (e) {
                  res.set('Content-Type', 'text/plain');
                  let mymessage = '['
                  e.forEach((e) => {
                    mymessage += '\n{\n"locId": ' + e.locId + ',\n"name": "' + e.name + '",\n"quota": ' + e.quota + '\n}\n,';
                  });
                  mymessage = mymessage.slice(0, -1);
                  if (mymessage == '')
                    mymessage = '[ ]';
                  else
                    mymessage += ']';
                  res.send(mymessage);
              }
              else {
                  res.set('Content-Type', 'text/plain');
                  res.status(404);
                  mymessage = 'error: locId not found';
                  res.send(mymessage);
              }
          }
      });
});

// Q7 above, combined with Q6
/*
List all the locations with quota of at least this number, with details formatted similarly to multiple
events. Respond with an empty array [ ] if there is none.
*/

// Q8 here
/*
If the event is updated successfully, respond to the user with the event details as in the GET
response. You may combine this HTML form with the POST form, or use a new HTML file.
Checking of event and location quota is not necessary here.
*/
app.use(bodyParser.json());
app.put('/event/:eventID', (req, res) => {
    // Your Q8 code here
    let locIDSearch = Location.findOne({locId: parseInt(req.body['eventLoc'])}), locID;
    locIDSearch.exec((err, e) => {
        if (err) {
            res.send(err);
        }
        else
        {
            locID = e._id;
            Event.findOneAndUpdate({eventId: parseInt(req.body['eventId'])}, 
            {
                name: req.body['eventName'],
                loc: locID,
                quota: req.body['eventQuota'] == '' ? null : parseInt(req.body['eventQuota'])
            },
            (err, e) => {
                if (err)
                {
                    res.send(err);
                }
                else
                    res.send(e);
            })
        }});
});

// For creating new location 
/*
app.get('/newloc', (req, res) =>{
    Location.create({
        locId: 3,
        name: "C Land"
    }, (err, e) => {
        if (err) res.send(err);
        else res.send(e);
    });
});
*/

// For deleting location
/*
app.get('/locr', (req, res) => {
    Event.find({ locId: 3}).remove((err, e) => {
        if (err) {
            res.send(err);
        }
        else
            res.send(e);
    })
});
*/

const server = app.listen(3000);