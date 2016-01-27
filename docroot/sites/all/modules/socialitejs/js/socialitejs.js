(function ($) {
Drupal.behaviors.socialitejs = {
  attach: function() {
  if (Drupal.settings.socialitejs.loading == 'hover'){
    $('.socialitejs').one('mouseenter', function()
		{
		Socialite.load($('.socialitejs').get());
		});
	}
  else {
    Socialite.load($('.socialitejs').get());
  }	
 }
};
}(jQuery));