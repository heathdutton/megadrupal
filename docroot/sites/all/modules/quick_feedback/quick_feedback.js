
/**
 * Add personal details to email feedback links.
 */
(function($) {
  Drupal.behaviors.quick_feedback = {
    attach: function (context, settings) {
      
      $.each(Drupal.settings.quick_feedback, function(id, val) {
        var debug = '';

        // Debug.
        if (val.mail_inc_browser) {
          debug += 'Browser: ' + UAParser().browser.name + ' ' + UAParser().browser.major + '%0D%0A';
        }
        if (val.mail_inc_os) {
          debug += 'Operating System: ' + UAParser().os.name + '%0D%0A';
        }

        // Append.
        $('form#' + id.replace(/\_/g, "-") + ' .quick-feedback-wrapper a').attr('href', function(id, attr) {
          return attr + '%0D%0A' + debug
        });

      });

    }
  };
})(jQuery);
