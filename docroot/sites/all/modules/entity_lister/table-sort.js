(function($) {
  "use strict";
  Drupal.behaviors.entity_lister = {
    attach: function(context, settings) {

      var table_sort = settings.entity_lister.table_sort;
      var url_sort = parseQS('sort', window.location.search);

      if (url_sort) {

        var label = decodeURIComponent(parseQS('order', window.location.search));

        for (var c in table_sort) {
          if (label === table_sort[c]['data']) {
            var key = c;
            var sort = url_sort;
            $('.entity-lister th#' + c).addClass('sort-active');
          }
          else {
            if ($('.entity-lister th#' + c).hasClass('sort-active')) {
              $('.entity-lister th#' + c).removeClass('sort-active');
            }
          }
        }

      } else {
        for (var c in table_sort) {
          if ('undefined' !== typeof table_sort[c]['sort']) {
            var key = c;
            $('.entity-lister th#' + c).addClass('sort-active');
            var sort = table_sort[c]['sort'];
            var order = table_sort[c]['data'];
          }
        }
      }

      $('th span.asc').hide();
      $('th span.desc').hide();

      if ('undefined' !== typeof key) {

        $('th#' + key + ' span.' + sort.toLowerCase()).show();
        $('th#' + key + ' span.' + sort.toLowerCase()).click(function() {

          if (url_sort) {
            sort = parseQS('sort', window.location.search);
          }

          if ('desc' == sort.toLowerCase()) {
            var revsort = 'asc';
          } else {
            var revsort = 'desc';
          }

          if (url_sort) {
            var reverse = window.location.href.replace(sort, revsort);
          }
          else {
            var reverse = window.location.href + '?order=' + order + '&sort=' + revsort;
          }
          window.location = reverse;

        });
      }

    }
  }

})(jQuery);
