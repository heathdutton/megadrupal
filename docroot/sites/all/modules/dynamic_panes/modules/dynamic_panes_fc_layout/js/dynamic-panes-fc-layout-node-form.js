(function ($) {
  Drupal.behaviors.dynamicPanesFcLayoutFieldsetSummaries = {
    attach: function (context) {
      // Provide the summary for the node type form.
      $('fieldset.dynamic-panes-fc-layout-node-type-settings-form', context).drupalSetSummary(function(context) {
        var vals = [];

        // Threading.
        var enabled = $(".form-item-dynamic-panes-fc-layout-enabled input:checked", context).next('label').text();
        if (enabled) {
          vals.push(enabled);
        }

        return Drupal.checkPlain(vals.join(', '));
      });
    }
  };
})(jQuery);
