/**
 * @file
 * Provide javascript features of currency converter block
 */

(function ($) {
  function getConversion(from, to) {
    var path = "http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.xchange%20where%20pair%20in%20%28%22"+from+to+"%22%29&env=store://datatables.org/alltableswithkeys&format=json";
    $.ajax({
      url: path,
      type: 'GET',
      dataType: 'json',
      success: function(data) {
       if (data.query.results.rate.Rate && data.query.results.rate.Rate != 'N/A') {
        var rate = data.query.results.rate.Rate;
        var amount = $('.easy-currency-con-amount').val();
        $('.easy-currency-con-rate').val(rate * amount);
       }

      }
    });
  }
  Drupal.behaviors.easy_currency_con_form = {
    attach: function (context, settings) {
      $('.easy-currency-con-amount', context).once('currency-conversion', function() {
        $('.easy-currency-con-amount').keypress(function(e) {
          var number = String.fromCharCode(e.which);
          if(!number.match(/[0-9]/)) {
            alert('Only numeric value allowed');
            return false;
          }
          else {
            if ($('.easy-currency-con-from-input').val() && $('.easy-currency-con-to-input').val()) {
              getConversion($('.easy-currency-con-from-input').val(), $('.easy-currency-con-to-input').val());
            }
          }
        });
      });

      $("input.easy-currency-con-from-input", context).bind('autocompleteSelect', function(event) {
        if ($(this).val() && ($(this).val() != 'N/A') && $('.easy-currency-con-to-input').val()) {
          getConversion($(this).val(), $('.easy-currency-con-to-input').val());
        };
      });
      $("input.easy-currency-con-to-input", context).bind('autocompleteSelect', function(event) {
        if ($(this).val() != 'N/A') {
          getConversion($('.easy-currency-con-from-input').val(), $(this).val());
        }
      });


      $('.easy-currency-con-inverse-input', context).once('currency-conversion', function() {
          $('.easy-currency-con-inverse-input').click(function() {
            var from = $('.easy-currency-con-from-input').val();
            var to = $('.easy-currency-con-to-input').val();
            $('.easy-currency-con-from-input').val(to);
            $('.easy-currency-con-to-input').val(from);
            if (from && to) {
              getConversion(to, from);
            };
          });
        });
    }
  };
})(jQuery);
