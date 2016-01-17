(function ($) {

    Drupal.LR = Drupal.LR || {};
    Drupal.behaviors.LR = {
        attach: function(context, settings) {
            if($('#lr-balance').length > 0) {
                var balance = $('#lr-balance');
                $.ajax({
                    method: 'GET',
                    url: settings.basePath + 'lr/balance',
                    success: function(data) {
                        balance.text(balance.text() + ' ' + data['balance'] + ' ' + data['currency']);
                    },
                    error: function(data) {
                        balance.text(balance.text() + " error!");
                    }
                });
            }
        }
    }

})(jQuery);