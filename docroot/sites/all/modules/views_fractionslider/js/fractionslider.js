/**
 * @file
 * Handles fractionslider js plugin.
 */

(function ($) {
 Drupal.behaviors.fractionslider = {	 
  attach: function (context, settings) {

	 if(Drupal.settings.fractionslider) {

		cont = (Drupal.settings.fractionslider.controls == 'false') ? false : true;
		pag = (Drupal.settings.fractionslider.pager == 'false') ? false : true;
		fullwidth = (Drupal.settings.fractionslider.fullwidth == 'false') ? false : true;
		responsive = (Drupal.settings.fractionslider.responsive == 'false') ? false : true;
		increase = (Drupal.settings.fractionslider.increase == 'false') ? false : true;
		autochange = (Drupal.settings.fractionslider.autochange == 'false') ? false : true;

		$('.slider-wrapper .slider').fractionSlider({
			'fullWidth': fullwidth,
			'controls': cont,
			'pager': pag,
			'responsive': responsive,
			'dimensions': Drupal.settings.fractionslider.dimensions,
			'increase': increase,
			'autoChange' : autochange,
			'pauseOnHover': false,
			'slideEndAnimation': true,
		});

	 }

	 if(Drupal.settings.viewsfractionslider) {

		cont = (Drupal.settings.viewsfractionslider.controls == 'false') ? false : true;
		pag = (Drupal.settings.viewsfractionslider.pager == 'false') ? false : true;
		fullwidth = (Drupal.settings.viewsfractionslider.fullwidth == 'false') ? false : true;
		responsive = (Drupal.settings.viewsfractionslider.responsive == 'false') ? false : true;
		increase = (Drupal.settings.viewsfractionslider.increase == 'false') ? false : true;

		$('.view .slider-wrapper .slider').fractionSlider({
			'fullWidth': fullwidth,
			'controls': cont,
			'pager': pag,
			'responsive': responsive,
			'dimensions': Drupal.settings.viewsfractionslider.dimensions,
			'increase': increase,
			'pauseOnHover': false,
			'slideEndAnimation': true,
			'autoChange' : true,
		});

	 }

  }
 }
})(jQuery);
