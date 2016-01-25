// Simply Scroll JS
(function ($) {
  // Store our function as a property of Drupal.behaviors.
  Drupal.behaviors.simplyScroll = {
    attach: function (context, settings) {
	  $.each(Drupal.settings.simplyscroll, function(id) {
		$('.' + this.customclass).simplyScroll(this);
  	  });
    }
  };
}(jQuery));