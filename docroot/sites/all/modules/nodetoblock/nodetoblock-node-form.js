(function ($) {
  Drupal.behaviors.nodetoblockFieldsetSummaries = {
    attach: function (context) {
      $('fieldset.nodetoblock-node-settings-form', context).drupalSetSummary(function (context) {
        if ($('.form-item-nodetoblock-enable input:checked', context).length) {
          return Drupal.t('Enabled, view mode: @mode', {'@mode': $(".form-item-nodetoblock-view-mode select option:selected", context).text()});
        }
        else {
          return Drupal.t('Disabled');
        }
      });
    }
  };
})(jQuery);
