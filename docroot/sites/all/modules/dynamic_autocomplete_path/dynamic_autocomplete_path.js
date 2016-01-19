/**
 * @file
 * Defines a custom search mechanism that replaces the default autocomplete
 * system from drupal.
 */

(function($) {

Drupal.dynamic_autocomplete_path = {
  get_uri: function(db) {
    var uri = db.uri.replace(/%23/g, '#');
    var ids = uri.match(/#([^\/]+)/g);
    for (var i = ids.length - 1; i >= 0; i--) {
      var search = ids[i];
      var replace = $(search).val();

      // Join the items of multi-item form widgets.
      if (Object.prototype.toString.call(replace) === '[object Array]') {
        var delimiter = $(search).attr('delimiter');
        if (delimiter !== undefined) {
          replace = replace.join(delimiter);
        }
        else {
          replace = replace.join(',');
        }
      }

      uri = uri.replace(search, replace);
    };
    // Trim the last slash in the URL
    uri = uri.substring(0, uri.length -1);
    return uri;
  }
}


$(document).ready(function() {

// Not all the pages have autocomplete widgets
if (Drupal && Drupal.ACDB) {

/**
 * Performs a cached and delayed search.
 */
Drupal.ACDB.prototype.search = function (searchString) {
  var db = this;
  this.searchString = searchString;

  // See if this string needs to be searched for anyway.
  searchString = searchString.replace(/^\s+|\s+$/, '');
  if (searchString.length <= 0 ||
    searchString.charAt(searchString.length - 1) == ',') {
    return;
  }

  // Find for replacement tokens inside the url
  uri = db.uri;
  if (uri.indexOf('/%23') != -1) {
    uri = Drupal.dynamic_autocomplete_path.get_uri(db);
  }
  else {
    // See if this key has been searched for before. Only valid for static uri
    if (this.cache[searchString]) {
      return this.owner.found(this.cache[searchString]);
    }
  }

  // Initiate delayed search.
  if (this.timer) {
    clearTimeout(this.timer);
  }
  this.timer = setTimeout(function () {
    db.owner.setStatus('begin');

    // Ajax GET request for autocompletion. We use Drupal.encodePath instead of
    // encodeURIComponent to allow autocomplete search terms to contain slashes.
    $.ajax({
      type: 'GET',
      url: uri + '/' + Drupal.encodePath(searchString),
      dataType: 'json',
      success: function (matches) {
        if (typeof matches.status == 'undefined' || matches.status != 0) {
          db.cache[searchString] = matches;
          // Verify if these are still the matches the user wants to see.
          if (db.searchString == searchString) {
            db.owner.found(matches);
          }
          db.owner.setStatus('found');
        }
      },
      error: function (xmlhttp) {
        alert(Drupal.ajaxError(xmlhttp, uri));
      }
    });
  }, this.delay);
};

}

});

})(jQuery);
