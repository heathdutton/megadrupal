(function($) {

  /**
	 * Toggle Feildset in dependence of selected radio button.
	 */
  Drupal.behaviors.cloudwordsForm = {
    attach: function (context, settings) {
      var $fieldset = $('#edit-stage-fieldset'),
      $radios = $('#edit-cloudwords-client-mode-production, #edit-cloudwords-client-mode-stage');
      // Hide fieldset on form load.
      $fieldset.hide();
      // Check for selected radio button on change.
      $radios.change(function() {
        checkRadios();				
      });
      // Check for selected radio buttons on page load.
      checkRadios();
      // Fieldset toggle function.
      function checkRadios() {
        if ($('#edit-cloudwords-client-mode-production:checked').length) {
          $fieldset.slideUp()
        } 
        else {
          $fieldset.slideDown();
        }
      }
    }
  }

})(jQuery)
