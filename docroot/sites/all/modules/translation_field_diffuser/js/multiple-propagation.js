(function($) {
  "use strict";

  $(function() {
    var checkAll = $('.translation-field-diffuser-all-data');
    var checkboxLabel = checkAll.next('label');

    /**
     * Check every checkbox input for a form element when the user click
     * on the global checkbox input
     */
    checkAll.change(function() {
      var checked = false;
      var wrapper = $(this).parents('.fieldset-wrapper');

      if ($(this).is(':checked')) {
        checked = true;
      }

      wrapper.find('.translation-field-diffuser-checkbox').each(function() {
        $(this).prop('checked', checked);
      });
    });

    /**
     * Trigger checkbox change on sibling label click.
     */
    checkboxLabel.click(function() {
      // Get only the sibling checkbox.
      var checkbox = $(this).prev('input');
      checkbox.prop('checked', !checkbox.prop('checked'));
      checkbox.trigger('change');
    });
  });
})(jQuery);
