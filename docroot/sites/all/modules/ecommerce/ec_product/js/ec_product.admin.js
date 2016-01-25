
(function ($) {

Drupal.behaviors.productFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.ec-product-node-type-settings-form').drupalSetSummary(function (context) {
      var enabled = $('#edit-ec-product-convert').val();
      if (enabled != '0') {
        var vals = [];
        $('#edit-ec-product-ptypes input:checked')
          .each(function () {
            var label = $(this).next('label').clone();
            $('dt', label).remove();
            vals.push($(label).text());
          });
        return Drupal.t('Product types') + ' ' + vals.join(', ');
      }
      else {
        return Drupal.t('Product not enabled.');
      }
    });
  }
};

})(jQuery);