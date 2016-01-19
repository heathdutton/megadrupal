(function($) {
  Drupal.behaviors.notyNotificationsRenderMessages = {
    attach: function(context, settings) {

      // Noty notification close callback
      var notyNotificationsNotificationDelete = function() {
        if (console) console.log(this.options.id, this.options.nid);

        if (this.options.nid != null) {
          // call ajax for deleting notification
          $.get(settings.basePath + 'noty/js/delete/' + this.options.nid);
        }
      }

      var notifications = settings.notyNotifications.notifications;
      for (index in notifications) {
        // define callback
        notifications[index].callback = { onClose: notyNotificationsNotificationDelete };
        // create new notification popup
        var n = noty(notifications[index]);

        if (console) console.log(n.options.id);
      }

    }
  }

})(jQuery);
