function getHashtags(tweet, index) {
    return tweet.startsWith('#');
}

function render(tweet, index){
    return tweet.slice(1);
}

/**
 * @param {String} tweet
 * @returns {String[]}
 */
module.exports = function (tweet) {
    return tweet.split(' ').filter(getHashtags).map(render);
};
