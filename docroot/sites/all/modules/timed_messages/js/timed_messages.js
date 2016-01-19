/**
 * @file
 *   enhances the messages with extra  js
 * @version 7.x-1.x
 */

(function($) {

  Drupal.behaviors.timedMessages = {
    attach: function(context, settings) {
      //work with all messages
      var messages = $('.' + Drupal.settings.timed_messages.messages_class);
      //wrap them, to be able to position the links absolute
      messages.wrap('<div class="messagewrapper"/>');
      //for each individual message
      messages.each(function() {
        var message = $(this);
        //add progress bar
        var progress = $('<div class="timed_message_progress"></div>');
        message.append(progress);
        //add showlink and its functionality, hidden for now
        var toggleclosed = false;
        var togglelink = $('<a href="#" class="showlink">show hidden message</a>');
        //get bgimg from message, to reuse it on the button
        var background = message.css('background-image');
        message.css('background-image', 'none');
        if(background != 'none'){
          togglelink.css('background-image', background);
        }
        togglelink.css('background-position', 'center center');
        message.after(togglelink);
        togglelink.click(function() {
          if (toggleclosed) {
            message.slideDown();
          } else {
            message.slideUp();
            progress.stop(true);
            progress.hide();
          }
          toggleclosed = !toggleclosed;
          return false;
        });
        //add hidelink and its functionality
        var hidelink = $('<a href="#" class="hidelink" title="hide message">hide message</a>');
        message.append(hidelink);
        hidelink.click(function() {
          message.slideUp('slow');
          return false;
        });

        var time;
        //add attributes depending on the message type
        if (message.hasClass(Drupal.settings.timed_messages.status_class)) {
          time = parseInt(Drupal.settings.timed_messages.status_time);
          togglelink.html('show hidden status message');
          togglelink.attr('title', 'show hidden status message');
          togglelink.addClass('status');
        }
        if (message.hasClass(Drupal.settings.timed_messages.warning_class)) {
          time = parseInt(Drupal.settings.timed_messages.warning_time);
          togglelink.html('show hidden warning message');
          togglelink.attr('title', 'show hidden warning message');
          togglelink.addClass('warning');
        }
        if (message.hasClass(Drupal.settings.timed_messages.error_class)) {
          time = parseInt(Drupal.settings.timed_messages.error_time);
          togglelink.html('show hidden error message');
          togglelink.attr('title', 'show hidden error message');
          togglelink.addClass('error');
        }

        //start progress animation, and if wanted show link and  hide the message when finished
        if (
                (
                        message.hasClass(Drupal.settings.timed_messages.status_class) && Drupal.settings.timed_messages.hide_status ||
                        message.hasClass(Drupal.settings.timed_messages.warning_class) && Drupal.settings.timed_messages.hide_warning ||
                        message.hasClass(Drupal.settings.timed_messages.error_class) && Drupal.settings.timed_messages.hide_error
                        ) && (
                message.find('.krumo-root').length <= 0 ||
                Drupal.settings.timed_messages.hide_with_krumo

                )
                ) {
          progress.animate({
            opacity: 1.0,
            width: '95%'
          }, time, function() {
            togglelink.show();
            message.slideUp('slow', function() {
              //hide progress, so when message is shown again, there is no full progress bar, thats not moving
              progress.hide();
              toggleclosed = true;
            });
          });
        }

        //when hovering over the message, pause and resume the progress animation
        message.hover(function() {
          progress.pause();
        }, function() {
          progress.resume();
        });

      });

    }
  };

})(jQuery);
