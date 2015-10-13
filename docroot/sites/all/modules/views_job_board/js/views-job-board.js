(function ($) {

  Drupal.behaviors.ViewsJobBord = {
    attach: function(context, settings) {
      var tables = $('table.views-job-board-style tbody', context);
      var rows;

      // Apply for each table.
      jQuery.each(tables, function() {
        rows = $('tr', this);

        // Clean zebra.
        rows.removeClass('odd even');

        // Shuffle thems.
        $(this).html(rows.sort(function() {
          return 0.5 - Math.random();
        }));

        // Move sticky item at top.
        $(this).prepend($('tr.sticky', this));

        // Add new zebra.
        jQuery.each($('tr', this), function(i, val) {
          $(this).addClass(i%2 ? "odd" : 'even');
        });
      });
    }
  }

})(jQuery);
