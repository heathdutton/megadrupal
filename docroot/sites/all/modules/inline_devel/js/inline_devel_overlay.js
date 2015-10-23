/**
 * @file
 * Handle js functionality for a quick access to the execute form in every page.
 */

(function ($) {

Drupal.behaviors.functionLoad = {
  attach: function(context, settings) {
    $(document).keydown(function(event) {
      if (event.which == 65 && event.ctrlKey == true  && event.shiftKey == true) {
        if ($("#quick-code").css("display") != 'block') {
          window.location = settings.baseURL + settings.q + '#overlay=devel/php';
        }
      }
    });
  }
}

})(jQuery);
