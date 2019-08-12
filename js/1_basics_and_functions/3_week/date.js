function render(data) {
    return Number(data);
}

function add(hour, property) {

}

/**
 * @param {String} date
 * @returns {Object}
 */
module.exports = function (date) {
    myObj = {
        _value: -1,
        set value(v) {
            this._value = new Date(v);
        },
        get value() {
            var curMonth = this._value.getMonth() + 1;
            var curDay = this._value.getDate();
            curMonth = curMonth < 10 ? '0' + curMonth : curMonth;
            curDay = curDay < 10 ? '0' + curDay : curDay;
            var leftPart = [
                this._value.getFullYear(),
                curMonth,
                curDay
            ].join('-');
            var rightPart = [
                (this._value.getHours() < 10
                    ? '0'
                    : '') + this._value.getHours(),
                (this._value.getMinutes() < 10
                    ? '0'
                    : '') + this._value.getMinutes()
            ].join(':');
            return leftPart + ' ' + rightPart;
        },
        dict: {
            'years': 0,
            'months': 0,
            'days': 0,
            'hours': 0,
            'minutes': 0
        },
        add: function (value, property) {
            if (value < 0 || !this.dict.hasOwnProperty(property)) {
                throw new TypeError;
            }
            switch (property) {
                case 'hours':
                    this._value.setHours(
                        this._value.getHours() + value
                    );
                    break;
                case 'months':
                    this._value.setMonth(
                        this._value.getMonth() + value
                    );
                    break;
                case 'days':
                    this._value.setDate(
                        this._value.getDate() + value
                    );
                    break;
                case 'minutes':
                    this._value.setMinutes(
                        this._value.getMinutes() + value
                    );
                    break;
                case 'years':
                    this._value.setFullYear(
                        this._value.getFullYear() + value
                    );
                    break;
            }
            return this;
        },
        subtract: function (value, property) {
            if (value < 0 || !this.dict.hasOwnProperty(property)) {
                throw new TypeError;
            }
            switch (property) {
                case 'hours':
                    this._value.setHours(
                        this._value.getHours() - value
                    );
                    break;
                case 'months':
                    this._value.setMonth(
                        this._value.getMonth() - value
                    );
                    break;
                case 'days':
                    this._value.setDate(
                        this._value.getDate() - value
                    );
                    break;
                case 'minutes':
                    this._value.setMinutes(
                        this._value.getMinutes() - value
                    );
                    break;
                case 'years':
                    this._value.setFullYear(
                        this._value.getFullYear() - value
                    );
                    break;
            }
            return this;
        }
    };
    myObj.value = date;
    return myObj;
};