(function ($) {
  /**
   * Views Slideshow Caroufredsel
   */

  // Add views slieshow api calls for views slideshow caroufredsel.
  Drupal.behaviors.viewsSlideshowCaroufredsel = {
    attach: function (context) {
      // Process pause on hover.
      $('.views-slideshow-caroufredsel-main-frame:not(.views-slideshow-caroufredsel-processed)', context).addClass('views-slideshow-caroufredsel-processed').each(function() {
        // Parse out the unique id from the full id.
        var vss_id = $(this).attr('id');
        var settings = Drupal.settings.viewsSlideshowcaroufredsel[vss_id];

        // The settings object will be passed to the carouFredSel plugin as
        // a configuration object. We must prepare a cople of things before
        delete settings.vss_id;
        delete settings.num_divs;

        // Let viewsSlideshow know our transition events so the rest of widgets
        // can react properly.
        // @todo Provide integration for views_slideshow without destroynig user entered configuration
        settings.scroll.onBefore = function(data) {
          if (data.items.visible.attr('id') == null) return;  // @fixme if items.start is set data.items.visible is NOT a jquery object the first time its ran.
          var slideNum = parseInt(data.items.visible.attr('id').replace(vss_id+'_', ''));
          Drupal.viewsSlideshow.action({ "action": 'transitionBegin', "slideshowID": vss_id, "slideNum": slideNum });
        };
        settings.scroll.onAfter = function(data) {
          if (data.items.visible.attr('id') == null) return; // @fixme if items.start is set data.items.visible is NOT a jquery object the first time its ran.
          var slideNum = parseInt(data.items.visible.attr('id').replace(vss_id+'_', ''));
             Drupal.viewsSlideshow.action({ "action": 'transitionEnd', "slideshowID": vss_id, "slideNum": slideNum });
        },
        $(this).carouFredSel(settings);
        // @todo provide settings for all these http://caroufredsel.dev7studios.com/code-examples/configuration.php
      });
    }
  }

  Drupal.viewsSlideshowCaroufredsel = Drupal.viewsSlideshowCaroufredsel || {};

  /**
  * Implement the nextSlide hook for carouselfredsel
  */
  Drupal.viewsSlideshowCaroufredsel.nextSlide = function(options) {
    $('#'+options.slideshowID).trigger('next', 1);
  }

  /**
  * Implement the prevSlide hook for carouselfredsel
  */
  Drupal.viewsSlideshowCaroufredsel.previousSlide = function(options) {
    $('#'+options.slideshowID).trigger('prev', 1);
  }

  /**
  * Implement the pause hook for carouselfredsel
  */
  Drupal.viewsSlideshowCaroufredsel.pause = function(options) {
    $('#'+options.slideshowID).trigger('pause');
  }

  /**
  * Implement the pause hook for carouselfredsel
  */
  Drupal.viewsSlideshowCaroufredsel.play = function(options) {
    $('#'+options.slideshowID).trigger('play');
  }
  /**
  * Implement the goToSlide hook for carouselfredsel
  */
  Drupal.viewsSlideshowCaroufredsel.goToSlide = function(options) {
    $('#'+options.slideshowID).trigger('slideTo', $('#'+options.slideshowID+'_'+options.slideNum));
  }
})(jQuery);
