// Телефонная книга
var phoneBook = {};

function addContact(nameAndPhone) {
    var name = nameAndPhone[0];
    var phones = nameAndPhone.slice(1)[0].split(',');
    if (!phoneBook.hasOwnProperty(name)) {
        phoneBook[name] = [];
    }
    phoneBook[name] = phoneBook[name].concat(phones);
}

function showBook() {
    var phoneList = [];
    for (var prop in phoneBook) {
        phoneList.push(
            [prop, phoneBook[prop].join(', ')].join(': ')
        );
    }
    return phoneList.sort();
}

function removePhone(phone) {
    var flag = false;
    for (var prop in phoneBook) {
        var pos = phoneBook[prop].indexOf(phone);
        if (pos > -1){
            phoneBook[prop].splice(pos, 1);
            flag = true;
            if (phoneBook[prop].length === 0){
                delete phoneBook[prop];
            }
        }
    }
    return flag;
}

/**
 * @param {String} command
 * @returns {*} - результат зависит от команды
 */
module.exports = function (command) {
    var words = command.split(' ');
    var commandName = words[0];
    switch (commandName) {
        case 'ADD':
            return addContact(words.slice(1));
        case 'REMOVE_PHONE':
            return removePhone(words[1]);
        case 'SHOW':
            return showBook();
    }
};
