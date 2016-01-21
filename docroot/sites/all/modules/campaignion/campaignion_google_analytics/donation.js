(function($) {
    Drupal.behaviors.campaignion_google_analytics_donation = {};
    Drupal.behaviors.campaignion_google_analytics_donation.attach = function(context, settings) {
      if (typeof ga === 'undefined') { return; }
      // guard against missing window.sessionStorage
      // this prevents errors, but degrades functionality on systems missing
      // sessionStorage
      if (typeof window.sessionStorage === 'undefined') {
        window.sessionStorage = {};
        window.sessionStorage['setItem'] = function () {};
        window.sessionStorage['getItem'] = function () {};
        window.sessionStorage['removeItem'] = function () {};
      }

      var config =  settings.campaignion_google_analytics;
      if (typeof config === "undefined") {
        return
      }

      // require the GA ecommerce only once (not needed to add again
      // on every ajax request)
      if (context === document) {
        ga('require', 'ec');
        // our default is EUR
        ga('set', '&cu', 'EUR');
      }

      // if some donation teasers have been rendered on the page
      // config.impressions will be set and we only have to send
      // them to GA as impressions
      if (typeof config.impressions !== "undefined") {
        $.each(config.impressions, function(i, imp) {
          ga('ec:addImpression', {
            'id': imp.nid,
            'name': imp.title + " ["+imp.lang+"]"
          });
        });
        ga('send', 'event', 'donation', 'impression');
      }

      // if there is a "view" in the config object, we know we are on
      // a donation
      if (typeof config.view !== "undefined") {
        var item = config.view;
        // set the currency according to the value configured in
        // paymethod_select
        ga('set', '&cu', config.view.currency);
        ga('ec:addProduct', {
          'id': item.nid,
          'name': item.title + " ["+item.lang+"]"
        });
        var pageNum = $('[name=\"details[page_num]\"]', context);
        var pageMax = $('[name=\"details[page_count]\"]', context);

        // reset purchase state
        sessionStorage.removeItem('sentDonationPurchase-'+item.nid);

        // on the first page we have an detail view
        if (sessionStorage.getItem('sentDonationView-'+item.nid) !== "1" && parseInt(pageNum.val(), 10) === 1) {
          ga('ec:setAction', 'detail');
          sessionStorage.setItem('sentDonationView-'+item.nid, '1');
          ga("send", "event", "donation", "view", item.title+" ["+item.nid+"]");
        }

        // on the second page we added a donation
        if (sessionStorage.getItem('sentDonationAdded-'+item.nid) !== "1" && parseInt(pageNum.val(), 10) === 2) {
          ga('ec:setAction', 'add');
          sessionStorage.setItem('sentDonationAdded-'+item.nid, '1');
          ga("send", "event", "donation", "add to cart", item.title+" ["+item.nid+"]");
        }

        // the third step begins our checkout
        if (sessionStorage.getItem('sentDonationCheckoutBegin-'+item.nid) !== "1" && parseInt(pageNum.val(), 10) === 3) {
          ga('ec:setAction', 'checkout',  {
            step: 1
          });
          ga("send", "event", "donation", "checkout", item.title+" ["+item.nid+"]");
        }

        // the last step is our last checkout step
        if (sessionStorage.getItem('sentDonationCheckoutLast-'+item.nid) !== "1" && parseInt(pageNum.val(), 10) === parseInt(pageMax.val(), 10)) {
          // bind on click of last step if there is an paymethod select form
          // submit is a complex option, as webform_ajax and clientside_validation
          // are involved
          $form = $('.webform-client-form #payment-method-all-forms', context).closest('form.webform-client-form', document);

          // the current webform page, does not contain a paymethod-selector.
          if ($form.length) {
            var form_id = $form.attr('id');
            var form_num = form_id.split('-')[3];
            var $button = $form.find('#edit-webform-ajax-submit-' + form_num);

            if ($button.length === 0) { // no webform_ajax.
              $button = $form.find('input.form-submit');
            }

            $button.click(function() {
              var $controller = $('#' + form_id + ' .payment-method-form:visible');
              var controller = $controller.attr('id');
              var $issuer = $('[name*=\"[issuer]\"]', $controller);
              var methodOption;
              if ($issuer.length > 0) {
                methodOption = controller + " [" + $issuer.val() + "]";
              } else {
                methodOption = controller;
              }

              ga('ec:setAction', 'checkout',  {
                step: 2,
                option: methodOption
              });
              sessionStorage.setItem('sentDonationCheckoutLast-'+item.nid, '1');
              ga("send", "event", "donation", "checkout", item.title+" ["+item.nid+"]");
            });
          }

        }
      }
    }
})(jQuery);
