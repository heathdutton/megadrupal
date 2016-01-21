/**
 * @file nodejs_chat.module.js
 *
 * Node JS Chat Server extension.
 */

exports.setup = function (config) {
  publishMessageToChannel = config.publishMessageToChannel;
  addClientToChannel = config.addClientToChannel;

  process.on('client-message', function (sessionId, message) {
    // Check message type. Prevents the extension from forwarding any message
    // that is not from the nodejs_chat module.
    if (message.type == 'nodejs_chat') {
      console.log('Node JS Chat extension received a "client-message" event. Action: ' + message.action);
      switch (message.action) {
        // When chat is initialised, user needs to be added to the chat Channel.
        case 'chat_init':
          addClientToChannel(sessionId, message.channel);
          // When entering a chat channel, the client might have sent a message
          // so that users know about this.
          publishMessageToChannel(message);
          break;
        // Usual message transmission.
        case 'chat_message':
          publishMessageToChannel(message);
          break;
      }
    }
  });
};
