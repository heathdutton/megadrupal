jQuery(document).ready(function($) {
  $('.criteria-value').change(function() {
    var value = $(this).val();
    if (value.length > 0) {
      var checkbox = $(this).parents('tr').find('input[type="checkbox"].criteria-use');
      checkbox.attr('checked', true);
    }
  });
});
