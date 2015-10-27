(function ($) {
  Drupal.behaviors.status_message = {
    attach: function (context, settings) {
      button = $('#status_message_close_button');
      button.click(function() {
        $.ajax({
          url: '/status_message/dismiss',
          success: function(data) {
            parent = button.parent();
            parent.fadeOut();
          }
        });
      });
    }
  };
})(jQuery);



