(function($) {

Drupal.behaviors.commerceKlarna = {
  attach: function (context, settings) {

    if (!$('#klarna-invoice-terms').length) {
      // No placeholder element available, bail out early.
      return;
    }

    if (Drupal.settings.commerceKlarna.loaded && !Drupal.settings.commerceKlarna.loading) {
      // The external Klarna library is already loaded, initiate the terms.
      commerceKlarnaInitTerms();
    }
    else if (!Drupal.settings.commerceKlarna.loading) {
      // Load the external script from Klarna. When it has loaded properly,
      // we'll initiate the invoice terms.
      Drupal.settings.commerceKlarna.loading = true;
      $.getScript('http://static.klarna.com/external/toc/v1.0/js/klarna.terms.min.js', function(data, status) {
        Drupal.settings.commerceKlarna.loading = false;
        Drupal.settings.commerceKlarna.loaded = true;
        commerceKlarnaInitTerms();
      });
    }
    
  }
}

/**
 * Initiate the terms and bind it to the placeholder element.
 */
commerceKlarnaInitTerms = function() {
  if (!$('#klarna-invoice-terms').hasClass('initiated')) {
    // Initiate the terms using Klarna.
    new Klarna.Terms.Invoice({  
      el: 'klarna-invoice-terms',
      eid: Drupal.settings.commerceKlarna.eStoreID,
      country: 'se',
      charge: Drupal.settings.commerceKlarna.charge
    });
  
    // Add a class so that the terms won't be initiated again.
    $('#klarna-invoice-terms').addClass('initiated');
  }
}

})(jQuery);