/**
 * @file
 * Utility functions to display settings summaries on vertical tabs.
 */

(function ($) {

Drupal.behaviors.canadapostAdminFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-uc-canadapost-markups', context).drupalSetSummary(function(context) {
      return Drupal.t('Rate markup') + ': '
        + $('#edit-uc-canadapost-rate-markup', context).val() + ' '
        + $('#edit-uc-canadapost-rate-markup-type', context).val() + '<br />'
        + Drupal.t('Weight markup') + ': '
        + $('#edit-uc-canadapost-weight-markup', context).val() + ' '
        + $('#edit-uc-canadapost-weight-markup-type', context).val();
    });
  }
};

})(jQuery);
