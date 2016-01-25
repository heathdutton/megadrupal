// Simple javascript snippet to check/uncheck all checkboxes from the line item
// manager widget.
jQuery(document).ready(function () {
  jQuery('#return-line-item-manager .select-all').click(
    function () {
      if(this.checked) {
      jQuery('#return-line-item-manager tbody input.form-checkbox').each(
        function () {
          jQuery(this).attr('checked', 'checked');
        });
    } else {
        jQuery('#return-line-item-manager tbody input.form-checkbox').each(
          function () {
            jQuery(this).removeAttr('checked');
          });
      }
    }
  );
});