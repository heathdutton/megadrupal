/**
 * @file
 * Apple News export settings vertical tab helper.
 */

(function ($) {

  Drupal.behaviors.AppleNewsExportSettings = {
    attach: function(context) {

      // General settings.
      $('#edit-config-additional-settings-general-tab', context).drupalSetSummary(function (context) {
        if ($('#edit-config-additional-settings-general-tab-enabled:checked', context).length) {
          return Drupal.t('Export enabled');
        }
        else {
          return Drupal.t('Export disabled');
        }
      });

      // Metadata
      $('fieldset#edit-config-additional-settings-metadata-tab', context).drupalSetSummary(function (context) {
        if ($('#edit-config-additional-settings-metadata-tab-channels input:checked', context).length !== 0) {
          return Drupal.t('@count default sections. @is_preview', {
            '@count' : $('#edit-config-additional-settings-metadata-tab-channels input:checked', context).length,
            '@is_preview' : $('#edit-config-additional-settings-metadata-tab-is-preview:checked', context).length
              ? Drupal.t('Preview only')
              : Drupal.t('Visible to everyone')
          });
        }
        else {
          return Drupal.t('Default sections and content visibility');
        }
      });

      // Layout
      $('fieldset#edit-config-additional-settings-layouts-tab', context).drupalSetSummary(function (context) {
        if ($('.form-item', context).length !== 0) {
          return Drupal.t('Configure layout settings');
        }
        else {
          return Drupal.t('No layout settings');
        }
      });

      // Components
      $('fieldset#edit-config-additional-settings-components-tab', context).drupalSetSummary(function (context) {
        var count = $('table tbody tr', context).length - 1;
        if (count) {
          return Drupal.formatPlural(count, '1 component enabled', '@count components');
        }
        else {
          return Drupal.t('Please add components');
        }
      });

    }
  };

}(jQuery));
