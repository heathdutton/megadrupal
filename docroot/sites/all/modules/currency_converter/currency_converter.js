(function ($) {

/**
 * Provide javascript features of currency converter block
 */
Drupal.behaviors.currency_converter = {
  attach: function (context, settings) {

    if (settings.currency_converter === undefined) {
      return;
    }

    var converter = settings.currency_converter;
    var $inputs = $('#currency_converter input[type=text]', context);

    $('#currency_converter input[type=text]', context).keyup(function() {

      var current_value = this.value;
      var current_index = $inputs.index(this);
      var currency = $(this).parents('.currency').attr('currency');
      var current_price = converter[currency];

      jQuery.each($inputs, function(i, val) {
        if (i != current_index) {
          var this_currency = $(this).parents('.currency').attr('currency');
          var this_price = converter[this_currency] != 0 ? converter[this_currency] : 1;
          if (current_price == 0) {
            val.value = (current_value / this_price).toFixed(2);
          }
          else {
            val.value = (current_value * current_price / this_price).toFixed(2);
          }
        }
      });		
    });
  }
}

})(jQuery);
