/* Events for Imgly share form */

(function ($) {
  "use strict";

  Drupal.behaviors.imgly_share = {
    attach: function(context, settings) {
      var $imgly_url = $('#edit-imgly-url', context),
      $imgly_container = $imgly_url.parent();

      // Selects content when clicking on input.
      $imgly_url.click(function() {
        $(this).get(0).select();
      });

      // Copy to clipboard.
      var clip = new ZeroClipboard.Client();
      clip.setText($imgly_url.text());
      clip.setHandCursor(true);
      clip.glue('edit-imgly-copy', 'imgly_clip_container');
      clip.addEventListener('onComplete', function() {
        // Simulate drupal_set_message().
        var $messages = $('#messages', context),
        message = '<div class="messages status"><h2 class="element-invisible">Status message</h2>' + Drupal.t('URL successfully copied to clipboard!') + '</div>';

        if ($messages.length) {
          $messages.find('.section').append(message);
        }
        else {
          $('#header').after('<div id="messages"><div class="section clearfix">' + message + '</div></div>');
        }
      });
    }
  };
}(jQuery));