var mongoose = require('mongoose');	
// connect to db
mongoose.connect('mongodb://student:012345@192.168.0.149/student');

//Schemas
var Sensorcheme = new mongoose.Schema({
    name: String,
    type: String,
    status: String,
    switch: String,
    description: String
});

// Models
var SensorModel = mongoose.model( 'qkboo_sensor', Sensorcheme);

exports.getAllSensors = function(req, res) {
    console.log( 'GET /api/sensors' );

    return SensorModel.find( function( err, sensors ) {
        if( err ) {
            console.log( err );
            return res.send(err);
        }
        return res.send( sensors );
    });
};

// sensors/:id -  GET / id로 선택된 book 정보얻기
exports.getSensorById = function(req, res) {
    return SensorModel.findById( req.params.id, function(err, sensor) {
        if (err)
            res.send(err);
        res.send(sensor);
    });
};

// POST:insert
exports.addSensor = function(req, res) {

	console.log("Adding new sensor: " + req.body.name );

    var sensor = new SensorModel();
    sensor.type = req.body.type;
    sensor.name = req.body.name;
    sensor.status = req.body.status;
    sensor.description = req.body.description;
    sensor.switch = req.body.switch;
    return sensor.save( function( err ) {
        if( err ) {
            console.log( err );
            return res.send(err);
        }
    	return res.send( sensor);
    });
};

// PUT / id의 정보 갱신
exports.updateSensor = function(req, res) {
    console.log("Update a sensor: " + req.params.id);

    var conditions = { _id: req.params.id }
      , update = { $set : { switch: req.body.switch} };
    
    SensorModel.update(conditions, update, function(err, sensor) {
        if (err)
            return res.send(err);
        return res.json({ message: 'Sensor Switch updated ->' + sensor.switch });
    });
};

// DELETE
exports.deleteSensorById = function(req, res) {
	console.log("Deleting a sensor:" + req.params.id);
	return SensorModel.remove( {_id: req.params.id}, function( err, sensor ) {
        if( err ) {
            console.log( err );
            return res.send(err);
        } else {
            return res.json( {message:"ID("+req.params.id+") Successfully deleted!"} );
        }
    });
};
