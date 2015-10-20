
// domready event
jQuery(function($) {

  $('#node-registration-registrations-settings-form input').keydown(function(e) {
    if (13 == e.keyCode) {
      e.preventDefault();

      // Don't submit via the first button; use #edit-submit instead.
      var submit = $(this.form).find('#edit-submit');
      submit.length && submit.click();
    }
  });

});

// IIFE
(function($) {

  // Fieldset summariy handlers.
  if ($.fn.drupalSetSummary) {
    Drupal.behaviors.node_registration_fieldsetSummary = {
      attach: function(context) {

        // Node form.
        $('.node-form #edit-node-registration', context).drupalSetSummary(function(context) {
          var enabled = $('.node-registration-status:checked', context).length,
            text = enabled ? Drupal.t('Enabled') : Drupal.t('Disabled');
          return text;
        });

        // Node type form.
        $('.node-type-form #edit-registration', context).drupalSetSummary(function(context) {
          return $('.node-registration-status option:selected', context).text();
        });

      }
    };
  }

})(jQuery);
