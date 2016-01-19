(function ($) {
  Drupal.behaviors.localStorageCKEditor = {
    attach: function (context, settings) {
      if (settings['localStorage']) {
        if (settings['localStorage'].elements) {
          $.each(settings['localStorage'].elements, function (key, elements) {
            $.each(elements, function (id) {
              $('#' + id).html5lsCkeditor();
            });
          });
        }
      }
    }
  };
})(jQuery);
