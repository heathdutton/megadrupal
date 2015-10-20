(function ($) {

Drupal.behaviors.hide_node_linksFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-hide-node-links', context).drupalSetSummary(function (context) {
      var status = $('#edit-hide-node-links-status option:selected').text();
      return Drupal.t('Visibility: @value', { '@value': status });
    });
  }
};

})(jQuery);