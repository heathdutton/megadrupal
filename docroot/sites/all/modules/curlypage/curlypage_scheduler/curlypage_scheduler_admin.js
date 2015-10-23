(function ($) {

/**
 * Provide the summary information for the curlypage settings vertical tabs.
 */
Drupal.behaviors.curlypageSchedulerSettingsSummary = {
  attach: function (context) {
    // The drupalSetSummary method required for this behavior is not available
    // on the Curlypages administration page, so we need to make sure this
    // behavior is processed only if drupalSetSummary is defined.
    if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
      return;
    }

    $('fieldset[id*="schedules"]').drupalSetSummary(function (context) {
      var vals = [];

      var enable_on = $('input[name*="enable_on"]', context).val();
      if (enable_on) {
        vals.push(Drupal.t('Enable on: @value', { '@value': enable_on }));
      }

      var disable_on = $('input[name*="disable_on"]', context).val();
      if (disable_on) {
        vals.push(Drupal.t('Disable on: @value', { '@value': disable_on }));
      }

      var repeat = $('input[name*="repeat_enable"]:checked', context).val();
      if (repeat) {
        var days = $('input[name*="repeat_days"]', context).val();
        var hours = $('input[name*="repeat_hours"]', context).val();
        vals.push(Drupal.t('Repeat every: @value1 days @value2 hours', { '@value1': days , '@value2': hours }));
        var until = $('input[name*="repeat_until"]', context).val();
        if (until) {
          vals.push(Drupal.t('Until: @value', { '@value': until }));
        }
      }

      return vals.join('<br />');
    });

  }
};

})(jQuery);
