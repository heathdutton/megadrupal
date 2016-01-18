/**
 * @file
 * Control the number of character in the live box tweet, live edit twitter status, count down between tweets.
 */

(function ($) {

  Drupal.behaviors.twitter_queue_list = {

    attach: function (context, settings) {

      var delay = (function () {
        var timer = 0;
        return function(callback, ms) {
          clearTimeout(timer);
          timer = setTimeout(callback, ms);
        };
      })();

      // Timer.
      /**
       * This piece of code is working, the problem is. It's really useful? Idk it keep it if someone ask to make a count down I use it.
       *
      var min = parseInt($('.timer-between strong').text());
      var sec = 59;
      countdown = setInterval(function () {
        $(".timer-between strong").html(min + ':' + sec);
        if (sec == 0) {
          min--;
          sec = 60;
          if (min < 1 && sec == 0) {
            window.location = Drupal.settings.basePath + 'admin/structure/twitter-queue';
          }
        }
        sec--;
      }, 1000);
      // */

      $('#edit-textarea').bind('focus', function () {
          $(this).attr('class', 'textOn');
          if ($(this).val() == Drupal.t("What's happening?")) $(this).val('');

      });

      $('#edit-textarea').bind('blur', function (){
          if ($(this).val() == '') {
              $(this).attr('class', 'textOff');
              $(this).val(Drupal.t("What's happening?"));
          }
      });

      $('#edit-textarea').keyup(function () {
        delay(function() {
          total = 140 - parseInt($('#edit-textarea').val().length);
          $('#totalchar').html(total);
        }, 200);
      });

      $('.queue-status span.text').bind('click', function () {
        value = $(this).text();

        if ($(this).hasClass('edit-mode').toString() != 'true') {
          input = '<input type="text" value="' + value + '" />';
          actions = '<a href="#save" class="save">' + Drupal.t('save') + '</a>' + ' ' + '<a href="#cancel" class="cancel">' + Drupal.t('cancel') + '</a>';

          $(this).html(input + ' ' + actions);
        }

        $(this).addClass('edit-mode');
      });

      $('.queue-status a.cancel').bind('click', function () {
        console.log('bling bling')
      });

    }
  }

})(jQuery);
