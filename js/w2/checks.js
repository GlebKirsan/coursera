// Встроенный в Node.JS модуль для проверок
var assert = require('assert');

// Подключаем свою функцию
var phoneBook = require('./phoneBook.js');

phoneBook('ADD Ivan 555,666');
phoneBook('ADD Ivan 777,888');
console.log(phoneBook('SHOW'));