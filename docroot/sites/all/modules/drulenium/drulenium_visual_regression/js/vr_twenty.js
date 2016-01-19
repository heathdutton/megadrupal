(function ($) {
  Drupal.behaviors.dashboard_notification_get_message = {
    attach: function(context) {
		$(window).load(function() {
		  $("#container1").twentytwenty();
		});
    }
  };
}(jQuery));