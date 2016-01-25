(function ($) {
  "use strict";

  Drupal.behaviors.viewsSupertable = {
    attach: function (context) {
      if (typeof $.fn.quicksearch === 'undefined') { return; }

      $("input#edit-id-search", context).once('views-supertable', function() {
        $(this).quicksearch("table tbody tr", {
          noResults: '#noresults',
          stripeRows: ['odd', 'even'],
          loader: 'span.loading'
        });
      });
    }
  };

})(jQuery);
