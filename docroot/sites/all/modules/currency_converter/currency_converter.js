// $Id$
(function ($) {

/**
 * Provide javascript features of currency converter block
 */
Drupal.behaviors.currency_converter = {
  attach: function (context) {
    $('#currency_converter input[type=text]').keyup(function() {      
      var current_value = this.value;
      var current_index = $('#currency_converter input[type=text]').index(this);
      var current_price = $('#currency_converter span').eq(current_index).text() ? $('#currency_converter span').eq(current_index).text() : '1';
      jQuery.each($('#currency_converter input[type=text]'), function(i, val) {
        if (i != current_index) {
          this_price = $('#currency_converter span').eq(i).text() ? $('#currency_converter span').eq(i).text() : '1';
          val.value = (current_value * current_price / this_price).toFixed(2);
        }
      });		
    });
  }
}

})(jQuery);
