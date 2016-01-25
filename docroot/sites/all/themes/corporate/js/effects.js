jQuery(function($){
  $(document).ready(function() {
    /* SUPERFISH MENU */	
    $('#main-menu ul').superfish({ 
        delay:       200,								// 0.1 second delay on mouseout 
        animation:   {height:'show'}	// fade-in and slide-down animation 
                        // disable drop shadows 
    });
  });
});