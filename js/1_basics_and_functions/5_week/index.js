module.exports = {
    subscribers: [],

    /**
     * @param {String} event
     * @param {Object} subscriber
     * @param {Function} handler
     */
    on: function (event, subscriber, handler) {
        var isSubscriberFinded = false;
        for(var i = 0; i < this.subscribers.length; ++i){
            if (this.subscribers[i].name === subscriber){
                isSubscriberFinded = true;
                this.subscribers[i].eventNames.push(event);
                this.subscribers[i].eventActions.push(handler);
                break;
            }
        }
        if (!isSubscriberFinded){
            this.subscribers.push({
                name: subscriber,
                eventNames: [event],
                eventActions: [handler]
            });
        }
        return this;
    },

    /**
     * @param {String} event
     * @param {Object} subscriber
     */
    off: function (event, subscriber) {

    },

    /**
     * @param {String} event
     */
    emit: function (event) {
        for(var i = 0; i < this.subscribers.length; ++i){
            currentSubscriber = this.subscribers[i];
            for(var j = 0; j < currentSubscriber.eventNames.length; ++j){
                if (currentSubscriber.eventNames[j] === event){
                    currentSubscriber.eventActions[j]();
                }
            }
        }
        return this;
    }
};
