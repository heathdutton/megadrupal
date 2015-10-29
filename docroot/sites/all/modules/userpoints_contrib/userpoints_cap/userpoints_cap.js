(function ($) {
  Drupal.behaviors.userpointsCapFieldsetSummaries = {
    attach: function (context) {
      $('fieldset#edit-userpoints-cap', context).drupalSetSummary(function (context) {
        if ($('#edit-userpoints-cap-enabled').is(':checked')) {
          return Drupal.t('Enabled');
        }
        else {
          return Drupal.t('Disabled');
        }
      })
    }
  };
})(jQuery);
