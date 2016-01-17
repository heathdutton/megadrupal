/**
 * @file
 * Utility functions to display settings summaries on vertical tabs.
 */

(function ($) {

Drupal.behaviors.canparAdminFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-uc-canpar-markups', context).drupalSetSummary(function(context) {
      return Drupal.t('Rate markup') + ': '
        + $('#edit-uc-canpar-rate-markup', context).val() + ' '
        + $('#edit-uc-canpar-rate-markup-type', context).val() + '<br />'
        + Drupal.t('Weight markup') + ': '
        + $('#edit-uc-canpar-weight-markup', context).val() + ' '
        + $('#edit-uc-canpar-weight-markup-type', context).val();
    });

    $('fieldset#edit-uc-canpar-quote-options', context).drupalSetSummary(function(context) {
      if ($('#edit-uc-canpar-insurance').is(':checked')) {
        return Drupal.t('Packages are insured');
      }
      else {
        return Drupal.t('Packages are not insured');
      }
    });

  }
};

})(jQuery);
