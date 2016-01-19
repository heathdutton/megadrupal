(function($) {
  $(window).load(function() {
    // The slider being synced must be initialized first
    $('#slider').flexslider({
      animation: "slide",
      controlNav: false,
      animationLoop: false,
      slideshow: false,
      //controlNav: false,
      directionNav: Drupal.settings.FlexGalleryFormatter.slideNavArrows,
    });
  });
})(jQuery);
