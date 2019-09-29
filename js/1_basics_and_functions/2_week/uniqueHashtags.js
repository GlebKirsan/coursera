/**
 * @param {String[]} hashtags
 * @returns {String}
 */
module.exports = function(hashtags) {
    return hashtags.map(toLowerCaseCallback)
        .filter(findUnique).join(', ');
};

function toLowerCaseCallback(hashtag, index) {
    return hashtag.toLowerCase();
}

function findUnique(word, index, self) {
    return self.indexOf(word) == index;
}