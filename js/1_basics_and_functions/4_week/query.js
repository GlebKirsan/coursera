/**
 * @param {Array} collection
 * @params {Function[]} – Функции для запроса
 * @returns {Array}
 */
function query(collection) {
    var collectionCopy = JSON.parse(JSON.stringify(collection));
    if (arguments.length === 1) {
        return collectionCopy;
    }
    var arrArguments = [].slice.call(arguments).slice(1);

    var toSelect = arrArguments.filter(selectionFilter);
    toSelect = toSelect.map(removeLast);

    var toFilter = arrArguments.filter(filtrationFilter);
    toFilter = toFilter.map(removeLast);

    var selectIntersection = leaveIntersection(toSelect);
    var filterIntersection = intersectFiltration(toFilter);

    finalFilter(collectionCopy, filterIntersection)
    if (selectIntersection.length !== 0) {
        collectionCopy = leaveNeededProperties(collectionCopy, selectIntersection);
    }
    return collectionCopy;
}

/**
 * @params {String[]}
 */
function select() {
    var arrArguments = [].slice.call(arguments);
    arrArguments.push('select');
    return arrArguments;
}

/**
 * @param {String} property – Свойство для фильтрации
 * @param {Array} values – Массив разрешённых значений
 */
function filterIn(property, values) {
    var arrArguments = [].slice.call(arguments);
    arrArguments.push('filter');
    return arrArguments;
}

module.exports = {
    query: query,
    select: select,
    filterIn: filterIn
};

function last(arr) {
    return arr[arr.length - 1];
}

function selectionFilter(operation, index) {
    return last(operation) === 'select';
}

function filtrationFilter(operation, index) {
    return last(operation) === 'filter';
}

function removeLast(arr, index) {
    arr.pop();
    return arr;
}

function makeUnique(item, index, self) {
    return self.indexOf(item) === index;
}

function leaveIntersection(toSelect) {
    var n = toSelect.length;
    var count = {};
    for (var i = 0; i < n; ++i) {
        toSelect[i] = toSelect[i].filter(makeUnique);
        for (var j = 0; j < toSelect[i].length; ++j) {
            if (!count.hasOwnProperty(toSelect[i][j])) {
                count[toSelect[i][j]] = 0;
            }
            ++count[toSelect[i][j]];
        }
    }
    var finalSelection = [];
    for (var property in count) {
        if (count[property] === n) {
            finalSelection.push(property);
        }
    }
    return finalSelection;
}

function intersectFiltration(toFilter) {
    var count = {};
    var prop = 0;
    var valuesOfProperty = 1;

    for (var i = 0; i < toFilter.length; ++i) {
        if (!count.hasOwnProperty(toFilter[i][prop])) {
            count[toFilter[i][prop]] = [];
        }
        count[toFilter[i][prop]].push(toFilter[i][valuesOfProperty]);
    }
    for (var property in count) {
        count[property] = leaveIntersection(count[property]);
    }

    return count;
}

function finalFilter(collection, toFilter) {
    for (var j = 0; j < collection.length; ++j) {
        var needAdd = true;
        for (var property in toFilter) {
            needAdd = needAdd && toFilter[property].indexOf(collection[j][property]) !== -1;
            if (!needAdd)
                break;
        }
        if (!needAdd) {
            collection.splice(j--, 1);
        }
    }
}

function leaveNeededProperties(collection, toSelect) {
    var result = [];
    for (var i = 0; i < collection.length; ++i) {
        var objToAdd = {};
        for (var j = 0; j < toSelect.length; ++j) {
            if (collection[i].hasOwnProperty(toSelect[j])) {
                objToAdd[toSelect[j]] = collection[i][toSelect[j]];
            }
        }
        result.push(objToAdd);
    }
    return result;
}