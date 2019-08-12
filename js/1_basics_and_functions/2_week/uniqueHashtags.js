function render(hashtag, index){
    return hashtag.toLowerCase();
}

function removeDuplicates(hashtag, index, hashtags){
    return hashtags.indexOf(hashtag) === index;
}

/**
 * @param {String[]} hashtags
 * @returns {String}
 */
module.exports = function (hashtags) {
    return hashtags.map(render).filter(removeDuplicates, hashtags).join(', ');
};
