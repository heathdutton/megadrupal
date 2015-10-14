(function ($) {

    $(document).ready(function(){															 

			if ($.browser.msie) { } else {
				$('ul.menu').mobileMenu({
				combine: true,
				switchWidth: 760,
				prependTo: "#main-menu",
				nested: true,
				groupPageText: 'More',
				topOptionText: 'Select a page'
				});
			}
			
    }); 

 	Drupal.behaviors.bonesSuperfish = {
	
			attach: function(context, settings) {
					
			$('#main-menu ul.menu', context).superfish({
				delay: 400,											    
				animation: {height:'show' },
				speed: 500,
				easing: 'easeOutBounce', 
				autoArrows: false,
				dropShadows: false /* Needed for IE */
			});
				
			}
		}		
				
})(jQuery);  