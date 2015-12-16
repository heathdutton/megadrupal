/**
 * Override the built-in autocomplete search
 */
Drupal.behaviors.autocomplete_post = (function() {

  "use strict";

  var applied = false;

  return {
    attach: function (context, settings) {
      if (typeof Drupal.ACDB != 'undefined' && !applied) {
        Drupal.ACDB.prototype.search = function(searchString) {
          var db = this;
          db.searchString = searchString;

          // See if this key has been searched for before
          if (db.cache[searchString]) {
            return db.owner.found(db.cache[searchString]);
          }

          // Initiate delayed search
          if (db.timer) clearTimeout(db.timer);
          db.timer = setTimeout(function() {
            db.owner.setStatus('begin');
            // Ajax POST request for autocompletion
            jQuery.ajax({
              type: 'POST',
              url: db.uri,
              data: {search: searchString},
              dataType: 'json',
              success: function (matches) {
                if (typeof matches['status'] == 'undefined' || matches['status'] != 0) {
                  db.cache[searchString] = matches;
                  // Verify if these are still the matches the user wants to see
                  if (db.searchString == searchString) {
                    db.owner.found(matches);
                  }
                  db.owner.setStatus('found');
                }
              },
              error: function (xmlhttp) {
                alert(Drupal.ahahError(xmlhttp, db.uri));
              }
            });
          }, db.delay);
        }; // search
        applied = true;
      } // applied
    } // attach
  }; // return obj

})();
