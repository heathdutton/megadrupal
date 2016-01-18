/**
 * @file
 * Contains js required for republish module.
 */

(function($) {

  Drupal.behaviors.republish = {
    attach: function(context, settings) {

      $(document).ready(function() {
        $('a#republish_content_button').click(function() {
          $('body').append('<div class="overlay-close" id="overlay"><div class="overlay republish-block"><a class="close-button">Close</a>' + $('#republish_content_overlay div.fieldset-wrapper').html() + '</div></div>');
            $('.close-button').click(function() {
              $('.overlay-close').remove();
            });
            $(document).keyup(function(e) {
              if (e.keyCode == 27) {
                $('.overlay-close').remove();
              }
            });
            if ($('.overlay-close')) {
              return FALSE;
            }
          });
        });
      }
  };
})(jQuery);
