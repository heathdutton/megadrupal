jQuery.noConflict(); if (typeof(window.$) === 'undefined') { window.$ = jQuery; }
(function($) {
  Drupal.behaviors.photowall = {
    attach: function (context) {
      var photowall = Drupal.settings.photowall;
      PhotoWall.init({
        el: '#photowall',  // Gallery element.
        zoom: true,  // Use zoom.
        zoomAction: 'mouseenter',  // Zoom on action.
        zoomTimeout: 500,  // Timeout before zoom.
        zoomImageBorder: 5,  // Zoomed image border size.
        zoomDuration: 100,  // Zoom duration time.
        showBox: true, // Enable fullscreen mode.
        showBoxSocial: true,  // Show social buttons.
        padding: 5,  // padding between images in gallery.
        lineMaxHeight: 150, // Max set height of pictures line
        lineMaxHeightDynamic: false,  // Dynamic lineMaxHeight.
        baseScreenHeight: 600  // Base screen size from wich calculating dynamic lineMaxHeight.
      });
      PhotoWall.load(photowall);
    }
  }
})(jQuery);

