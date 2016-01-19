/**
* @file
* Handles display of vertical tabs for CRM Core Donation.
*
*/
(function ($) {
  Drupal.behaviors.cmcd_vertical_tab = {
    attach: function(context) {
      $('fieldset#edit-crm-core-donation', context).drupalSetSummary(function (context) {
        return Drupal.t('Optional settings for donation pages');
      });
    }
  }
})(jQuery);
