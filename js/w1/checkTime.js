/**
 * @param {Number} hours
 * @param {Number} minutes
 * @returns {Boolean}
 */
module.exports = function (hours, minutes) {
    var isHoursValid = hours < 24 && hours >= 0;
    var isMinutesValid = minutes < 60 && minutes >= 0;
    return isHoursValid && isMinutesValid;
};
