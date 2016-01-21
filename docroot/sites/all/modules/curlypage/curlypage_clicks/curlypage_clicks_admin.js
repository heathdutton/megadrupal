(function ($) {

/**
 * Provide the summary information for the curlypage settings vertical tabs.
 */
Drupal.behaviors.curlypageClicksSettingsSummary = {
  attach: function (context) {
    // The drupalSetSummary method required for this behavior is not available
    // on the Curlypages administration page, so we need to make sure this
    // behavior is processed only if drupalSetSummary is defined.
    if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
      return;
    }

    $('fieldset#edit-tracking-settings', context).drupalSetSummary(function (context) {

      var vals = [];

      if (!$('input[name="clicks_enabled"]:checked', context).val()) {
        return Drupal.t('Disabled');
      }

      vals.push(Drupal.t('Enabled'));

      var max_close = $('input[name="max_views_close"]', context).val();
      if (max_close) {
        vals.push(Drupal.t('Max views close: @value', { '@value': max_close }));
      }

      var max_open = $('input[name="max_views_open"]', context).val();
      if (max_open) {
        vals.push(Drupal.t('Max views open: @value', { '@value': max_open }));
      }

      return vals.join('<br />');
    });

  }
};

})(jQuery);
