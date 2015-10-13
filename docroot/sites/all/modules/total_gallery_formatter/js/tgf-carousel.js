(function($) {
  Drupal.total_gallery_formatter = Drupal.total_gallery_formatter || {};
  Drupal.total_gallery_formatter.tgfToBoolean = function(number) {
    if (number) {
      return true;
    }
    else {
      return false;
    }
  };
  Drupal.behaviors.tgfCarousel = {
    attach : function(context, settings) {
      var carouselConfiguration = Drupal.settings.totalGalleryFormatter.carouselConfiguration;
      var itemsVisible = carouselConfiguration.itemsVisible;
      var scrollFx = carouselConfiguration.scrollFx;
      var direction = carouselConfiguration.direction;
      var slideDuration = carouselConfiguration.slideDuration;
      var autoplay = carouselConfiguration.autoplay;
      var circular = carouselConfiguration.circular;
      var infinite = carouselConfiguration.infinite;
      var colorbox = carouselConfiguration.colorbox;
      var easing = carouselConfiguration.easing;
      var responsive = carouselConfiguration.responsive;
      $('body').once('total_gallery_formatter', function() {
        var containerWidth = null;
        var items = parseInt(itemsVisible);
        if (responsive) {
          containerWidth = '100%';
          items = null;
        };
        $('.tgf-carousel-container', context).each(function(index){
          var $thisCarousel = $(this);
          $thisCarousel.find('.tgf-slides').carouFredSel({
            width : containerWidth,
            items : items,
            circular: Drupal.total_gallery_formatter.tgfToBoolean(circular),
            infinite: Drupal.total_gallery_formatter.tgfToBoolean(infinite),
            direction : direction,
            auto : Drupal.total_gallery_formatter.tgfToBoolean(autoplay),
            scroll : {
              items : 1,
              fx : scrollFx,
              easing : easing,
              duration : parseInt(slideDuration)
            },
            prev : {
              button : $('.tgf-prev-button', $thisCarousel),
              key : 'left'
            },
            next : {
              button : $('.tgf-next-button', $thisCarousel),
              key : 'right'
            },
          });
        });
        if (!$('.page-admin').length) {
          if (colorbox) {
            $('.tgf-slides a').colorbox({returnFocus:false, maxHeight:'98%', maxWidth:'98%', fixed: true});
          }
        }
      });
    }
  };
})(jQuery);
