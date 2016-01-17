(function ($) {

  Drupal.behaviors.timeTradePopup = {
    attach: function (context, settings) {

      $('.timetrade-button.appt-popup').click(function() {
        // Create a div to show a status message in while iframe loads
        $('#fancybox-wrap').append('<div class="waiting">' + Drupal.t('Loading available times... Click to cancel.') + '</div>');
        $('#fancybox-wrap .waiting').click(function() {
          $('#fancybox-wrap .waiting').remove();
          $.fancybox.close()
          return false;
        });

        // If lightbox canceled by clicking on background
        $('#fancybox-overlay').click(function() {
          $('#fancybox-wrap .waiting').remove();
        });

        // After short timeout add onload handler to iframe to remove message
        setTimeout(function() {
          if ($('iframe#fancybox-frame').length) {
            // If iframe exists add handler
            $('iframe#fancybox-frame').load(function() {
              $('#fancybox-wrap .waiting').remove();
            });
          } else {
            // Otherwise just remove the message after 2 seconds
            setTimeout(function() {
              $('#fancybox-wrap .waiting').remove();
            }, 2*1000);
          }
        }, 1000);

      });

      $('a.iframe.video').fancybox({
        'height': 500,
        'width': 650
      });

      $('a.iframe.video-wide').fancybox({
        'height': 450,
        'width': 800
      });

      $('a.iframe.appt-popup').fancybox({
        'height': 500,
        'width': 750
      });

    }
  };


  Drupal.behaviors.timeTradeNewAccount = {
    attach: function (context, settings) {
      // Show status message while waiting for response
      $('#edit-create-account').click(function() {
        $('body').append('<div id="loading-message">' + Drupal.t('Creating account...') + '</div>');
      });
    }
  };

})(jQuery);
