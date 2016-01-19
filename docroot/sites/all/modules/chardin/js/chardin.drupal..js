(function($) {
  Drupal.behaviors.chardin = {
    attach: function(context) {
      $('body').chardinJs();
			$('a[data-toggle="chardinjs"]').on('click', function(e) {
				e.preventDefault();
				return ($('body').data('chardinJs')).toggle();
			});
    }
  }
})(jQuery);
