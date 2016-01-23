/**
 * Override the built-in autocomplete search
 */
Drupal.behaviors.autocomplete_hack = (function() {

  "use strict";

  var applied = false;

  return {
    attach: function (context, settings) {
      if (typeof Drupal.ACDB != 'undefined') {
        var acdb = Drupal.ACDB.prototype;
        acdb._search = acdb.search;
        acdb.search = function(searchString) {
          this._search(searchString.replace(/\//g, '~$~$~$~'));
        };
        applied = true;
      }
    }
  };

})();
