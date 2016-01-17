(function ($) {
  Drupal.behaviors.BrowseProductGrids = {
    attach: function(context) {
      $("#quicktabs-browse_products .browse-grid-item").equalHeight();
      $("#quicktabs-tabpage-browse_products_full-0").removeClass("quicktabs-hide");
      $("#quicktabs-browse_products_full .browse-grid-item").equalHeight();
      $("#quicktabs-tabpage-browse_products_full-0").addClass("quicktabs-hide");
    }
  }
})(jQuery);
