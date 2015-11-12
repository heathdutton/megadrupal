(function ($) {
  $(document).ready(function() {
    var conf = {};
    $('form#ePay input:hidden').each(function(key, element) {
      if (!$(element).attr('name').match(/^form_.*/)) {
        conf[$(element).attr('name')] = $(element).val();
      }
    });
    if (conf.windowstate === '3' || conf.windowstate === 3) {
      $('form#ePay').submit();
    }
    else if (typeof PaymentWindow == 'function') {
      var conf = {};
      $('form#ePay input:hidden').each(function(key, element) {
        if (!$(element).attr('name').match(/^form_.*/)) {
          conf[$(element).attr('name')] = $(element).val();
        }
      });
      var pw = new PaymentWindow(conf);
      pw.open();
    }
  });
})(jQuery);