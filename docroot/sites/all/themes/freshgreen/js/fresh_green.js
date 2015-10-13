(function ($) {
  Drupal.behaviors.fresh_green = {
    attach: function(context) {
      $("table.zebra tr:not(:has(th)):even", context).addClass("even");
    }
  }
})(jQuery);
