jQuery(function($){
	$(document).ready(function(){
		$('#home-slides').slides({
			effect: 'fade',
			crossfade: true,
			fadeSpeed: 800,
			slideSpeed: 700,
			play: 5000,
			pause: 10,
			autoHeight: true,
			preload: true,
			hoverPause: true,
			generateNextPrev: true,
			generatePagination: true
		});

	}); // END doc ready
}); // END function