
(function ($) {

Drupal.behaviors.productFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.ec-product-details').drupalSetSummary(function (context) {
      var text = $('#edit-ptype-description').val();
      return text ? Drupal.checkPlain(text) : Drupal.t('Not a product');
    });
  }
};

})(jQuery);