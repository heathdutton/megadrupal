/* --------------------------------------------- 

* Filename:     effects.js
* Version:      1.0.0 (2015-03-21)
* Website:      http://www.zymphonies.com
                http://www.freebiezz.com
* Description:  System JS
* Author:       Zymphonies Team
                info@zymphonies.com
                
-----------------------------------------------*/

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