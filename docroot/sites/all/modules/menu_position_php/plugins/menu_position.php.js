(function ($) {
  /**
   * Provide the summary information for the lolspeak plugin's vertical tab.
   */
  Drupal.behaviors.menuPositionPhpSettingsSummary = {
    attach: function (context) {
      $('fieldset#edit-php', context).drupalSetSummary(function (context) {
        if (!$('textarea[name="php"]', context).val()) {
          return Drupal.t('Any page');
        }
        else {
          return Drupal.t('Using custom PHP code');
        }
      });
    }
  };
})(jQuery);
