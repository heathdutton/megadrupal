(function ($) {
	Drupal.behaviors.jquery_msg_alert = {
  attach: function(context) {
	    var theclasses = "";
	    for (message in Drupal.settings.jquery_msg_alert.types) {
			theclasses += "." + Drupal.settings.jquery_msg_alert.types[message] + ", ";
		}		
   	$('div.messages').each(function(index) {
			if ($(this).is(theclasses)) {
			  $(this).MsgAlert({
				alertWidth: Drupal.settings.jquery_msg_alert.alertWidth,
				alertHeight: Drupal.settings.jquery_msg_alert.alertHeight,
				autoClose: Drupal.settings.jquery_msg_alert.autoClose,
				wait: Drupal.settings.jquery_msg_alert.wait,
				closeTime: Drupal.settings.jquery_msg_alert.closeTime, 
				intervalTime: Drupal.settings.jquery_msg_alert.intervalTime,
				cssPosition: Drupal.settings.jquery_msg_alert.cssPosition,
				cssRight: Drupal.settings.jquery_msg_alert.cssRight,
				alertTitleDefault: Drupal.settings.jquery_msg_alert.alertTitleDefault, 
				alertContentDefault: Drupal.settings.jquery_msg_alert.alertContentDefault,
				className: Drupal.settings.jquery_msg_alert.className,
				interSpaceHeight: Drupal.settings.jquery_msg_alert.interSpaceHeight, 
				initialSpaceHeight: Drupal.settings.jquery_msg_alert.initialSpaceHeight
				});
			}
	 })
  }
};
})(jQuery);