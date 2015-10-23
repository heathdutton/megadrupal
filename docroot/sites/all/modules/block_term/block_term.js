(function ($) {

/**
 * Provide the summary information for the block_term settings vertical tab.
 */
Drupal.behaviors.blockTermSettingsSummary = {
  attach: function (context) {

    $('fieldset#edit-terms', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="checkbox"]:checked', context).each(function () {
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
