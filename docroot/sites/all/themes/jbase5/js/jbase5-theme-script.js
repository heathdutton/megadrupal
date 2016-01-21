(function ($) {

// Placeholder for custom function
/*
Drupal.behaviors.jbase5Something = {
  attach: function (context, settings) {
    
    // Description
    $('a#something').click(function () {
      $(this).toggleClass('active');
    });
    
  }
};
*/


// Modernizer script for IE
Drupal.behaviors.jbase5Modernizer = {
  attach: function (context, settings) {
    if(!Modernizr.input.placeholder){
    
    	$('[placeholder]').focus(function() {
    	  var input = $(this);
    	  if (input.val() == input.attr('placeholder')) {
    		input.val('');
    		input.removeClass('placeholder');
    	  }
    	}).blur(function() {
    	  var input = $(this);
    	  if (input.val() == '' || input.val() == input.attr('placeholder')) {
    		input.addClass('placeholder');
    		input.val(input.attr('placeholder'));
    	  }
    	}).blur();
    	$('[placeholder]').parents('form').submit(function() {
    	  $(this).find('[placeholder]').each(function() {
    		var input = $(this);
    		if (input.val() == input.attr('placeholder')) {
    		  input.val('');
    		}
    	  })
    	});
    
    }
  }
};

})(jQuery);