(function ($) {
  Drupal.behaviors.pusher_notifications_block = {
    attach: function (context) {
      $('#block-pusher-notifications-pusher-notifications .pusher-notifications-link').click(function() {
        $("#pusher-notifications-list").toggleClass('element-invisible');
        $(this).toggleClass('active');

        if ($('#pusher-notifications-list.element-invisible').length == 0 &&
            $('.pusher-notificaitons-list-content .pusher-notification-wrapper').length == 0) {
          // Ajax
          $.ajax({
            url: Drupal.settings.basePath + 'notifications/ajax',
            success: function(data) {
              $('.pusher-notificaitons-list-content').html(data);
            }
          });
        }

        return false;
      });
    }
  }
})(jQuery);
