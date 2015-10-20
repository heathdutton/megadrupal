(function ($) {

  Drupal.behaviors.commerce_product_urls = {
    attach: function(context, settings) {
      // On full Product Display pages, let's update the URL when the page first
      // loads.
      $('.commerce-product-url-product-display').once('commerce-product-url-product-display', function() {
        update_url(Drupal.settings.commerceProductURLs.product_id);
        // No need for this class anymore once the item has been processed.
        $(this).removeClass('commerce-product-url-product-display');
      });
    }
  };

  // Provide new command for Drupal's Ajax framework.
  Drupal.ajax.prototype.commands.commerceProductUrlsUpdateUrl = function(ajax, response, status) {
    update_url(response.data);
  };

  function update_url(queryString) {
    // Build new query string with updated product identifier.
    var newQueryString;
    var pattern = new RegExp('([?|&])(id|sku)=.*?(&|$)', 'i');
    // Product identifier is already in the URL, it needs to be replaced.
    if (location.search.match(pattern)) {
      newQueryString = location.search.replace(pattern, '$1' + queryString + '$3');
    }
    // Product identifier is not in the URL yet, it needs to be added.
    else {
      newQueryString = '?' + queryString;
    }

    // If HTML5 history.pushState() exists...
    if (window.history && window.history.pushState) {
      history.pushState(null, document.title, newQueryString);
    }
    // ...otherwise just update URL by changing location.search value
    // (which will reload current page with new search params).
    else if (Drupal.settings.commerceProductURLs.updateURLFallback) {
      location.search = newQueryString;
    }
  }

})(jQuery);

