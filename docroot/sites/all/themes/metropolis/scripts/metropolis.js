(function ($) {

	Drupal.behaviors.metropolis = {
		attach: function(context, settings) {
			
			if ($.browser.msie && ($.browser.version < 7)) {
				$('#superfish li .nolink').hover(function() {
				$(this).addClass('hover');
				}, function() {
				$(this).removeClass('hover');
				});
			};

		}
	};

})(jQuery);