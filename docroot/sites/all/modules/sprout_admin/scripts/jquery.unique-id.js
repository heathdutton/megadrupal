/**
 * @file
 * Generate unique session-level html ids
 */

;( function( $, undefined ) {
  'use strict';

  var uuid = 0;

  $.fn.uniqueId = function(prefix) {
    return this.each(function() {
      if (!this.id) {
        this.id = prefix + '-id-' + (++uuid);
      }
    });
  };

})( jQuery );
