(function($) {
  $(window).load(function() {
    // The slider being synced must be initialized first
    $('#carousel').flexslider({
      animation: "slide",
      controlNav: false,
      animationLoop: false,
      slideshow: false,
      itemWidth: Drupal.settings.FlexGalleryFormatter.thumbWidth,
    });
  });
})(jQuery);
