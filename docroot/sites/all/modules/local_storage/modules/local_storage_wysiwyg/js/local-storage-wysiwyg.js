/**
 * @file
 * Contains behavior that inits LocalStorageWysiwyg on elements.
 */

(function ($) {
  Drupal.behaviors.localStorageWysiwyg = {
    attach: function (context, settings) {
      if (settings['localStorage']) {
        if (settings['localStorage'].elements) {
          $.each(settings['localStorage'].elements, function (key, elements) {
            $.each(elements, function (id) {
              $('#' + id).html5lsWysiwyg();
            });
          });
        }
      }
    }
  };
})(jQuery);
