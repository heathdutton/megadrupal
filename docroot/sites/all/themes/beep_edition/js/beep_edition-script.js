/**
 * @file
 * Global JavaScript file for the site.
 */


// base function for using flexslider
(function ($) {
	
	       // $(".player").fitVids();

  // All your code here
	$(window).load(function() {

		$('.flexslider').flexslider({
			animation: "slide",
			slideshowSpeed: 7000,
			controlsContainer: ".flexslider-container"
		});

	});
	Drupal.behaviors.something = {
		attach: function (context, settings) {
			$(window).resize(function() {
				// this is here to demonstrate tapping into media queries in JS using Modernizr
				$('body').removeClass('mq-phone');
				$('body').removeClass('mq-sm_tab');
				$('body').removeClass('mq-lg_tab');
				$('body').removeClass('mq-desk');
				$('body').removeClass('mq-desk-plus');
				if(Modernizr.mq('only all and (min-width: 80em)')) {
					$('body').addClass('mq-desk-plus');
				} else if(Modernizr.mq('only all and (min-width: 58em)')) {
					$('body').addClass('mq-desk');
				} else if(Modernizr.mq('only all and (min-width: 45em)')) {
					$('body').addClass('mq-lg_tab');
				} else if(Modernizr.mq('only all and (min-width: 25em)')) {
					$('body').addClass('mq-sm_tab');
				} else {
					$('body').addClass('mq-phone');
				}
			});
		}
	};
})(jQuery);

// Rewritten version for correcting a screen-zoom issue on rotation in iOS
// By @mathias, @cheeaun and @jdalton

(function(doc) {

	var addEvent = 'addEventListener',
		type = 'gesturestart',
		qsa = 'querySelectorAll',
		scales = [1, 1],
		meta = qsa in doc ? doc[qsa]('meta[name=viewport]') : [];

	function fix() {
		meta.content = 'width=device-width,minimum-scale=' + scales[0] + ',maximum-scale=' + scales[1];
		doc.removeEventListener(type, fix, true);
	}

	if ((meta = meta[meta.length - 1]) && addEvent in doc) {
		fix();
		scales = [0.25, 1.6];
		doc[addEvent](type, fix, true);
	}

}(document));