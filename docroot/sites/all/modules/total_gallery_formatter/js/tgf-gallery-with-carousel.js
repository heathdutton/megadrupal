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
  Drupal.behaviors.totalGalleryFormatter = {
    attach : function(context, settings) {
      var carouselConfiguration = Drupal.settings.totalGalleryFormatter.galleryCarouselConfiguration;
      var itemsVisible = carouselConfiguration.itemsVisible;
      var scrollFx = carouselConfiguration.scrollFx;
      var direction = carouselConfiguration.direction;
      var slideDuration = carouselConfiguration.slideDuration;
      var autoplay = carouselConfiguration.autoplay;
      var circular = carouselConfiguration.circular;
      var infinite = carouselConfiguration.infinite;
      var colorbox = carouselConfiguration.colorbox;
      var easing = carouselConfiguration.easing;
      var pagDuration = carouselConfiguration.pagDuration;
      var responsive = carouselConfiguration.responsive;
      $('body').once('total_gallery_formatter', function() {
        // Pagination.
        var pagWidth = null;
        var pagItems = parseInt(itemsVisible);
        // Getting main image size.
        var mainImgWidth = null;
        var mainImgHeight = null;
        if (responsive) {
          mainImgWidth = parseInt($('.tgf-container .tgf-slides img:first', context).attr('width'));
          mainImgHeight = parseInt($('.tgf-container .tgf-slides img:first', context).attr('height'));
          var mainImgHeightPercent = (mainImgHeight * 100) / mainImgWidth;
          mainImgHeight = mainImgHeightPercent + '%';
          // Responsive pagination.
          pagWidth = '100%';
          pagItems = null;
        };
        // Building gallery.
        $('.tgf-container', context).each(function(index){
          var $thisCarousel = $(this);
          $thisCarousel.find('.tgf-slides').carouFredSel({
            responsive: Drupal.total_gallery_formatter.tgfToBoolean(responsive),
            items    : {
              visible : 1,
              width : mainImgWidth,
              height : mainImgHeight
            },
            align : 'center',
            direction : direction,
            auto : Drupal.total_gallery_formatter.tgfToBoolean(autoplay),
            scroll : {
              fx : scrollFx,
              duration : parseInt(slideDuration),
              onAfter : function(data) {
                var slideId = $(data.items.visible[0]).attr('data-slide-id');
                $thisCarousel.find('.tgf-pagination .tgf-pag-item').removeClass('selected');
                $thisCarousel.find('.tgf-pagination .tgf-pag-item[data-slide-id="' + slideId + '"]').addClass('selected');
              }
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
          $thisCarousel.find('.tgf-pagination').carouFredSel({
            width : pagWidth,
            items    : {
              visible : pagItems
            },
            circular: Drupal.total_gallery_formatter.tgfToBoolean(circular),
            infinite: Drupal.total_gallery_formatter.tgfToBoolean(infinite),
            auto  : false,
            scroll : {
              easing : easing,
              duration : parseInt(pagDuration),
            },
            prev : {
              button : $thisCarousel.find('.tgf-pag-prev-button')
            },
            next : {
              button : $thisCarousel.find('.tgf-pag-next-button'),
            }
          });
          // Setting the selected class.
          $thisCarousel.find('.tgf-pagination .tgf-pag-item').each(function(index) {
            $(this).click(function(e) {
              var slideId = $(this).attr('data-slide-id');
              $thisCarousel.find('.tgf-slides').trigger("slideTo", '.tgf-slide-item[data-slide-id="' + slideId + '"]');
              $thisCarousel.find('.tgf-pagination .tgf-pag-item').removeClass('selected');
              $(this).addClass('selected');
            });
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
