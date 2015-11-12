(function ($) {
  $(document).ready(function() {
    // Set the classes for the field values in the permissions grid.
    $("table.lyris-field-perms td input:radio").addFieldDefaultClass();
    
    // Set the preview data to match the values entered in the mailing
    // forms fields.
    $("#lyris-mailing-preview .to .value").matchValue('#lyris-mailing-form input#edit-lyris-to');
    $("#lyris-mailing-preview .from .value").matchValue('#lyris-mailing-form input#edit-lyris-from');
  });

  /**
   * Apply a class to a TD element matching the default value of the field
   * within it.
   */
  $.fn.addFieldDefaultClass = function() {
    $(this).change(function() {
      var td = $(this).parents('td');

      td.removeClass('status-0 status-1 status-2');
      td.addClass('status-' + $(this).val());
    });
  };
  
  /**
   * Link a container's HTML value to that of a form field.
   */
  $.fn.matchValue = function(selector) {
    var c = $(this);
    var f = $(selector);
    
    f.keyup(function() {
      c.html(f.val());
    });
  }
})(jQuery);
