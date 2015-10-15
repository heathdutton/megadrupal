(function ($) {

  // Provide new command for Drupal's Ajax framework.
  Drupal.ajax.prototype.commands.commerceProductUrlsUpdateUrl = function(ajax, response, status) {
    // Build new query string with updated product identifier.
    var new_query_string;
    var pattern = new RegExp('([?|&])(id|sku)=.*?(&|$)', 'i');
    // Product identifier is already in the URL, it needs to be replaced.
    if (location.search.match(pattern)) {
      new_query_string = location.search.replace(pattern, '$1' + response.data + '$3');
    }
    // Product identifier is not in the URL yet, it needs to be added.
    else {
      new_query_string = '?' + response.data;
    }

    // If HTML5 history.pushState() exists...
    if (window.history && window.history.pushState) {
      history.pushState(null, document.title, new_query_string);
    }
    // ...otherwise just update URL by changing location.search value
    // (which will reload current page with new search params).
    else if (Drupal.settings.commerceProductURLs.updateURLFallback) {
      location.search = new_query_string;
    }
  };

})(jQuery);

