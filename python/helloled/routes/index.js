var express = require('express');
var router = express.Router();

var sw = 'off';
/* GET home page. */
router.route('/sensors')        // Browser Query
    .get( function(req, res, next) {
        sw = req.query.switch;
        console.log("Switch:" + sw );
        res.send({'switch' : sw});
    })
    ;
router.route('/sensors/v')      // Python query
    .get( function(req, res, next) {
           res.send({'switch' : sw});

    })
    ;
module.exports = router;
