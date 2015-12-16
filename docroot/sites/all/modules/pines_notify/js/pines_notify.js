(function ($) {

  Drupal.behaviors.pinesNotify = {
    attach: function (context, settings) {

      // Javascript handler to render messages as user notifications.
      pinesNotify = function (type, title, message) {
        var notificationSettings = {
          title: title,
          text: message,
          type: type,
          styling: 'jqueryui'
        };
        jQuery.each(Drupal.settings.pines_notify, function(key, value) {
          if (key == 'nonblock' && value == true) {
            notificationSettings['nonblock'] = {
              nonblock: true,
              nonblock_opacity: .2
            };
          }
          else if (key == 'desktop' && value == true) {
            notificationSettings['desktop'] = {
              desktop: true
            };
          }
          else if (key != 'messages') {
            notificationSettings[key] = value;
          }
        });
        // Display the notification.
        new PNotify(notificationSettings);
      };

      $(window).load(function() {
        // Ask the user for permission to use desktop notifications.
        if (Drupal.settings.pines_notify.desktop == 1) {
          PNotify.desktop.permission();
        }

        if (Drupal.settings.pines_notify.messages != undefined) {
          // Output all messages using pinesNotify.
          jQuery.each(Drupal.settings.pines_notify.messages, function(key, value) {
            // Display the notification.
            var title = '';
            if (key == 'success') {
              title = Drupal.settings.pines_notify.title_success;
            }
            if (key == 'error') {
              title = Drupal.settings.pines_notify.title_error;
            }
            pinesNotify(key, title, value);
          });
        }
      });

    }
  };
})(jQuery);
