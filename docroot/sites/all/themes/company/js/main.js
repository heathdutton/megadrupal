jQuery(function($){
	$(document).ready(function(){
		// superFish
		$('#main-menu ul.menu').supersubs({ 
			minWidth:    16,
			maxWidth:    40,
			extraWidth:  1
		})
		.superfish();
		// scrolling
			$('a.go-top').click(function(){
			$.scrollTo( 0, 500);
			return false;
		});

		// opacity	
		$(".opacity").hover(
			function() {
			$(this).stop().animate({"opacity": "0.50"}, "fast");
			},
			function() {
			$(this).stop().animate({"opacity": "1"}, "fast");
		});

    //homepage image slider
    $('#slider').nivoSlider({
      directionNav: true,
      directionNavHide: false,
      captionOpacity: 0.8,
      boxCols: 8,
      boxRows: 4,
      effect: 'random',
      slices: 15,
      animSpeed: 500,
      pauseTime: 3000,
      controlNav: false
    });
	}); // end doc ready
}); // end no conflict