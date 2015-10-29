/**
 * @file
 * Javascript resources for chatblock module
 */
(function ($) {
  // Define a global namespace and prepare variables.
  Drupal.chatblock = {
    "responseIds": new Array()
  }
  // Indicate that we have never called adjustPollRate yet.
  Drupal.chatblock.pollFactorBefore = -1;

  // Also remember we have never set the poll interval.
  Drupal.chatblock.intervalNotSet = true;

  // Remember if this is really the global first time.
  Drupal.chatblock.inited = false;

  // Prepare a flag for keeping track whether the user has scrolled upward.
  Drupal.chatblock.scrolled = false;

  /**
   * Initiate onload functions.
   *
   * @see Drupal.chatblock.scroll()
   * @see Drupal.chatblock.adjustPollRate()
   */
  Drupal.behaviors.chatblockInit = {
    attach: function(context) {

      if (!Drupal.chatblock.inited) {

        // Remove context links in case contextual links is active
        if (Drupal.settings.chatblock.removeLinks && $('div#block-chatblock-chatwindow div.contextual-links-wrapper').length > 0) {
          $('div#block-chatblock-chatwindow div#chatblock-links').remove();
        }

        // Set the actual poll rate
        Drupal.chatblock.adjustPollRate();

        // Bind a click handler to the form submit button.
        $('input#edit-chatblocksubmit', context).click(function(event) {

          // Only contact the server if there is any message content
          if ($('#edit-chatblocktext').val().replace(/(^\s+|\s+$)/g,'') != '') {
            Drupal.chatblock.sendMessage($(this));
            $('#edit-chatblocktext').focus();
          }
          else {
            return false;
          }
          event.preventDefault();
        });

        // Bind an event handler to keep track of the user's scroll behavior
        // (whether he is at the bottom of the chat window or not).
        // The handler also needs to react on window.resize events
        // because the chat window dimensions change and so does probably
        // the scroll position.
        $("div#chatblock-chatcontent").scroll(Drupal.chatblock.setScrollStatus);
        $(window).resize(Drupal.chatblock.setScrollStatus);

        // Make sure pressing enter won't submit the form either
        // (mainly related to older browsers like IE6)
        $('#edit-chatblocktext', context).bind('keydown',function(event){
          if (event.which == 13) {
            event.preventDefault();
            if ($('#edit-chatblocktext').val().replace(/(^\s+|\s+$)/g,'') != '') {
              $('input#edit-chatblocksubmit').click();
            }
            else {
              return false;
            }
          }
        });

        // Remove the submit button and move the input field label into the field.
        //
        // @see Drupal.chatblock.inputFocus()
        if (Drupal.settings.chatblock.minimizeInputForm) {
          $('input#edit-chatblocksubmit', context).hide();

          // This will only work if an input field exists.
          if (Drupal.settings.chatblock.inputFieldLabel) {
            $('div#edit-chatblocktext-wrapper label').hide();
            $('input#edit-chatblocktext').val(
              Drupal.settings.chatblock.inputFieldLabel
            ).focus(function(event) {
              Drupal.chatblock.inputFocus($(this), true);
            }).blur(function(event) {
              Drupal.chatblock.inputFocus($(this), false);
            }).attr(
              'title',
              Drupal.t('Type and press enter to send your message')
            ).addClass('inactive');
          }
        }
        // Scroll to the last chat line.
        Drupal.chatblock.scroll();

        // Attach an event listener to deal with overlays being opened
        // and closed in order to prevent unwanted polling while e.g.
        // deactivating the module.
        $(document).bind('drupalOverlayOpen', function(){Drupal.chatblock.die();});
        $(document).bind('drupalOverlayClose', function(){Drupal.chatblock.setPollInterval();});

        Drupal.chatblock.inited = true;
      }
    }
  }

  /**
   * Initially re-calculate all time strings based on the
   * client's actual localtime.
   */
  Drupal.behaviors.chatblockInitTimeStrings = {
    attach: function() {
      $('span.chatblock-timestamp.chatblock-servertime').each(function(){
        $(this).html(
          '[' + Drupal.chatblock.createTimeString(
            $(this).attr("id").replace(/\D/g,''),
            Drupal.settings.chatblock.timestampFormat
          ) + ']'
        ).removeClass('chatblock-servertime').removeAttr("id");
      });
      $('span.chatblock-timestamp-tooltip.chatblock-servertime').each(function(){
        $(this).attr("title",Drupal.chatblock.createTimeString(
          $(this).attr("id").replace(/\D/g,''),
          Drupal.settings.chatblock.timestampFormat
        )).removeClass('chatblock-servertime').removeAttr("id");
      });

      // Also: Alternative timestamp formats (e.g. for log pages).
      $('span.chatblock-timestamp.chatblock-servertime-alternative').each(function(){
        $(this).html(
          '[' + Drupal.chatblock.createTimeString(
            $(this).attr("id").replace(/\D/g,''),
            Drupal.settings.chatblock.timestampFormatAlternative
          ) + ']'
        ).removeClass('chatblock-servertime-alternative').removeAttr("id");
      });
      $('span.chatblock-timestamp-tooltip.chatblock-servertime-alternative').each(function(){
        $(this).attr("title",Drupal.chatblock.createTimeString(
          $(this).attr("id").replace(/\D/g,''),
          Drupal.settings.chatblock.timestampFormatAlternative
        )).removeClass('chatblock-servertime-alternative').removeAttr("id");
      });
    }
  }

  /**
   * React to enter/leave events for the message input field.
   *
   * Conditionally adds the "type your text here" message to the input field.
   * Also toggles the CSS "active" class.
   */
  Drupal.chatblock.inputFocus = function(element, isActive) {
    if (isActive) {
      element.addClass('active').removeClass('inactive');
      if (element.val() == Drupal.settings.chatblock.inputFieldLabel) {
        element.val('');
      }
    }
    else {
      element.addClass('inactive').removeClass('active');
      if (element.val() == '') {
        element.val(Drupal.settings.chatblock.inputFieldLabel);
      }
    }
  }

  /**
   * Set the follow-up timer for getMessage().
   */
  Drupal.chatblock.setPollInterval = function() {
    if (Drupal.chatblock.pollInterval) {
      window.clearInterval(Drupal.chatblock.pollInterval);
    }
    if (Drupal.chatblock.pollRate) {
      Drupal.chatblock.pollInterval = window.setInterval(
        "Drupal.chatblock.getMessage()",
        Drupal.chatblock.pollRate
      );
    }
  }

  /**
   * Detects and stores whether the chat window
   * is currently scrolled to the bottom or not.
   */
  Drupal.chatblock.setScrollStatus = function() {
    // Decide whether the content is scrolled to the bottom.
    // We leave a tolerance of 9 pixels for difficult screens.
    Drupal.chatblock.scrolled = $("div#chatblock-chatcontent").attr('scrollHeight')
      > $("div#chatblock-chatcontent").innerHeight() + $("div#chatblock-chatcontent").scrollTop() + 5;
  }

  /**
   * Move chat window content to the very bottom.
   */
  Drupal.chatblock.scroll = function() {
    if (!Drupal.chatblock.scrolled) {
      $("div#chatblock-chatcontent").scrollTop(
        $("div#chatblock-chatcontent").attr('scrollHeight')
      );
    }
  }

  /**
   * Check for new messages.
   *
   * Queries the server for new messages since the most recent
   * received message's timestamp. Also sends a random token
   * in order to prevent the response being cached by older
   * browsers like e.g. IE.
   */
  Drupal.chatblock.getMessage = function() {
    jQuery.ajax({
      type: "POST",
      cache: false,
      dataType: "json",
      url: Drupal.settings.chatblock.viewUrl,
      data: {
        'chatboxtoken': Math.random(),
        'cid': Drupal.settings.chatblock.cid,
        'maxId': Drupal.settings.chatblock.maxId,
        'mp': Drupal.settings.chatblock.pathDetails,
        'session': Drupal.settings.chatblock.session,
        'token': Drupal.settings.chatblock.tokenData.usertoken,
        'tokentime': Drupal.settings.chatblock.tokenData.usertokentime
      },
      success: function(data) {
        if (data.ok) {
          Drupal.chatblock.response(data);
        }
        else {
          Drupal.chatblock.die();
        }
      },
      error: function(a,b,c) {
        // Only for debugging cases: alert(b + "\n" + c);
      }
    });
  }

  /**
   * Conditionally increase poll intervals on idle times.
   *
   * Checks how much time has passed since the last message has been received.
   * The poll rate is constantly decreased in configured intervals and will be
   * reset with the next message.
   */
  Drupal.chatblock.adjustPollRate = function() {

    // Only work if a "smart poll stepwidth" has been configured.
    if (Drupal.settings.chatblock.smartPollStepwidth) {

      if (Drupal.settings.chatblock.lastTimestamp) {
        // If a lastTimestamp is given, calculate the smartPollRate.

        var now = new Date();

        // Calculate how many minutes have passed since the last message.
        var minutesPassed = Math.floor(
          (now.getTime() - Drupal.settings.chatblock.lastTimestamp * 1000)
          / 1000
          / 60
        );

        // Calculate the multiplicator for the default poll rate
        // by dividing the minutes having passed by the admin configured
        // "step width" (see admin settings form).
        var pollFactor = Math.max(
          Math.floor(minutesPassed / Drupal.settings.chatblock.smartPollStepwidth),
          1
        );

        // Adjust poll rate, if changed or at first call.
        if (
          Drupal.chatblock.pollFactorBefore == -1
          ||
          pollFactor != Drupal.chatblock.pollFactorBefore
        ) {
          Drupal.chatblock.pollRate = Math.min(
            Drupal.settings.chatblock.pollRateMax * 60 * 1000,
            Drupal.settings.chatblock.pollRateDefault * pollFactor
          );
          Drupal.chatblock.setPollInterval();
          Drupal.chatblock.pollFactorBefore = pollFactor;
        }
      }
      else {
        // If there are no recent messages, set the pollRate to
        // either the maximum or the default value.

        if (Drupal.settings.chatblock.pollRateMax) {
          Drupal.chatblock.pollRate = Drupal.settings.chatblock.pollRateMax * 60 * 1000;
        }
        else {
          Drupal.chatblock.pollRate = Drupal.settings.chatblock.pollRateDefault;
        }
        Drupal.chatblock.setPollInterval();
      }
    } else {
      Drupal.chatblock.pollRate = Drupal.settings.chatblock.pollRateDefault;
      Drupal.chatblock.setPollInterval();
    }
  }

  /**
   * Callback function to handle the json response from the server.
   *
   * @param jsonData
   *   The server response as a Json object
   */
  Drupal.chatblock.response = function(jsonData) {

    // Update status data.
    Drupal.chatblock.updateStatusVars(jsonData);

    // Process all received messages.
    if (jsonData.messages && jsonData.messages.length) {
      jQuery.each(jsonData.messages, function(idx, message) {
        // If we did not already process this message (= unless the
        // message's id is not in our remember array):
        if (jQuery.inArray(message.i, Drupal.chatblock.responseIds) < 0 ) {
          // Prepare username if one is given, otherwise prepare system message
          var userInfo = '';
          if (message.n != '') {
            userInfo = '<span class="chatblock-username">' + message.n + ': </span> ';
          }
          else {
            message.m = '<span class="chatblock-system-message">' + message.m + '</span>';
          }

          // Prepare timestamp
          message.localTimestamp = '';
          if (Drupal.settings.chatblock.timestampTooltip) {
            message.m = '<span class="chatblock-timestamp-tooltip"'
              + ' title="'
              + Drupal.chatblock.createTimeString(message.t, Drupal.settings.chatblock.timestampFormat)
              + '">' + message.m + '</span>'
            ;
          }
          else {
            message.localTimestamp = '<span class="chatblock-timestamp">['
              + Drupal.chatblock.createTimeString(message.t, Drupal.settings.chatblock.timestampFormat)
              + ']</span> '
            ;
          }

          // Add the message to the chat window.
          $("#chatblock-chatcontent").append(
            '<div class="chatblock-message">' + message.localTimestamp
            + userInfo + message.m + '</div>'
          );
          // Keep track of the current messages.
          Drupal.chatblock.responseIds.push(message.i);
        }
      });

      // Try to reduce memory allocation/leaking.
      jsonData = {};
      Drupal.chatblock.scroll();
    }

    // Scroll to the bottom again.
    $("input#edit-chatblocksubmit").attr('disabled','');

    Drupal.chatblock.adjustPollRate();
  }

  /**
   * Update clientside statuses from JSON response data.
   *
   * @param jsonData
   *   The server response object.
   */
  Drupal.chatblock.updateStatusVars = function(jsonData) {
    $.each(['tokenData', 'lastTimestamp', 'maxId'], function(id, el) {
      console.log(jsonData);
      if (jsonData[el]) {
        Drupal.settings.chatblock[el] = jsonData[el];
      }
    });
  }

  /**
   * Create a displayable timestamp from a Unix date.
   *
   * Translates the raw Unix timestamp to the client-localtime.
   * This is done on the client side (if possible)
   * in order to save server time.
   *
   * @param timestamp
   *   The timestamp as provided by the server.
   *
   * @return string
   *   The formatted timestamp string.
   */
  Drupal.chatblock.createTimeString = function(timestamp, timeString) {
    if (Drupal.settings.chatblock.showTimestamps) {

      // Cast timestamp into a number.
      timestamp = Number(timestamp);

      // Transform timestamp from 1/1 sec to 1/1000sec
      timestamp *= 1000;

      // Format time string.
      // @todo Retrieve formatting instructions from Drupal.settings array
      //   (and make it configurable in chatblock settings form)
      clientTime = new Date(timestamp);

      // Support a syntax based on PHP:date() for some tokens.
      // @todo Eventually extend with jQuery.date plugin support.
      timeString = timeString.replace(
        /Y/,clientTime.getFullYear()
      ).replace(
        /y/,clientTime.getFullYear().toString().substr(2,2)
      ).replace(
        /m/,Drupal.chatblock.decimals(clientTime.getMonth()+1)
      ).replace(
        /n/,clientTime.getMonth()+1
      ).replace(
        /d/,Drupal.chatblock.decimals(clientTime.getDate())
      ).replace(
        /j/,clientTime.getDate()
      ).replace(
        /H/,Drupal.chatblock.decimals(clientTime.getHours())
      ).replace(
        /G/,clientTime.getHours()
      ).replace(
        /i/,Drupal.chatblock.decimals(clientTime.getMinutes())
      ).replace(
        /s/,Drupal.chatblock.decimals(clientTime.getSeconds())
      );

      return timeString;
    }
    else {
      return '';
    }
  }

  /**
   * Helper function to add leading 0 to time values.
   */
  Drupal.chatblock.decimals = function(timeval) {
    return (timeval < 10 ? '0' : '') + timeval;
  }

  /**
   * This function is called when the user wants to send a message to the database
   * It expects the server to response with any new messages (including the users message, filtered)
   */
  Drupal.chatblock.sendMessage = function(button) {
    // disable the submit button so the user can't submit twice
    button.attr('disabled','disabled');

    // send the message
    jQuery.ajax({
      type: "POST",
      dataType: "json",
      url: Drupal.settings.chatblock.updateUrl,
      data: {
        'maxId': Drupal.settings.chatblock.maxId,
        'chatboxtoken': Math.random(),
        'message': $("input#edit-chatblocktext").val(),
        'token': Drupal.settings.chatblock.tokenData.usertoken
      },
      success: function(data) {
        if (data.ok) {
          Drupal.chatblock.response(data);
        }
        else {
          Drupal.chatblock.die();
        }
      },
      error: function (XMLHttpRequest, textStatus, errorThrown) {
        console.error(XMLHttpRequest.status);
      }
    });

    // Empty the message input textfield.
    $("input#edit-chatblocktext").val('');

  }

  /**
   * Stop all intervals, discontinue polling.
   *
   * Will be called by Ajax response handlers unless a response contains "ok".
   */
  Drupal.chatblock.die = function() {
    window.clearInterval(Drupal.chatblock.pollInterval);
  }
})(jQuery);
