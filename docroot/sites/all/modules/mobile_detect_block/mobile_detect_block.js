(function ($) {

/**
 * Provide the summary information for the mobile_detect Block settings
 * vertical tab.
 */
Drupal.behaviors.MobileDetectBlockSettingsSummary = {
  attach: function (context) {

    $('fieldset#edit-mobile-detect-block', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="radio"]:checked', context).each(function () {
        vals.push($.trim($(this).next('label').text()));
      });
      if (!vals.length) {
        vals.push(Drupal.t('Not restricted'));
      }
      return vals.join(', ');
    });

  }
};

})(jQuery);
