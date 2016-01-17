
(function ($) {

Drupal.behaviors.multisubmitFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.multi-submit-node-settings-form', context).drupalSetSummary(function(context) {
      if ($('[name="multi_submit_enable"]', context).is(':not(:checked)')) {
        return Drupal.t('Disabled');
      }
      var vals = [];

      vals.push(Drupal.t('Enabled'));


      var multiple = $('[name="multi_submit_multiple"]', context).val();
      if (multiple) {
        vals.push(Drupal.t('Multiple field: @multiple', {'@multiple': multiple}));
      }

      var single = $('[name="multi_submit_single"]', context).val();
      if (single) {
        vals.push(Drupal.t('Single field: @single', {'@single': single}));
      }

      return vals.join(', ');
    });
  }
};

})(jQuery);
