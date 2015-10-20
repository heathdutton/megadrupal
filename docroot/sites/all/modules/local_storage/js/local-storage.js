(function ($) {
  Drupal.behaviors.localStorage = {
    attach: function (context, settings) {
      var isWindow = (context == window.document);
      // Whether browser support local storage.
      var isStorage = (typeof(window['localStorage']) != undefined);
      // Whether browser support JSON.
      var isJSON = (typeof(window['JSON']) != undefined);

      if (!isWindow || !isStorage || !isJSON) {
        return;
      }

      if (settings['localStorage']) {
        settings = settings['localStorage'];
        Drupal.localStorageCore = new LocalStorageCore('localStorage', settings);
        if (settings.elements) {
          $.each(settings.elements, function (key, elements) {
            $.each(elements, function (id, elSettings) {
              $('#' + id)
                .html5ls(Drupal.localStorageCore, key, elSettings)
                .init();
            });
          });
        }
      }
    }
  };
})(jQuery);
