/**
 * @file
 * Contains behavior that inits LocalStorageMessage on elements.
 */

(function ($) {
  Drupal.behaviors.localStorageMessage = {
    attach: function (context, settings) {
      if (settings['localStorage']) {
        if (settings['localStorage'].elements) {
          $.each(settings['localStorage'].elements, function (key, elements) {
            $.each(elements, function (id,  settings) {
              if (settings.plugins['local_storage_message']) {
                $('#' + id).html5lsMessage();
              }
            });
          });
        }
      }
    }
  };
})(jQuery);
