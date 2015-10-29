(function ($) {

Drupal.behaviors.biblebls = {

	attach : function (context, settings) {
		$('span.bls', context).each(function () {
			$(this).hover(function(E) {
	      var scrollTop = parseInt($(document).scrollTop());
	      var scrollLeft = parseInt($(document).scrollLeft());
	      var docWidth = parseInt($(window).width());
	      var docHeight = parseInt($(window).height());
	      var winRight = scrollLeft + docWidth;
	      var winBottom = scrollTop + docHeight;

		    var popup = $('#popup-'+this.id);
		    var leftShift = $(this).offset().left - $(this).position().left;
		    var topShift = $(this).offset().top - $(this).position().top;

	      popup.css('visibility', 'visible');

	      var popupwidth = parseInt(popup.css('width'));
	      var posX = $(this).offset().left - scrollLeft - leftShift;
	      if (posX + popupwidth + leftShift > docWidth) {
	    	  posX = docWidth - popupwidth - leftShift - 5;
	      }
	      popup.css('left', posX);

	      var popupheight = parseInt(popup.css('height'));
	      var posY = $(this).offset().top + $(this).height() - topShift;
	      if (posY + popupheight + topShift > winBottom) {
	      	posY = winBottom - popupheight - topShift - 5;
	      }
	      popup.css('top', posY);
			},
			function() {
				$('#popup-'+this.id).css('visibility', 'hidden');
			});

			$('span.popup').hover(function(E) {
				E.target.style.visibility = "visible";
			},
			function(E) {
				$('span.popup').css('visibility', 'hidden');
			});
		})
	}
};

})(jQuery);
