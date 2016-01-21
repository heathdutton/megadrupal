
(function ($) {

Drupal.behaviors.submenutreeFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.node-form-submenutree-settings', context).drupalSetSummary(function (context) {
      var submenuCheckbox = $('.form-item-submenutree-enable input', context);
      var siblingmenuCheckbox = $('.form-item-siblingmenutree-enable input', context);

      if (submenuCheckbox.is(':checked') && siblingmenuCheckbox.is(':not(:checked)')) {
        return Drupal.t('Sub content');
      }
      
      if (siblingmenuCheckbox.is(':checked') && submenuCheckbox.is(':not(:checked)')) {
        return Drupal.t('Sibling content');
      }
      
      if (submenuCheckbox.is(':checked') && siblingmenuCheckbox.is(':checked')) {
        return Drupal.t('Sub content and sibling content');
      }

      return Drupal.t('No sub content or sibling content');

    });
  }
};

})(jQuery);
