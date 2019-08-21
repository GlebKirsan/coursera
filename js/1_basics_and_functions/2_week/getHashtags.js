/**
 * @param {String} tweet
 * @returns {String[]}
 */
module.exports = function(tweet) {
    return tweet.split(' ').filter(getHashtags)
        .map(removeSymbol);
};

function getHashtags(word, index) {
    return word.startsWith('#');
}

function removeSymbol(word, index) {
    return word.slice(1);
}