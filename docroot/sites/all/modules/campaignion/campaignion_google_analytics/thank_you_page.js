(function($) {
    Drupal.behaviors.campaignion_google_analytics = {};
    Drupal.behaviors.campaignion_google_analytics.attach = function(context, settings) {
        if (typeof ga === 'undefined') { return; }
        var config =  settings.campaignion_google_analytics;
        if (typeof config !== 'undefined') {
            if (typeof config.thank_you !== "undefined") {
                ga("send", "event", "webform", "submitted", config.title+" ["+config.nid+"]");
            }
            if (typeof config.payment !== 'undefined') {
              if (sessionStorage.getItem('sentDonationPurchase-'+config.nid) !== '1') {
                ga('require', 'ec');
                ga('set', '&cu',  config.payment.currency);
                ga('ec:addProduct', {
                  id: config.nid,
                  name: config.title + " ["+config.lang+"]",
                  price: config.payment.price,
                  category: config.payment.category,
                  quantity: 1
                });
                ga('ec:setAction', 'purchase',  {
                  id: config.payment.id,
                  currency: config.payment.currency,
                  revenue: config.payment.price
                });
                ga("send", "event", "donation", "purchase", config.title+" ["+config.nid+"]");

                sessionStorage.setItem('sentDonationPurchase-'+config.nid, '1');
                // reset state
                sessionStorage.removeItem('sentDonationCheckoutBegin-'+config.nid);
                sessionStorage.removeItem('sentDonationCheckoutLast-'+config.nid);
                sessionStorage.removeItem('sentDonationView-'+config.nid);
                sessionStorage.removeItem('sentDonationAdd-'+config.nid);
              }
            }
        }
    }
})(jQuery);
