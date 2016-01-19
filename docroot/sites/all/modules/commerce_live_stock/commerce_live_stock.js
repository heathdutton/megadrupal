/**
 * 
 */
(function ($) {
Drupal.Nodejs.callbacks.commerce_live_stock = {
    callback: function (message) {
      if(message.callback == 'commerce_live_stock') {
        if(qty = parseFloat($('.product-' + message.data.sku + ' .field-item').html())){
          var precision = message.data.precision;
          qty = qty - parseFloat(message.data.quantity);
          qty = parseFloat(qty).toFixed(precision);
          $('.product-' + message.data.sku + ' .field-item').html(qty);
          if(qty <= 0){
            $('#edit-submit').attr('disabled', 'disabled');
            $('#edit-submit').val('Out of stock');
            $('#edit-submit').addClass('form-button-disabled');
            $('#edit-submit').disabled = true;
          }
        }
       }
      }
};
})(jQuery);
