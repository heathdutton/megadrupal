Drupal.notification_bar = Drupal.notification_bar || {};

(function ($) {

  Drupal.notification_bar.initialize = function(){
      // Move messages from closure to right after opening of body tag.
      $("body").prepend($("#notification-bar-messages"));
  };

  $(document).ready(function(){
    Drupal.notification_bar.initialize();
  });

})(jQuery);
