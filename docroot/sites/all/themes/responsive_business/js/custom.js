jQuery(function($){
	$(document).ready(function(){
		
		// superFish
		$("#main-menu ul.menu").superfish({ 
			autoArrows: true,
			delay: 400,
		});
		
		// back to top
		$('a[href=#toplink]').click(function(){
			$('html, body').animate({scrollTop:0}, 200);
			return false;
		});
		
	}); // END doc ready
}); // END function
