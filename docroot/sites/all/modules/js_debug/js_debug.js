(function($) {
  $(function() {
    if (Drupal.settings.js_debug.messages) {
      for (var i = 0; i < Drupal.settings.js_debug.messages.length; i++) {
        console.log(Drupal.settings.js_debug.messages[i]);
      }
    }
  });
})(jQuery);
