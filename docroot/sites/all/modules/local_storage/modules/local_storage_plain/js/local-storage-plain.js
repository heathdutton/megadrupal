(function ($) {
  Drupal.behaviors.localStoragePlain = {
    attach: function (context, settings) {
      if (settings['localStorage']) {
        if (settings['localStorage'].elements) {
          $.each(settings['localStorage'].elements, function (key, elements) {
            $.each(elements, function (id,  settings) {
              if (settings.plugins['local_storage_plain']) {
                $('#' + id).html5lsPlain();
              }
            });
          });
        }
      }
    }
  };
})(jQuery);
