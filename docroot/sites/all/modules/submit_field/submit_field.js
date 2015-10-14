(function ($) {
  Drupal.behaviors.submitField = {
    attach: function (context) {
      var buttons = '';
      // Get the current action buttons and build out duplicates.
      $('#edit-actions input').each(function() {
        buttons += '<input type="button" class="form-submit" value="' + $(this).val() + '" onClick="document.getElementById(\'' + $(this).attr('id') + '\').click();" />'; 
      });
      // Add the buttons to the added fields.
      $('.submit-field-buttons').html(buttons);
    }
  }
})(jQuery);
