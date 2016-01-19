(function ($) {
  Drupal.behaviors.submission_timeout_countdown = {
    attach: function($context, $settings) {
      var $ob = Drupal.settings.form_submission_timeout;
      $time = $ob.time;
      $show_timer = $ob.show_timer;
      $form_id = $ob.dom_form_id;
      $rendered_form_id = $ob.form_id;
      $message = $ob.message;
      $url = $ob.url;

      var $timer = $('<div id="fst-countdown"></div>');
      $interval = setInterval(function() {
        updateSubmissionCountdown()
      }, '1000');

      $('#' + $form_id).prepend($timer);
    }
  }

  function updateSubmissionCountdown() {
    if ($time > 1) {
      --$time;
      if ($show_timer) {
        $('#fst-countdown').html('Form times out in ' + $time + ' seconds');
        if ($time < 6) {
          if (!$('#fst-countdown').hasClass('fst-countdown-seconds-left')) {
            $('#fst-countdown').addClass('fst-countdown-seconds-left');
          }
        }
      }
    }
    else {
      clearInterval($interval);
      $('#fst-countdown').html($message).addClass('fst-countdown-over');
      $('#' + $form_id).addClass('fst-input-disabled');
      $('#' + $form_id + ' input[type="submit"]').click(function($e) {
        $e.preventDefault();
      });

      // Reset the $_SESSION timer.
      $.ajax({
        url: $url,
        data: {form_id: $rendered_form_id}
      });
    }
  }
})(jQuery);