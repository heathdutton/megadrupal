(function ($) {
	Drupal.behaviors.chardinDemo = {
    attach: function(context) {
			$('#chardin-toggle').on('click', function(e) {
				e.preventDefault();		
				if (!$('.jumbotron img').is(':visible')) {
					return $('.jumbotron img').animate({
						height: 250
					}, 600, function() {
						return '';
					});
				}
			});
		}
	}
}(jQuery));
