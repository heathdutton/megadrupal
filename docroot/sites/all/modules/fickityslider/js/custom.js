/**
 * @file
 * slider flickity funciton.
 */

(function($) {
  Drupal.behaviors.flickity = {
    attach: function(context, settings) {
      var autoplay = settings.custom.autoplay;
      if(autoplay) {
        var autoplaytime = parseInt(settings.custom.autoplaytime);
      } else {
        autoplaytime = false;
      }
      
      var prev_next = settings.custom.prev_next;
      if(prev_next) {
        var prevnextbuttons = true; 
      } else {
        prevnextbuttons = false;
      }
      
      var gallery = $('.gallery').flickity({
        
        cellSelector: 'img',
        imagesLoaded: true,
        percentPosition: false,
        autoPlay: autoplaytime,
        prevNextButtons: prevnextbuttons
      });
    }
  };
})(jQuery);
