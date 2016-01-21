(function ($) {
  // Original JavaScript code.
  
var eh_state = 0;

function eh_changeMessage() {

        switch(eh_state)
        {
          case 1: 
            jQuery("#earth_hour #inner").hide().html(eh_msg_1).fadeIn();	
		    eh_state = 2;
            break;
          case 2: 
		    jQuery("#earth_hour #inner").hide().html(eh_msg_2).fadeIn();
		    eh_state = 0;
            break;
          default: caption ="default";
            jQuery("#earth_hour #inner").hide().html(eh_msg_3).fadeIn();
            eh_state = 1;
        }

}

jQuery(document).ready( function() {
	setInterval( eh_changeMessage, 15000); 
});

})(jQuery);
