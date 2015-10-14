(function($){
  $(document).ready(function(){
    
    // Make th first slide active
  	$('.home-rotator-slide:first').addClass('active');
  	
  	// Change the active element on hover
    $('.home-rotator-text-block').hover(
     	function () {
     	  if (!$(this).parent().hasClass('active')) {
     	    $('.home-rotator-slide').removeClass('active');
     	    $(this).parent().addClass('active');
     	  }
     	},
     	function () {;
     	}
    );
  	
  });
})(jQuery); //shoots self in head for doing this
            //
            // Kaloyan:
            // I don't know why this works this way byt PhaseII use it this way in menu.js in profile.
            
