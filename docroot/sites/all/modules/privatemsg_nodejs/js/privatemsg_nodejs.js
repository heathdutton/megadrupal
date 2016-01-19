(function ($) {

/**
 * Drupal.Nodejs.callback on message receive in chat.
 */
Drupal.Nodejs.callbacks.privatemsgNodejs = {
  callback: function (message) {
    var pmNodejs = Drupal.privatemsgNodejs,
        $markup = pmNodejs.prepareMarkup(message),
        lastClass = pmNodejs.properties.lastClass,
        $threadWrap = $('.' + pmNodejs.properties.threadWrapClass),
        $lastMessage = $('.privatemsg-message.' + lastClass),
        isThread = window.location.href.indexOf('messages/view/' + message.thread_id),
        $clone,
        uid = Drupal.settings.privateMsgNodeJS.user;

    // Add last class for newly received message.
    $markup.filter('div').addClass(lastClass);

    // Insert new message to the end of matched messages.
    if (isThread >= 0) {
      $clone = $markup.clone();
      $clone.addClass('privatemsg-message');
      $lastMessage
        .removeClass(lastClass)
        .after($clone);
      $threadWrap.find('.typing').remove();

      // Scroll thread to the last received message.
      $threadWrap.find('>div').animate({
        scrollTop: $threadWrap.find('>div')[0].scrollHeight
      }, 500);
    }

    // Insert new message to minichat.
    if ($('.wrapper-mini-chat-' + message.thread_id).length) {
      $miniChat = $('.wrapper-mini-chat-' + message.thread_id);
      $markup.filter('div').addClass('pm-nj-message');
      $miniChat
        .find('.typing').remove()
        .end()
        .find('.' + lastClass)
        .removeClass(lastClass)
        .parent()
        .append($markup.clone());
      var $mesContainer = $miniChat.find('.pmnj-mes');
      $mesContainer[0].scrollTop = $mesContainer[0].scrollHeight;
    }

    // Play sound.
    if ($('body').find('.channel-' + message.channel).length && (message.messageAuthor.uid !== uid)) {
      pmNodejs.soundAlert('message');
    }

    // Attach behavior.
    Drupal.behaviors.privatemsgNodejs.attach();
  }
};

/**
 * Drupal.Nodejs.callback for message alert window.
 */
Drupal.Nodejs.callbacks.privatemsgNodejsAlert = {
  callback: function (message) {
    var pmNodejs = Drupal.privatemsgNodejs,
        threadPath = 'messages/view/' + message.thread_id,
        settings  = Drupal.settings.privateMsgNodeJS;

    if (!($('.wrapper-mini-chat-' + message.thread_id).length) && (window.location.href.indexOf(threadPath) < 1)) {
      if (!$('body').find('.nodejs-messages-wrapper').length) {
        $('body').append('<div class="nodejs-messages-wrapper" />')
      }
      $messageAlert = $(message.markup);
      $('.nodejs-messages-wrapper').prepend($messageAlert);

      // Remove alert after 10 seconds.
      pmNodejs.removeAlert($messageAlert, 10000, 500);

      // Play sound.
      pmNodejs.soundAlert('alert');

      // Remove alert on close button.
      $messageAlert.find('.close-button').bind('click', function (e) {
        pmNodejs.removeAlert($(this).parent(), 1, 500);
        e.stopPropagation();
      });

      // Go to main thread on bubble click.
      $messageAlert.click(function () {
        window.location = Drupal.settings.basePath + threadPath;
      });

      // Show mini chat.
      $messageAlert.find('a.mini-chat').click(function (e) {
        var threadId = $(this).attr('thread_id'),
            token = $(this).attr('token'),
            url = $(this).attr('href');

        // Add mini chat info to settings if mini chat is new.
        pmNodejs.miniChatInfoAdd(threadId, token);

        // Add minichat info to or activate if it exists.
        if ($('.wrapper-mini-chat-' + threadId).length) {
          $('.wrapper-mini-chat-' + threadId).addClass('active');
        } else {
          pmNodejs.miniChatOpen(url, threadId);
        }

        e.preventDefault();
        e.stopPropagation();
      });
    }
  }
};

/**
 * Drupal.Nodejs.callback for message alert window.
 */
Drupal.Nodejs.callbacks.privatemsgNodejsStatus = {
  callback: function (message) {
    var pmNodejs = Drupal.privatemsgNodejs,
        settings = Drupal.settings.privateMsgNodeJS,
        $miniChat = $('.wrapper-mini-chat-' + message.thread_id),
        $threadWrap = $('.thread-wrap-' + message.thread_id),
        $miniChatInner = $miniChat.find('.pmnj-mes-inner'),
        timer = pmNodejs.properties.statusTimer,
        statusMessage = Drupal.t('typing a message...'),
        statusHtml = '<div class="typing">' + message.name + ' ' + statusMessage + '</div>';

    if (settings.user !== message.uid && ($miniChat.length || $threadWrap.length)) {
      // Clear timeout and remove status message.
      clearTimeout(timer[[message.thread_id]]);
      $threadWrap.find('.typing').remove();
      $miniChatInner.find('.typing').remove();

      // Add status message.
      $threadWrap.append(statusHtml);
      $miniChatInner.append(statusHtml);

      // Set timeout for 3 seconds.
      pmNodejs.properties.statusTimer[message.thread_id] = setTimeout(function () {
        $miniChatInner.find('.typing').remove();
        $threadWrap.find('.typing').remove()
      }, 3000);
    }
  }
};

/**
 * Attaches the privatemsgNodejs behavior.
 */
Drupal.behaviors.privatemsgNodejs = {
  attach: function (context, settings) {
    if (settings.privateMsgNodeJS) {
      var pmNodejs = Drupal.privatemsgNodejs,
          $privateMsgForm = $('form[id*="' + pmNodejs.properties.privateMsgFormId + '"]'),
          threadWrapClass = pmNodejs.properties.threadWrapClass,
          $privateMsgNew  = $('.' + pmNodejs.properties.newClass),
          privateMsgMiniForm = pmNodejs.properties.privateMsgMiniFormId;

      // Add mini_chats object if it missed.
      settings.privateMsgNodeJS.mini_chats = settings.privateMsgNodeJS.mini_chats || {};

      // Build mini chats on load.
      miniChats = settings.privateMsgNodeJS.mini_chats;
      $('body').once('open-mini-chats', function() {
        $.each(miniChats, function (i, j) {
          var threadId = j.thread_id,
              url = pmNodejs.genMiniChatUrl('add', threadId, j.x, j.y, j.w, j.h);

          if (!$('.wrapper-mini-chat-' + threadId).length) {
            pmNodejs.miniChatOpen(url, threadId);
          }
        });

        // Bind window resize to prevent mini chat hiding.
        $(window).resize(function (event) {
          pmNodejs.miniChatWindowResize();
        });
      });

      // Read message on new message click.
      $privateMsgNew.live('click', function () {
        var $form = $(this).parents('.' + threadWrapClass).parent().find('form');
        if ($form.length) {
          $form = $form;
        }
        else if ($(this).parents('.pmnj-minichat').length) {
          $form = $(this).parents('.pmnj-minichat').find('form');
        }
        pmNodejs.privateMessageRead($form);
      });

      // Add positioning for clicked mini chat.
      miniChatsSel = 'div[class*="wrapper-mini-chat-"]';
      $(miniChatsSel).live('mousedown', function () {
        $(miniChatsSel).removeClass('at-top');
        $(this).addClass('at-top');
      });

      // Bind events to thread form.
      if ($privateMsgForm.length) {
        var thread = $privateMsgForm.attr('action').match(/(\d+)/g);
        $privateMsgForm.once('messages-form', function () {
          if (thread) {
            statusUrl = Drupal.settings.basePath + 'privatemsg-nodejs/status/' + thread[0] + '/simple/start';

            // Bind events to form textarea.
            $privateMsgForm.find('textarea').bind({
              // Read message on textarea focus.
              'focus': function () {
                pmNodejs.privateMessageRead($(this).parents('form'));
              },
              // Writing status.
              'keyup': function () {
                pmNodejs.miniChatRequest(statusUrl, '', 'writing');
              }
            });

            // Bind submit on private message form.
            // @todo delete if not needed in future.
            //$privateMsgForm.bind('submit', pmNodejs.privateMessageSubmit);

            // Bind mini chat open to link.
            $privateMsgForm.find('.mini-chat').click(function (e) {
              var threadId = $(this).attr('thread_id'),
                  token = $(this).attr('token');

              // Add mini chat info to settings if mini chat is new.
              pmNodejs.miniChatInfoAdd(threadId, token);
              if (!$('.wrapper-mini-chat-' + thread).length) {
                pmNodejs.miniChatOpen(this.href, thread);
              }
              e.preventDefault();
            });
          }
        });

        // Scroll thread to the last received message.
        $('.' + threadWrapClass).once('thread-scroll', function () {
          $(this).find('>div').animate({
            scrollTop: 999999
          }, 500);
        });
      }
    }
  }
};

/**
 * Provide functions for messages manipulations, etc.
 */
Drupal.privatemsgNodejs = {
  // Properties.
  properties: {
    privateMsgMiniFormId: 'form[id*="privatemsg-nodejs-mini-form-"]',
    privateMsgFormId:     'privatemsg-new',
    lastClass:            'privatemsg-message-last',
    newClass:             'privatemsg-message-new',
    threadWrapClass:      'thread-messages-wrap',
    statusTimer:           {}
  },

  /**
   * Function for ajax submit private message form.
   */
  privateMessageSubmit: function (e) {
    var $form = $(this),
        pmNodejs = Drupal.privatemsgNodejs;
        isEmpty = pmNodejs.isEmptyTxt($form.find('textarea').val());

    if (!isEmpty) {
      $.ajax({
        url: $form.attr('action'),
        type: 'POST',
        data: $form.serialize(),
        beforeSend: function () {
          $form[0].reset();
        },
        success: function (data) {}
      });
    }
    e.preventDefault();
  },

  /**
   * Function for ajax read private message.
   *
   * @param $form
   *   jQuery object of message form.
   * @param readUrl
   *   Url for callback page to read the message if such needed.
   */
  privateMessageRead: function ($form, readUrl) {
    $form = $form || $();
    var newClass = Drupal.privatemsgNodejs.properties.newClass,
        $parent = $form.parents().eq(1),
        data = $form.parents('.pmnj-minichat').data(),
        $threadForm,
        basePath = Drupal.settings.basePath,
        url = (readUrl) ? readUrl : '';

    // Check if form is for mini chat.
    if ($form.attr('id') && $form.attr('id').indexOf('privatemsg-nodejs-mini-form') >= 0) {
      url = basePath + 'messages/view/' + data.thread
    }

    // Read new message in thread if it was readed in mini chat.
    if (data && $('form[id*="privatemsg-new"][action*="messages/view/' + data.thread + '"]').length) {
      $threadForm = $('form[id*="privatemsg-new"][action*="messages/view/' + data.thread + '"]');
      $threadForm.parents().eq(1)
        .find('.' + newClass)
        .trigger('click');
    }

    // Read new message in mini chat if was readed in thread.
    if ($form.attr('id') && $form.attr('id').indexOf('privatemsg-new') >= 0) {
      var thread = $form.attr('action').match(/(\d+)/g)[0];
      $('.wrapper-mini-chat-' + thread).find('.' + newClass).removeClass(newClass);
      url = $form.attr('action');
    }

    // Ajax request.
    $.ajax({
      url: url,
      type: 'POST',
      success: function (data) {
        $parent
          .find('.' + newClass)
          .animate({
            'background': '#fff'
          }, 1000, function () {
            $(this).removeAttr('style')
          })
          .removeClass(newClass)
          .end()
          .find('span.new').remove();
      }
    });
  },

  /**
   * Function for prepare markup of message on nodejs response.
   *
   * @param content
   *   Content of received string message from nodejs.
   * @return
   *   Prepared jQuery object with correct username and additional classes.
   */
  prepareMarkup: function (content) {
    var settings = Drupal.settings,
        curentUser = settings.privateMsgNodeJS.user,
        messageAuthor = content.messageAuthor,
        newClass = Drupal.privatemsgNodejs.properties.newClass,
        $content = $(content.markup);

    if (curentUser !== messageAuthor.uid) {
      // We check Access on the js level because message in nodejs is one for
      // all users
      if (settings.privateMsgNodeJS.profileAccess) {
        user = messageAuthor.link;
        $content.find('.user-picture > img').wrap('<a href="' + $(user).attr('href') + '">');
      }
      else {
        user = messageAuthor.name;
        $content.find('.user-picture a img').unwrap();
      }
      // Set username.
      $content
        .addClass('channel-' + content.channel)
        .addClass(newClass)
        .find('.privatemsg-author-name').html(user);
    }
    return $content;
  },

  /**
   * Remove alert.
   *
   * @param $alert
   *   jQuery object of alert message.
   * @param timeout
   *   Timeout to message show.
   * @param speed
   *   Animation speed.
   */
  removeAlert: function ($alert, timeout, speed) {
    timeout = timeout || 5000;
    speed = speed || 500;
    setTimeout(function () {
      $alert.animate({
        'opacity': 0.1
      }, speed, function () {
        $(this).remove();
      });
    }, timeout);
  },

  /**
   * Sound alert.
   *
   * @param type
   *   Type of sound alert, can be 'message' or 'alert' string.
   * 
   * @todo Change this in future.
   */
  soundAlert: function (type) {
    var settings = Drupal.settings,
        privateMsgNodeJS = settings.privateMsgNodeJS,
        audioSel = 'audio[id*="pm-alert"]',
        html;

    if (privateMsgNodeJS.sounds) {
      var sounds = privateMsgNodeJS.sounds;
      if (type == 'message' && sounds.message) {
        soundPath = settings.basePath + privateMsgNodeJS.folder + '/sounds/message.mp3';
        if (sounds.message !== 'default') {
          soundPath = sounds.message;
        }
        html = '<audio id="pm-alert-1" class="message" src="' + soundPath + '"></audio>';
      }
      else if (type == 'alert' && sounds.alert) {
        soundPath = settings.basePath + privateMsgNodeJS.folder + '/sounds/alert.mp3';
        if (sounds.alert !== 'default') {
          soundPath = sounds.alert;
        }
        html = '<audio id="pm-alert-2" class="alert" src="' + soundPath + '"></audio>';
      }
      $('body').append(html);
      if ($(audioSel).length) {
        $(audioSel).parent('.audiojs').find('.pause').click();
        $(audioSel).parent('.audiojs').remove();
      }
      $audionInstance = audiojs.create($(audioSel));
      if ($audionInstance[0].settings.hasFlash && $audionInstance[0].settings.useFlash) {
        $audionInstance[0].settings.autoplay = true;
      }
      $(audioSel).parent('.audiojs').find('.play').click();
      $(audioSel).parent('.audiojs').addClass('pn-nj-audiojs').hide();
    }
  },

  /**
   * Open MiniChat.
   *
   * @param url
   *   Url for mini chat window which contains mini chat in json format.
   * @param threadId
   *   Private message thread id.
   */
  miniChatOpen: function(url, threadId) {
    pmNodejs = Drupal.privatemsgNodejs;

    $.ajax({
      url: url,
      type: 'POST',
      success: function (data) {
        content = data.data.content;
        options = data.data.options;
        tokens = data.data.contentTokens;
        options.thread = threadId;

        // Add new mini chat tokens to node js.
        if (tokens && Drupal.settings.nodejs.contentTokens) {
          $.each(tokens, function (i ,j) {
            Drupal.settings.nodejs.contentTokens[i] = j;
          });
          Drupal.Nodejs.sendAuthMessage();
        }

        // Add mini chat.
        if (content) {
          $content = $(content);
          // remove "New" marker.
          $content.find('span.new').remove();
          $content.data(options);
          $content.find('>div:first')
            .prepend($('<div>', {
              'class': 'close',
              title: Drupal.t('Close')
            }))
            .prepend($('<a>', {
              'class': 'open-thread',
              text:    Drupal.t('Open thread'),
              title:   Drupal.t('Open thread'),
              href:    Drupal.settings.basePath + 'messages/view/' + threadId
            }));
          $content.css({
            'position': 'fixed',
            'left': options.x + 'px',
            'bottom': options.y + 'px',
            'width': options.w + 'px',
            'height': options.h + 'px'
          });

          $('body').append($content);

          // Change size of mini chat inner content.
          pmNodejs.miniChatResizeHelper($content);

          // Scroll mini chat window to last message.
          var $mesContainer = $content.find('.pmnj-mes');
          $mesContainer[0].scrollTop = $mesContainer[0].scrollHeight;

          // Prevent mini chat hiding.
          pmNodejs.miniChatWindowResize();

          // Bind drag to mini chat.
          pmNodejs.miniChatDraggable($content, threadId);

          // Bind resizing to mini chat.
          pmNodejs.miniChatResize($content, threadId);

          // Bind mini chat remove.
          pmNodejs.miniChatDelete($content, threadId);

          // Bind mini chat message sending.
          pmNodejs.miniChatSend($content, threadId);

          // Bind mini chat message read.
          $content.find('textarea').bind('focus', function () {
            pmNodejs.privateMessageRead($(this).parents('form'));
          });
        }
      }
    });
  },

  /**
   * Mini Chat request function for different ajax calls.
   *
   * @param url
   *   Request url.
   * @param data
   *   Data to send if such needed.
   * @param type
   *   Type of request.
   * @param threadId
   *   Private message thread id.
   */
  miniChatRequest: function (url, data, type, threadId) {
    data = (data) ? data : ''
    $.ajax({
      url: url,
      type: 'POST',
      data: data,
      success: function (data) {
        if (type == 'send') {
          Drupal.privatemsgNodejs.privateMessageRead('', Drupal.settings.basePath + 'messages/view/' + threadId);
        }
        if (type == 'writing') {}
      }
    });
  },

  /**
   * Check for empty message.
   *
   * @param value
   *   Text of message.
   * @return
   *   Boolean true or false.
   */
  isEmptyTxt: function (value) {
    return !$.trim(value).length;
  },

  /**
   * Mini Chat send message.
   *
   * @param $content
   *   jQuery object of mini chat.
   * @param threadId
   *   Private message thread id.
   */
  miniChatSend: function ($content, threadId) {
    var pmNodejs = Drupal.privatemsgNodejs,
        url = pmNodejs.genMiniChatUrl('send', threadId),
        $form = $content.find('form');

    $form.find('textarea').keyup(function (e) {
      $this = $(this);
      isEmpty = pmNodejs.isEmptyTxt($this.val());
      // Writing status.
      var statusUrl = Drupal.settings.basePath + 'privatemsg-nodejs/status/' + threadId + '/simple/start';
      pmNodejs.miniChatRequest(statusUrl, '', 'writing');

      if (e.keyCode == 13 && !e.shiftKey && !isEmpty) {
        e.preventDefault();
        $form.submit();
        $form[0].reset();
      }
    });

    $form.submit(function (e) {
      e.preventDefault();
      pmNodejs.miniChatRequest(url, $form.serialize(), 'send', threadId);
    });
  },

  /**
   * Delete mini chat.
   * 
   * @param $content
   *   jQuery object of mini chat.
   * @param threadId
   *   Private message thread id.
   */
  miniChatDelete: function ($content, threadId) {
    $content.find('.close').bind('click', function () {
      $content.remove();
      var pmNodejs = Drupal.privatemsgNodejs,
          url = pmNodejs.genMiniChatUrl('delete', threadId);

      // Request to delete mini chat.
      pmNodejs.miniChatRequest(url);
    });
  },

  /**
   * Enable mini chats dragging.
   * 
   * @param $chat
   *   jQuery object of mini chat.
   * @param threadId
   *   Private message thread id.
   */
  miniChatDraggable: function ($chat, threadId) {
    pmNodejs = Drupal.privatemsgNodejs;

    $chat.draggable({
      handle:      '.name',
      containment: 'window',
      cursor:      'move',
      stop: function(event, ui) {
        var x = ui.position.left,
            y = $(window).height() - (ui.offset.top - $(window).scrollTop()) - ui.helper.outerHeight(true),
            url = pmNodejs.genMiniChatUrl('move', threadId, x, y, $chat.data('w'), $chat.data('h'));

        // Minimum bottom position should be 0.
        y = (y > 0) ? y : 0;

        // Set css after draggable stop.
        $chat.css({
          'top': 'auto',
          'bottom': y
        });

        // Save to object data.
        $chat.data('x', x);
        $chat.data('y', y);

        // Request to save position.
        pmNodejs.miniChatRequest(url);
      }
    });
  },

  /**
   * Enable mini chats resizing.
   *
   * @param $chat
   *   jQuery object of mini chat.
   * @param threadId
   *   Private message thread id.
   */
  miniChatResize: function ($chat, threadId) {
    pmNodejs = Drupal.privatemsgNodejs;

    $chat.resizable({
      handles: 'se',
      containment: 'document',
      resize: function (event, ui) {
        var outerHeight = ui.helper.outerHeight(true),
            height      = ui.helper.height(),
            winHeight   = $(window).height(),
            y = winHeight - (ui.position.top - $(window).scrollTop()) - outerHeight,
            outerSpace = outerHeight - height;

        // Minimum bottom position should be 0.
        y = (y > 0) ? y : 0;

        // Height shouldn't be larger than window height.
        if (winHeight < outerHeight) {
          height = winHeight - outerSpace;
        }

        // Set css after resize.
        $chat.css({
          'top': 'auto',
          'bottom': y,
          'height': height,
          'position': 'fixed'
        });

        // Save to object data.
        $chat.data('y', y);

        // Call resize helper for inner content height.
        pmNodejs.miniChatResizeHelper($chat);
      },
      stop: function (event, ui) {
        var w = $chat.width(),
            h = $chat.height(),
            url = pmNodejs.genMiniChatUrl('resize', threadId, $chat.data('x'), $chat.data('y'), w, h);

        // Save to object data.
        $chat.data('w', w);
        $chat.data('h', h);

        // Request to save size.
        pmNodejs.miniChatRequest(url);
      }
    });
  },

  /**
   * Mini chat resize helper.
   *
   * @param $chat
   *   jQuery object of mini chat.
   */
  miniChatResizeHelper: function ($chat) {
    $mes = $chat.find('.pmnj-mes');
    $mesParent = $chat.find('.pmnj-mes').parent();
    h = $chat.height();
    fH = $chat.find('.form').outerHeight(true);
    mesSpace = $mes.outerHeight(true) - $mes.height();
    mesOuterSpace = $mesParent.outerHeight(true) - $mesParent.height();
    $mes.height(h - fH - mesSpace - mesOuterSpace);
  },

  /**
   * Generate mini chat urls.
   *
   * @param action
   *   Action of event, string value can be: add, send, delete, move, resize.
   * @param threadId
   *   Private message thread id.
   * @param x
   *   Vertical offset number value of mini chat.
   * @param y
   *   Horizontal offset number value of mini chat.
   * @param w
   *   Width value of mini chat.
   * @param h
   *   Height value of mini chat.
   * @return
   *   Url string value for mini chat request.
   */
  genMiniChatUrl: function (action, threadId, x, y, w, h) {
    var settings = Drupal.settings,
        miniChatSettings = settings.privateMsgNodeJS.mini_chats,
        token = miniChatSettings[threadId].token,
        pos = '',
        size = '',
        get = '';
    if (x !== undefined && y !== undefined && x !== false) {
      pos = 'x=' + x + '&y=' + y + '&';
    }
    if (w !== undefined && h !== undefined && w !== false) {
      size = 'w=' + w + '&h=' + h;
    }
    if (pos || size) {
      get = '?';
    }
    return settings.basePath + 'privatemsg-nodejs/mini/' + threadId + '/' + token + '/' + action + get + pos + size;
  },

  /**
   * Add new mini chat info to settings.
   *
   * @param threadId
   *   Private message thread id.
   * @param token
   *   Mini chat token value.
   */
  miniChatInfoAdd: function (threadId, token) {
    var settings = Drupal.settings.privateMsgNodeJS;

    if (!settings.mini_chats[threadId]) {
      settings.mini_chats[threadId] = {
        thread_id: threadId,
        token: token,
        x: 50,
        y: 50
      }
    }
  },

  /**
   * Mini chat window resize.
   */
  miniChatWindowResize: function () {
    pmNodejs = Drupal.privatemsgNodejs;
    var wH = $(window).height(),
        $miniChats = $('.pmnj-minichat');

    $miniChats.each(function () {
      var mH = $(this).height(),
          mOutH = $(this).outerHeight(),
          mPos = $(this).offset();
          mS = mOutH - mH;

      if (mPos.top < 0) {
        $(this).css({
          'top': 0,
          'bottom': 'auto'
        });
      }
      if (mH > wH && wH > 200 && mPos.top <= 0) {
        $(this).height(wH - mS)
        pmNodejs.miniChatResizeHelper($(this));
      }
    });
  }
};

}(jQuery));
