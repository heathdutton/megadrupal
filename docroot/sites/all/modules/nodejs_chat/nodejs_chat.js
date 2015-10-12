/**
 * @file nodejs_chat.js
 *
 * Node JS Callbacks and general Javascript code for the Node JS Chat module.
 */

(function ($) {

  Drupal.nodejs_chat = Drupal.nodejs_chat || {'initialised' : false};
  var chatIdsMapping = {};

  Drupal.nodejs_chat.initialiseChat = function() {
    for (var chat in Drupal.settings.nodejs_chat.chats) {
      // For Chats transmitting messages through sockets, make sure the user is
      // added to the chat channel.
      if (!Drupal.settings.nodejs_chat.chats[chat].settings.messageTransmission) {
        Drupal.nodejs_chat.addClientToChatChannel(Drupal.settings.nodejs_chat.chats[chat].channel);
      }

      // Chat form events handling.
      var chatID = '#nodejs_chat_' + Drupal.settings.nodejs_chat.chats[chat].channel;
      chatIdsMapping[chatID] = Drupal.settings.nodejs_chat.chats[chat].channel;

      $(chatID + ' .form-type-textarea textarea').keyup(function(e) {
        if (e.keyCode == 13 && !e.shiftKey && !e.ctrlKey) {
          Drupal.nodejs_chat.processMessageArea(e);
        }
        else {
          return true;
        }
      });

      $(chatID + ' .form-submit').click(function (e) {
        e.preventDefault();
        e.stopPropagation();
        Drupal.nodejs_chat.processMessageArea(e);
      });
    }
  }

  Drupal.Nodejs.connectionSetupHandlers.nodejs_chat = {
    connect: function() {
      Drupal.nodejs_chat.initialiseChat();
      Drupal.nodejs_chat.initialised = true;
    }
  }

  Drupal.Nodejs.callbacks.nodejsChatUserOnlineHandler = {
    callback: function (message) {
//      console.log(message);
      if (Drupal.settings.nodejs_chat.chats[message.channel].settings.showNotifications) {
        Drupal.nodejs_chat.showNotification(message.data.body, message.data.subject, message.channel);
      }
      // Get variables for user-box markup
      var uid = message.data.user.uid;
      if (message.data.user.picture != '0') {
        var picture = message.data.user.picture;
      }
      else var picture = Drupal.settings.nodejs_chat.globalSettings.defaultAvatar;
      var name = message.data.user.name;

      var userMarkup =  '<li class="nodejs-chat-user-box-' + uid + '" class="nodejs-chat-user-box first">' +
                          '<img src="' + picture + '" width="35" height="35" alt="">' +
                          '<span class="username">' + name + '</span></li>';

      if ($('#nodejs_chat_' + message.channel + ' .nodejs-chat-user-box-' + uid).length == 0) {
        $('#nodejs_chat_' + message.channel + ' .user-list ul').append(userMarkup)
      }

    }
  }

  Drupal.Nodejs.callbacks.nodejsChatMessageHandler = {
    callback: function(message) {
      var msg = message.data;
      var chatID = '#nodejs_chat_' + message.channel;

      // Get current date, to display the time at which the message was sent.
      var currentTime = new Date();
      var messageTime = '<span class="message-time">' + currentTime.getHours() + ':' + currentTime.getMinutes() + '</span>';
      var messageAuthor = '<span class="message-author">' + msg.name + ':</span>';

      // Display URLs as proper links.
      // After failing for some time with my custom regex, took one from
      // http://kroltech.com/2013/05/quick-tip-regex-to-convert-urls-in-text-to-links-javascript/,
      // because I'm no man of honor.
      var regexp = /(\b(https?|ftp|file):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig;
      var parsedText = msg.msg.replace(regexp, "<a href='$1' target='_blank'>$1</a>");

      var messageText = '<span class="message-text">' + parsedText + '</span>';

      // Assemble the markup for the message.
      var messageMarkUp = '<div class="nodejs-chat-message"><div class="message-content"> ' + messageAuthor + messageText + '</div>'
        + messageTime + '</div>';

      // Finally, add it to the chat log.
      $(chatID + ' .chat-log').append(messageMarkUp);

      // Scroll to the last comment. TODO: This has to be improved, to avoid
      // auto-scrolling when a user is reading the comments log. Checking if the
      // chat-log div is focused might be enough.
      $(chatID + ' .chat-log')[0].scrollTop = $(chatID + ' .chat-log')[0].scrollHeight;
    }
  }

  Drupal.Nodejs.contentChannelNotificationCallbacks.nodejs_chat = {
    callback: function (message) {
      if (message.contentChannelNotification && message.data.type == 'disconnect') {
        // The next check is probably unneeded. This callback will be triggered
        // Only on clients linked to socket.io channel generating the event.
//        if (message.channel == Drupal.settings.nodejs_chat.chatwindow) {
          var uid = message.data.uid;
          if (Drupal.settings.nodejs_chat.chats[message.channel].settings.showNotifications) {
            Drupal.nodejs_chat.showNotification('User with uid: ' + uid + ', went offline', 'User disconnected', message.channel);
          }
//        }
        // Remove user from connected users.
        var chatID = '#nodejs_chat_' + message.channel;
        $(chatID + ' .nodejs-chat-user-box-' + uid).remove();
      }
    }
  }

  Drupal.nodejs_chat.addClientToChatChannel = function(channelId) {
    var msg = {
      type: 'nodejs_chat',
      action: 'chat_init',
      channel: channelId,
      // Set the callback so all users know a new user has entered the chat.
      callback: 'nodejsChatUserOnlineHandler',
      data: {
        user: Drupal.settings.nodejs_chat.currentUser
      }
    };
    Drupal.Nodejs.socket.emit('message', msg);
  }

  Drupal.nodejs_chat.postMessage = function(message, channelDomId) {
    var channelId = chatIdsMapping[channelDomId];

    if (Drupal.settings.nodejs_chat.chats[channelId].settings.messageTransmission == 'drupal') {
      var postMessagePath = Drupal.settings.nodejs_chat.chats[channelId].settings.postMessagePath;
      $.ajax({
        type: 'POST',
        url: postMessagePath + '/' + channelId,
        dataType: 'json',
        success: function(data) {},
        data: {
          message: message
        }
      });
    }
    else {
      var msg = {
        type: 'nodejs_chat',
        action: 'chat_message',
        channel: channelId,
        callback: 'nodejsChatMessageHandler',
        data: {
          uid: Drupal.settings.nodejs_chat.currentUser.uid,
          name: Drupal.settings.nodejs_chat.currentUser.name,
          msg: message
        }
      };
      Drupal.Nodejs.socket.emit('message', msg);
    }
  }

  Drupal.nodejs_chat.showNotification = function(subject, message, channel) {
    var notificationTime = (Drupal.settings.nodejs_chat.chats[channel].settings.notificationTime)
      ? Drupal.settings.nodejs_chat.chats[channel].settings.notificationTime
      : Drupal.settings.nodejs_notify.notification_time * 1000;
    $.jGrowl(message, {header: subject, life: notificationTime});
  }

  Drupal.nodejs_chat.processMessageArea = function(e) {
    var domChatID = '#' + $(e.target).closest('.nodejs-chat').attr('id');
    var messageText = $('<div></div>').text($(domChatID + ' .form-type-textarea textarea').val()).html().replace(/^\s+$/g, '');
    if (messageText) {
      Drupal.nodejs_chat.postMessage(messageText, domChatID);
      $(domChatID + ' .form-type-textarea textarea').val('').focus();
    }
  }

})(jQuery)