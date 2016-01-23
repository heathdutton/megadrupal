/**
 * @file
 * Attaches javascript for the GPA Calculator Settings form.
 */

(function($) {
  Drupal.behaviors.gpa_calculator = {
    attach: function (context, settings) {
      // Validation on settings page for Grades textarea.
      $('#gpa-calculator-settings-form .form-submit').click(function() {
        if ($('#edit-gpa-calculator-grades').val() != '') {
          var grades_lines = $('#edit-gpa-calculator-grades').val().split('\n');
          for (var i = 0; i < grades_lines.length; i++) {
            if (grades_lines[i].indexOf('|') === -1) {
              // Only show missing pipe error once.
              if ($('.gpa-pipe-setting-error').length == 0) {
                $("label[for='edit-gpa-calculator-grades']").
                  after('<div class="gpa-pipe-setting-error">' + Drupal.t('You are missing one or more pipe separators in Grades.') + '</div>');
              }
              return false;
            }
            else {
              // Pipe exists on every line so proceed.
              // Remove missing pipe error message if it exists.
              if($('.gpa-pipe-setting-error').length) {
                $('.gpa-pipe-setting-error').remove();
              }

              // Check that characters before pipe are all numbers.
              var option_value = grades_lines[i].substr(0, grades_lines[i].indexOf('|'));

              // If option value before pipe is not a number.
              if (!isFinite(option_value)) {
              // Only show missing numerical value error once.
                if ($('.gpa-value-setting-error').length == 0) {
                  $("label[for='edit-gpa-calculator-grades']").
                    after('<div class="gpa-value-setting-error">' + Drupal.t('One or more of your values before a pipe (|) is not a number.') + '</div>');
                }
                return false;
              }
              else {
                // Remove missing numerical value error if it exists.
                if($('.gpa-value-setting-error').length) {
                  $('.gpa-value-setting-error').remove();
                }
              }
            }
          }
        }
      });
    }
  };
}(jQuery));
