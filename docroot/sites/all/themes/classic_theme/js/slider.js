/* --------------------------------------------- 

* Filename:     slider.js
* Version:      1.0.0 (2015-03-21)
* Website:      http://www.zymphonies.com
                http://www.freebiezz.com
* Description:  System JS
* Author:       Zymphonies Team
                info@zymphonies.com
                
-----------------------------------------------*/

jQuery(function(){
  jQuery('#slides').slides({
    play: 5000,
    pause: 2500,
    hoverPause: true 
  });
});

jQuery(document).ready(function() {
  jQuery("#slides").hover(function() {
    jQuery(".slides_nav").css("display", "block");
  },
  function() {
    jQuery(".slides_nav").css("display", "none");
  });
}); 