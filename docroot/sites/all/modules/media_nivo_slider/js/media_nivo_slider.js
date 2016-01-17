/**
 * @file
 *  Applies the nivo slider functionality to the drupal blocks.
 */
 
(function ($) {
  Drupal.behaviors.mediaNivoSlider = {
    attach: function (context, settings) {
      // Iterate over all nivo sliders available via the Drupal javascript settings.
      for (var key in Drupal.settings.media_nivo_slider) {
        var slider = Drupal.settings.media_nivo_slider[key];
        
        // Set the block width, corrects the width setting when using varied image styles.
        var imageWidth = $("#" + key + "-media-nivo-slider").find('img:first').width();
        $("#" + key + "-media-nivo-slider").css('width', imageWidth);

        // Apply nivo slider to block
        $("#" + key + "-media-nivo-slider").nivoSlider({
          effect: slider.effect, //Specify sets like: 'fold,fade,sliceDown'
          slices: parseInt(slider.slices),
          boxCols: parseInt(slider.boxCols), // For box animations
          boxRows: parseInt(slider.boxRows),
          animSpeed: parseInt(slider.animSpeed), //Slide transition speed
          pauseTime: parseInt(slider.pauseTime),
          startSlide: parseInt(slider.startSlide), // Set starting Slide (0 index)
          directionNav: slider.directionNav, // Next & Prev navigation
          directionNavHide: slider.directionNavHide, // Only show on hover
          controlNav: slider.controlNav, // 1,2,3... navigation
          controlNavThumbs: slider.controlNavThumbs, // Use thumbnails for Control Nav
          controlNavThumbsFromRel: slider.controlNavThumbsFromRel, // Use image rel for thumbs
          controlNavThumbsSearch: slider.controlNavThumbsSearch, // Replace this with...
          controlNavThumbsReplace: slider.controlNavThumbsReplace, // ...this in thumb Image src
          keyboardNav: slider.keyboardNav, // Use left & right arrows
          pauseOnHover: slider.pauseOnHover, // Stop animation while hovering
          manualAdvance: slider.manualAdvance, // Force manual transitions
          captionOpacity: slider.captionOpacity, // Universal caption opacity
          prevText: slider.prevText, // Prev directionNav text
          nextText: slider.nextText, // Next directionNav text
          randomStart: slider.randomStart, // Start on a random slide
          beforeChange: new Function('', JSON.parse(slider.beforeChange)), // Triggers before a slide transition
          afterChange: new Function('', JSON.parse(slider.afterChange)), // Triggers after a slide transition
          slideshowEnd: new Function('', JSON.parse(slider.slideshowEnd)), // Triggers after all slides have been shown
          lastSlide: new Function('', JSON.parse(slider.lastSlide)), // Triggers when last slide is shown
          afterLoad: new Function('', JSON.parse(slider.afterLoad)) // Triggers when slider has loaded
        });        
      }
    }
  };

}(jQuery));
