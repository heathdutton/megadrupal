(function($) {
  "use strict";

  $(function() {
    /**
     * Move the checkbox into each field wrapper, aside the right field item.
     * Show the checkbox that allow user to propagate the data of the field.
     */
    $('.translation-field-diffuser-checkbox').each(function() {
      var wrapper = $(this).parent();
      var id = $(this).attr('id');
      var associateFieldId = id.replace('-propagate', '');
      var fieldItemClass = associateFieldId.replace('edit-', '');
      var associateField;

      // For single value field (not all field widget) remove the delta.
      if (!$('#' + associateFieldId).length) {
        associateFieldId = associateFieldId.split('-');
        associateFieldId.pop();
        associateFieldId = associateFieldId.join('-');
      }
      associateField = $('#' + associateFieldId);

      if (associateField.length) {
        // Fieldset is too far away from the field item data.
        if (associateField.get(0).tagName === 'FIELDSET') {
          associateField.find('.form-item').each(function() {
            if ($(this).attr('class').indexOf(fieldItemClass)) {
              associateField = $(this);
              return false;
            }
          });
        }

        // Move the checkbox into the field wrapper is associate to.
        associateField.parent().append(wrapper);
        wrapper.children().show();
      }
    });
  });
})(jQuery);
