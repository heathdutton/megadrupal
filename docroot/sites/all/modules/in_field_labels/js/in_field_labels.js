(function(window, document, Math, $, undef) {
  Drupal.behaviors.in_field_labels = {
    attach: function (context, settings) {
      // This enables initializing In Field Labels on multiple forms per page.
      if (undef !== Drupal.settings.in_field_labels && undef !== Drupal.settings.in_field_labels.forms) {
        for (var i = 0 ; i < Drupal.settings.in_field_labels.forms.length ; i++) {
          $("#" + Drupal.settings.in_field_labels.forms[i] + " label", context).once('in-field-labels', function(index, element) {
            $(element).inFieldLabels();
          });
        }
      }
    },
  };
})(window, window.document, Math, jQuery);
