var express = require('express');
var router = express.Router();
var sensormodel = require('./sensormodel');

/* GET home page. */
router.route('/sensors')        // Browser Query
    .get( sensormodel.getAllSensors )
    .post( sensormodel.addSensor )
    ;

router.route('/sensors/:id')      // Python query
    .get( sensormodel.getSensorById)
    .put( sensormodel.updateSensor)
    .delete(sensormodel.deleteSensorById)
    // .get( function(req, res, next) {
    //        res.send(sensormodel.);

    // })
    ;

module.exports = router;
