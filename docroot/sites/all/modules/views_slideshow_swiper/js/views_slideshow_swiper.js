(function ($) {
  // List arguments for all of Swiper's callback function parameters.
  Drupal.settings.viewsSlideshowSwiperValues = {
    'prefixId': 'views_slideshow_swiper_main_',
    'callbacks': {
      'onInit': ['swiper'],
      'onSlideChangeStart': ['swiper'],
      'onSlideChangeEnd': ['swiper'],
      'onTransitionStart': ['swiper'],
      'onTransitionEnd': ['swiper'],
      'onTouchStart': ['swiper', 'event'],
      'onTouchMove': ['swiper', 'event'],
      'onTouchMoveOpposite': ['swiper', 'event'],
      'onSliderMove': ['swiper', 'event'],
      'onTouchEnd': ['swiper', 'event'],
      'onClick': ['swiper', 'event'],
      'onTap': ['swiper', 'event'],
      'onDoubleTap': ['swiper', 'event'],
      'onImagesReady': ['swiper'],
      'onProgress': ['swiper', 'progress'],
      'onReachBeginning': ['swiper'],
      'onReachEnd': ['swiper'],
      'onDestroy': ['swiper'],
      'onSetTranslate': ['swiper', 'translate'],
      'onSetTransition': ['swiper', 'transition'],
      'onAutoplayStart': ['swiper'],
      'onAutoplayStop': ['swiper'],
      'onLazyImageLoad': ['swiper', 'slide', 'image'],
      'onLazyImageReady': ['swiper', 'slide', 'image'],
    },
    'callbackAdditions': {
      'onInit': function(swiper) {
        // Register callback to save references to Swiper instances. Allows
        // Views Slideshow controls to affect the Swiper.
        Drupal.viewsSlideshowSwiper.active = Drupal.viewsSlideshowSwiper.active || {};
        Drupal.viewsSlideshowSwiper.active[Drupal.settings.viewsSlideshowSwiper] = swiper;
      },
      // Trigger Slideshow's transition events when Swiper's transition events occur.
      'onTransitionStart': function(swiper) {
        Drupal.viewsSlideshow.action({
          'action': 'transitionBegin',
          'slideshowID': Drupal.settings.viewsSlideshowSwiper['#' + swiper.container.attr('id')].vss_id,
          'slideNum': swiper.activeIndex - 1 // Minus one as the active slide is not yet in focus.
        });
      },
      'onTransitionEnd': function(swiper) {
        Drupal.viewsSlideshow.action({
          'action': 'transitionEnd',
          'slideshowID': Drupal.settings.viewsSlideshowSwiper['#' + swiper.container.attr('id')].vss_id,
          'slideNum': swiper.activeIndex
        });
      },
      'onAutoplayStart': function(swiper) {
        Drupal.viewsSlideshow.action({
          'action': 'play',
          'slideshowID': Drupal.settings.viewsSlideshowSwiper['#' + swiper.container.attr('id')].vss_id
        });
      },
      'onAutoplayStop': function(swiper) {
        Drupal.viewsSlideshow.action({
          'action': 'pause',
          'slideshowID': Drupal.settings.viewsSlideshowSwiper['#' + swiper.container.attr('id')].vss_id
        });
      },
    }
  };
    

  // This is called when the page first loads to bootstrap Swiper.
  Drupal.behaviors.viewsSlideshowSwiper = {
    attach: function (context) {
      $('.views_slideshow_swiper_main:not(.views_slideshow_swiper-processed)', context).addClass('views_slideshow_swiper-processed').each(function() {
        // Get the ID of the slideshow
        var fullId = '#' + $(this).attr('id');

        // Create settings container
        var settings = Drupal.settings.viewsSlideshowSwiper[fullId];
        if ('autoplay' in settings.options && (settings.options['autoplay'] === 0 || settings.options['autoplay'] === '')) {
          settings.options.splice('autoplay', 1);
        }

        // Define function to create callback function objects from user-inputted function body strings.
        var createFunction = (function(args, body) {
          function F(args) {
            return Function.apply(this, args);
          }
          F.prototype = Function.prototype;

          return function(args, body) {
            return new F(args.concat(body));
          }
        })();

        // For each callback function, add to its function body a call to an additional function if it exists for that callback.
        Object.keys(Drupal.settings.viewsSlideshowSwiperValues.callbackAdditions).forEach(function(parameter) {
          // If the function body is empty, instantiate it as an empty string to be added to.
          if (!(parameter in settings.options)) {
            settings.options[parameter] = '';
          }
          // Derive the function call string parts.
          var functionName = "Drupal.settings.viewsSlideshowSwiperValues.callbackAdditions." + parameter;
          var parameterList = Drupal.settings.viewsSlideshowSwiperValues.callbacks[parameter].join(", ");
          // Add the function call to the callback function body.
          settings.options[parameter] += functionName + '(' + parameterList+ ');';
        });
        Object.keys(settings.options).filter(function(value) {
          return value in Drupal.settings.viewsSlideshowSwiperValues.callbacks;
        }).forEach(function(parameter) {
          // Get user-defined code that comprises the callback function body.
          var functionCode = settings.options[parameter];

          // Instantiate and add a callback function if there is a non-empty function body for that callback function parameter
          if (functionCode) {
            settings.options[parameter] = createFunction(Drupal.settings.viewsSlideshowSwiperValues.callbacks[parameter], functionCode);
          }
          else {
            delete settings.options[parameter];
          }
        });

        // Set that Swiper has yet to load.
        settings.loaded = false;

        // Finally, instantiate Swiper for this View.
        Drupal.viewsSlideshowSwiper.load(fullId);
      });
    }
  };

  // Initialize the swiperjs object
  Drupal.viewsSlideshowSwiper = Drupal.viewsSlideshowSwiper || {};

  // Load mapping from Views Slideshow to Swiper
  Drupal.viewsSlideshowSwiper.load = function(fullId) {
    var settings = Drupal.settings.viewsSlideshowSwiper[fullId];

    // Ensure the slider isn't already loaded
    if (!settings.loaded) {
      settings.swiper = new Swiper(fullId, settings.options);
      settings.loaded = true;
    }
  };

  // Define central action dispatcher to handle all accepted actions.
  Drupal.viewsSlideshowSwiper.action = function(options) {
    var settings = Drupal.settings.viewsSlideshowSwiper['#' + Drupal.settings.viewsSlideshowSwiperValues.prefixId + options.slideshowID];
    if (settings.loaded) {
      switch (options.action) {
        case 'goToSlide':
          settings.swiper.slideTo(options.slideNum);
          break;
        case 'nextSlide':
          settings.swiper.slideNext();
          break;
        case 'pause':
          settings.swiper.stopAutoplay();
          break;
        case 'play':
          settings.swiper.startAutoplay();
          break;
        case 'previousSlide':
          settings.swiper.slidePrev();
          break;
      }
    }
  }

  // Use a central action dispatcher for more normalised code.
  Drupal.viewsSlideshowSwiper.goToSlide = Drupal.viewsSlideshowSwiper.action;
  Drupal.viewsSlideshowSwiper.pause = Drupal.viewsSlideshowSwiper.action;
  Drupal.viewsSlideshowSwiper.play = Drupal.viewsSlideshowSwiper.action;
  Drupal.viewsSlideshowSwiper.nextSlide = Drupal.viewsSlideshowSwiper.action;
  Drupal.viewsSlideshowSwiper.previousSlide = Drupal.viewsSlideshowSwiper.action;
})(jQuery);
