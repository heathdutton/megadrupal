// Need the comment below for Chrome dev tools to pickup the script when it is loaded dynamically by a modal window.
//# sourceURL=country-province.js

// Document is read function
Drupal.behaviors.civiCrmEntitiesFlexiformCountry = {
  attach: function(context, settings) {
    (function($){
      $(function(){
        var $state = $('select.state-id');
        var $country = $('select.country-id');
        var $default_country = $country.attr('data-value');
        var $default_state = $state.attr('data-value');

        // Add change event to country element.
        $country.change(function(){

          // Reset the required mark for each change. The results of the AJAX call will handle hiding it is needed.
          $state.parent().find('.form-required').show();
          $state.find('option').remove();
          $state.append('<option value="">- Select -</option>');

          if($country.val()) {
            $state.append('<option value="">Loading...</option>');

            // Call our AJAX handler to get the states/provinces for the selected country.
            $.ajax('/flexiforms/civicrm/country/' + $country.val()).done(function(data){
              // Set the state/province with the results.
              $state.find('option').remove();
              $state.append('<option value="">- Select -</option>');

              // Don't do a AJAX call if we don't have a country selected.
              if(!$.isEmptyObject(data)) {

                $.each(data, function(key, value) {
                  $state.append('<option value=' + key + '' + ($default_state == key ? ' selected' : '') + '>' + value + '</option>');
                });
              }
              else {
                // Hide the required marker because the state has no options.
                $state.parent().find('.form-required').hide();
              }
            });
          }
        });
      });
    })(jQuery);
  }
};
