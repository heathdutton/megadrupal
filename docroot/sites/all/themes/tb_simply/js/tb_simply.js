(function ($) {
Drupal.behaviors.actionTBSimply = {
  attach: function (context) {
    window.setTimeout(function() {
      $('#main-content .grid-inner:first, .region-sidebar-first').matchHeights();
    }, 100);
  }
};
})(jQuery);
