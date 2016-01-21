/**
 * @file
 * Script to filter modules in module list.
 */

(function ($) {
  /**
   * Provide the summary information for the block_per_menu settings vertical tab.
   */
  Drupal.behaviors.blockMenuSettingsSummary = {
    attach: function (context) {
      $('fieldset#edit-menu', context).drupalSetSummary(function (context) {
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
