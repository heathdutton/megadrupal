(function ($) {

  Drupal.behaviors.blackseaSuperfish = {

    attach: function(context, settings) {
      $('#main-menu ul.menu', context).superfish({
		    delay: 300,											    
        animation: {opacity:'show' },
        speed: 300,
        autoArrows: false,
        dropShadows: false /* Needed for IE */
      });
    }
	}

})(jQuery);  