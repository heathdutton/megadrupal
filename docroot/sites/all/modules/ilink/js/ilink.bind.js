(function ($) {
  // Initialize iLink object.
  Drupal.Ilink = Drupal.Ilink || {};

  Drupal.behaviors.ilink = {
    attach: function (context, settings) {
      var name, eventName, id;

      // Register iLink objects so we have data, as well as initialize object
      // properties.
      for (name in settings.ilink.resources) {

        // Prevent traversing over prototype properties.
        if (settings.ilink.resources.hasOwnProperty(name)) {
          // Initialize ilink object.
          Drupal.Ilink[name] = Drupal.Ilink[name] || {
            events: {},
          };

          for (eventName in settings.ilink.resources[name].events) {

            // Prevent traversing over prototype properties.
            if (settings.ilink.resources[name].events.hasOwnProperty(eventName)) {

              // Initialize compatible event callbacks. Other modules can now
              // override this method.
              Drupal.Ilink[name].events[eventName] = function(e) {}
            }
          }
        }
      }

      // Loop through each ilink instance..
      for (id in settings.ilink.instances) {

        // Prevent traversing over prototype properties.
        if (settings.ilink.instances.hasOwnProperty(id)) {

          // .. Loop through each specified event of the ilink plugin.
          name = settings.ilink.instances[id].name;
          for (eventName in settings.ilink.resources[name].events) {

            // Prevent traversing over prototype properties.
            if (settings.ilink.resources[name].events.hasOwnProperty(eventName)) {

              // Ensure we bind only once per event.
              $('#' + id).once('ilink-event-' + eventName, function () {

                // Bind event handlers to each ilink instance.
                // @todo this is bork'd. anonymous function is called live, but
                // the argument list is not dynamic.
                $(this).bind(eventName, function () {
                  Drupal.Ilink[name].events[eventName].call(this, {
                    'event': arguments[0],  // first argument is the event argument.
                    'name': settings.ilink.instances[id].name, // name of plugin
                    'arguments': settings.ilink.instances[id].arguments, // array of arguments
                    'type': settings.ilink.instances[id].type // type of implementation (ie, button/link/etc.)
                  });
                });

              });
            }
          }
        }
      }
    }
  }

})(jQuery);
