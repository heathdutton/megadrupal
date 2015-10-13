(function ($) {  
  	Drupal.behaviors.aeSettingsForm = {
    	attach: function (context, settings) {    		
    		$('.addressbook-extra-table tbody tr').each(function() {
    			var registerCheckbox = $(this).find('.register-cell input');
    			var profileCheckbox = $(this).find('.profile-edit-cell input');
                var addrEnableCheckbox = $(this).find('.addr-enable-cell input');
                if (!registerCheckbox.is(':checked')) {
                    profileCheckbox.attr('disabled', true);
                }
    			// check AddrEnable checkox by checking Register checkbox
    			registerCheckbox.click(function() {
    				if ($(this).is(':checked')) {    					
                        profileCheckbox
                        .attr('checked', true)
                        .attr('disabled', false);
    					addrEnableCheckbox.attr('checked', true);

    				} else {
                        profileCheckbox
                        .attr('checked', false)
                        .attr('disabled', true);
                    }
    			});
    		});
		}
  	};
})(jQuery);