(function ($) {
  Drupal.behaviors.userpointsRegisterFieldsetSummaries = {
    attach: function (context) {
      // This is both for the userpoints settings and the node type settings
      // vertical tab.
      $('fieldset#edit-userpoints-register', context).drupalSetSummary(function (context) {
        if ($('#edit-userpoints-register-enabled').is(':checked')) {
          return Drupal.t('Enabled.');
        }
        else {
          return Drupal.t('Disabled.');
        }
      })
    }
  };
})(jQuery);
