(function ($) {
  $(document).ready(function() {
    var cbox = $("#edit-instance-settings-auto-dl");
    var help = $(".form-item-instance-settings-auto-dl-help-text");

    // Test the current settings and toggle the help field appropriately
    if (cbox.is(":checked")) {
      help.show();
    }
    else {
      help.hide();
    }

    // Bind the action to take when the box is clicked.
    cbox.click(function() {
      if ($(this).is(":checked")) {
        help.slideDown('fast');
      }
      else {
        help.slideUp('fast');
      }
    });
  });
}(jQuery));
