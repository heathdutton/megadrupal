jQuery(function($){
	$(document).ready(function(){
		
		// superFish
		$("#main-menu ul.menu").superfish({ 
			autoArrows: true,
			animation:  {opacity:'show',height:'show'}
		});
		
		// back to top
		$('a[href=#toplink]').click(function(){
			$('html, body').animate({scrollTop:0}, 'slow');
			return false;
		});
	
	}); // END doc ready
}); // END function