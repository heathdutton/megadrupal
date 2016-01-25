/**
 * Image zooming jQuery plugin script by VicTheme.com
 * 
 * Available options
 * (int) multiple = the factor that the image should be enlarged when hovered
 * (int) mouseInSpeed = the animation speed when zoomed in
 * (int) mouseOutSpeed = the animation speed when zoomed out
 * (int) zindex = the z-index to use.
 * (int) topOffset = set additional top offset
 * (int) leftOffset = set additional left offset
 */
(function ($) {
    $.fn.imageEnlarge = function (options) {
			//Set the default values, use comma to separate the settings, example:
			var defaults = {
				multiple: 1.2,
				mouseInSpeed : 200,
				mouseOutSpeed : 400,
				zindex : 600,
				topOffset : 0,
				leftOffset : 0
			};
				
			var options =  $.extend(defaults, options);

			return this.each(function() {
			  
			  var $items = $(this).find('img');
			  
	       // Get the original image height and width only once
        if ($iH === undefined && $iW === undefined) {
          var $iH = $items.height(),
              $iW = $items.width();
        }

				var $position = $items.position(),
						iHH = $iH * options.multiple,
						iHW = $iW * options.multiple,
						mT = ( iHH - $iH ) / 2,
						mL = ( iHW - $iW ) / 2,
						pT = $position.top - mT,
						pL = $position.left - mL;
				
			$(this).hover(function() {
				$items.addClass('iehover').css({'z-index' : options.zindex, 'position' : 'absolute'}).stop().animate({
					top: pT + parseInt(options.topOffset),
					left: pL + parseInt(options.leftOffset),
					width: iHW,
					height: iHH
				}, options.mouseInSpeed);
			}, function() {
				$items.removeClass('iehover').css({'z-index' : options.zindex, 'position' : 'static'}).stop().animate({
					top: $position.top,
					left: $position.left,
					width: $iW,
					height: $iH
				}, options.mouseOutSpeed);
			});
	});
};
})(jQuery);