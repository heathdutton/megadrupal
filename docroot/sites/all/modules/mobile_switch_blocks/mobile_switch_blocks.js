(function ($) {

/**
 * Provide the summary information for the Mobile Switch Blocks (Is Mobile)
 * settings vertical tab.
 */
Drupal.behaviors.MobileSwitchBlocksSettingsSummary = {
  attach: function (context) {

    $('fieldset#edit-mobile-switch-blocks', context).drupalSetSummary(function (context) {
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
