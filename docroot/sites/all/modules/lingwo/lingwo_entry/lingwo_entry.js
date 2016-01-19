
(function ($) {
  Drupal.lingwo_entry = {
    callbacks: {
      pos: {},
      language: {},
    },

    setCallback: function (name, type, callback) {
      this.callbacks[type][name] = callback;
    },

    triggerCallbacks: function (type, evt) {
      var callbacks = [], name;

      for (name in this.callbacks[type]) {
        this.callbacks[type][name](evt);
      }
    },
  };

  Drupal.behaviors.lingwo_entry = {
    attach: function (context, settings) {
      Drupal.lingwo_entry.setCallback('lingwo_entry', 'language', function (evt) {
        var id = 'edit-lingwo-entry-reload-pos',
          ajax_settings = settings.ajax[id],
          event_name = ajax_settings['event'],
          selector = ajax_settings['selector'];
        $(selector).trigger(event_name);
      });

      $('#edit-language, #edit-pos', context).once('lingwo_entry', function () {
        $(this).change(function (evt) {
          Drupal.lingwo_entry.triggerCallbacks(evt.target.name, evt);
        });
      });
    }
  };
})(jQuery);

