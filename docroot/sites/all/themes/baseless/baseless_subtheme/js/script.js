/**
 * @file
 * BaseLESS Helper script, can be deleted if not needed.
 *
 */
 
// BaseLESS Subtheme Scripts
jQuery(document).ready(function($) {
  
  // Adding page width classes
  var limit  = 960, // maximum page width
      unit   = 160, // base unit width
      prefix = 'page-max-width-'; 
      
  windowWidth(prefix, limit, unit);
  
  $(window).resize(function() {
    windowWidth(prefix, limit, unit);
  });
  
  // adding standard width classes
  function windowWidth(prefix, limit, unit) {
    var screenWidth = $(window).width();
    
    for(var i = 1; screenWidth > (unit * i); i++) {
      if(screenWidth > limit) { // sets the limit
        screen_width = prefix + limit;
      }
      else {        
        screen_width = prefix + (unit * i);
      }
    }
    $('body').removeClassWild(prefix + '*').addClass(screen_width);
  }

});
	

