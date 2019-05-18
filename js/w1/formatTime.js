var MINUTES_PER_HOUR = 60;
var HOURS_PER_DAY = 24;

/**
 * @param {Number} hours
 * @param {Number} minutes
 * @param {Number} interval
 * @returns {String}
 */
module.exports = function (hours, minutes, interval) {
    minutes += interval;
    hours += Math.floor(minutes / 60);

    minutes = minutes % MINUTES_PER_HOUR;
    hours = hours % HOURS_PER_DAY;

    minutes = minutes < 10 ? '0' + minutes : minutes;
    hours = hours < 10 ? '0' + hours : hours;

    return hours + ':' + minutes;
};
