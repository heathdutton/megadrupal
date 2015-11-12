(function ($) {

/**
 * Provide the summary information for the lolspeak plugin's vertical tab.
 */
Drupal.behaviors.menutreeSettingsSummary = {
  attach: function (context) {
    $('fieldset.menutree-tab-pane', context).drupalSetSummary(function (context) {
      var vals = [];
      if (title = $('input.menutree-title', context).val()) {
        vals.push('“' + title + '”');
      }
      if ($('textarea.menutree-intro-text', context).val()) {
        vals.push(Drupal.t('Has intro text'));
      }
      if ($('select.menutree-weight', context).val() != '<none>') {
        vals.push(Drupal.t('Included in main index'));
      }
      if (vals.length) {
        return vals.join(', ');
      }
      else {
        return Drupal.t('Default settings');
      }
    });
  }
};

})(jQuery);
