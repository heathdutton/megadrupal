(function ($) {

Drupal.behaviors.magentoCustomers = {
  attach: function(context, settings) {
    if ($('.block-magento-customers', context).length == 0) {
      return;
    }
    var $wrapper = $('.block-magento-customers ul.magento-menu', context);
    $.ajax({
      url: '/magento/customer/cartcount',
      success: function(data) {
        var item_qty = parseInt(data.cart_count);
        $('li.first .count',$wrapper).html('(' + item_qty + ')');
      }
    });
  }
}

})(jQuery);


