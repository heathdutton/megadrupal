(function ($) {  
  	Drupal.behaviors.aeSettingsForm = {
    	attach: function (context, settings) {             

    		$('.addressbook-extra-table tbody tr').each(function() {
                var fieldCheckbox = $(this).find('.field-cell input');
    			var registerCheckbox = $(this).find('.register-cell input');
    			var profileCheckbox = $(this).find('.profile-edit-cell input');
                var addrEnableCheckbox = $(this).find('.addr-enable-cell input');                             
                var requiredFieldsCheckbox = $(this).find('.required-fields-cell input');
    			// profile & register checkboxes have same handler
    			profileCheckbox.click({
                    fieldCheckbox: fieldCheckbox,
                    registerCheckbox: registerCheckbox, 
                    profileCheckbox: profileCheckbox,
                    requiredFieldsCheckbox: requiredFieldsCheckbox
                }, updateStates);
                registerCheckbox.click({
                    fieldCheckbox: fieldCheckbox,
                    registerCheckbox: registerCheckbox, 
                    profileCheckbox: profileCheckbox,
                    requiredFieldsCheckbox: requiredFieldsCheckbox
                }, updateStates);

                requiredFieldsCheckbox.click({        
                    fieldCheckbox: fieldCheckbox,            
                    registerCheckbox: registerCheckbox
                    }, function(event) {   
                        var fieldCheckbox = event.data.fieldCheckbox;                     
                        var registerCheckbox = event.data.registerCheckbox;
                        // if required
                        if ($(this).is(':checked')) {
                            registerCheckbox
                            .attr('checked', true)
                            ;
                            //
                            fieldCheckbox
                            .attr('checked', true);
                        } else {
                            registerCheckbox                            
                            .attr('disabled', false);
                        }
                    }
                );
    		});

            function updateStates(event) {                
                var fieldCheckbox = event.data.fieldCheckbox;
                var registerCheckbox = event.data.registerCheckbox;
                var profileCheckbox = event.data.profileCheckbox;
                var requiredFieldsCheckbox = event.data.requiredFieldsCheckbox;

                // activate field checkbox after checking
                // register/profile boxes
                if (registerCheckbox.is(':checked') ||
                    profileCheckbox.is(':checked')) {
                    fieldCheckbox
                    .attr('checked', true);
                }

                // when all options has been disabled - 
                // show warning and propositi to remove 
                // a field                
                if (!registerCheckbox.is(':checked') &&
                    !profileCheckbox.is(':checked') &&
                    fieldCheckbox.is(':checked')) {                    
                    // disable field checkbox and remove 
                    // after form submit
                    fieldCheckbox
                    .attr('checked', false);
                } 

                if (!registerCheckbox.is(':checked') &&
                    requiredFieldsCheckbox.is(':checked')) {
                    requiredFieldsCheckbox.attr('checked', false);
                }              
            };
		}
  	};
})(jQuery);