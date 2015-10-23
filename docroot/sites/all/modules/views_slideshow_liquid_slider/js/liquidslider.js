/**
 * @file liquidslider.js
 */
(function ($) {
  Drupal.behaviors.viewsSlideshowLiquidSlider = {
    attach: function (context, settings) {
      var fullId;
      var slideshowSettings;
      var slideshowContainer;

      $('.views_slideshow_liquid_slider_main:not(.viewsSlideshowLiquidSlider-processed)', context).addClass('viewsSlideshowLiquidSlider-processed').each(function() {
        // The id of the slider.
        fullId = '#' + $(this).attr('id');
        slideshowSettings = settings.viewsSlideshowLiquidSlider[fullId];

        slideshowSettings.autoHeight = (slideshowSettings.autoheight_settings.autoHeight == 1) ? true : false;
        slideshowSettings.autoHeightMin = parseInt(slideshowSettings.autoheight_settings.autoHeightMin);
        slideshowSettings.autoHeightEaseDuration = parseInt(slideshowSettings.autoheight_settings.autoHeightEaseDuration);

        slideshowSettings.dynamicTabs = (slideshowSettings.dynamictabs_settings.dynamicTabs == 1) ? true : false;
        slideshowSettings.dynamicTabsAlign = slideshowSettings.dynamictabs_settings.dynamicTabsAlign;
        slideshowSettings.dynamicTabsPosition = slideshowSettings.dynamictabs_settings.dynamicTabsPosition;
        slideshowSettings.panelTitleSelector = slideshowSettings.dynamictabs_settings.panelTitleSelector;

        slideshowSettings.dynamicArrows = (slideshowSettings.dynamicarrows_settings.dynamicArrows == 1) ? true : false;
        slideshowSettings.hoverArrows = (slideshowSettings.dynamicarrows_settings.hoverArrows == 1) ? true : false;

        slideshowSettings.autoSlide = (slideshowSettings.autoslide_settings.autoSlide == 1) ? true : false;
        slideshowSettings.autoSliderDirection = slideshowSettings.autoslide_settings.autoSliderDirection;
        slideshowSettings.autoSlideInterval = parseInt(slideshowSettings.autoslide_settings.autoSlideInterval);
        slideshowSettings.autoSlideControls = (slideshowSettings.autoslide_settings.autoSlideControls == 1) ? true : false;
        slideshowSettings.autoSlideStartText = slideshowSettings.autoslide_settings.autoSlideStartText;
        slideshowSettings.autoSlideStopText = slideshowSettings.autoslide_settings.autoSlideStopText;
        slideshowSettings.autoSlideStopWhenClicked = (slideshowSettings.autoslide_settings.autoSlideStopWhenClicked == 1) ? false : true;
        slideshowSettings.autoSlidePauseOnHover = (slideshowSettings.autoslide_settings.autoSlidePauseOnHover == 1) ? true : false;

        slideshowSettings.responsive = (slideshowSettings.responsive_settings.responsive == 1) ? true : false;
        slideshowSettings.mobileNavigation = (slideshowSettings.responsive_settings.mobileNavigation == 1) ? true : false;
        slideshowSettings.mobileNavDefaultText = slideshowSettings.responsive_settings.mobileNavDefaultText;
        slideshowSettings.mobileUIThreshold = parseInt(slideshowSettings.responsive_settings.mobileUIThreshold);
        slideshowSettings.hideArrowsWhenMobile = (slideshowSettings.responsive_settings.hideArrowsWhenMobile == 1) ? true : false;
        slideshowSettings.hideArrowsThreshold = parseInt(slideshowSettings.responsive_settings.hideArrowsThreshold);
        slideshowSettings.useCSSMaxWidth = parseInt(slideshowSettings.responsive_settings.useCSSMaxWidth);
        slideshowSettings.swipe = (slideshowSettings.responsive_settings.swipe == 1) ? true : false;

        slideshowSettings.targetId = '#' + $(fullId + " :first").attr('id');
        slideshowContainer = $(slideshowSettings.targetId);
//alert('fullId : ' + fullId);
//alert(JSON.stringify(slideshowContainer));
        // Check if liquidSlider has been loaded.
        if (!jQuery.isFunction(slideshowContainer.liquidSlider)) {
          return;
        }

// preloader at true do stop to load ? ...

        slideshowContainer.liquidSlider({

          autoHeight:slideshowSettings.autoHeight,
          autoHeightMin:slideshowSettings.autoHeightMin,
          autoHeightEaseDuration:slideshowSettings.autoHeightEaseDuration,

          autoSlideInterval:slideshowSettings.autoSlideInterval,
          autoSlideControls:slideshowSettings.autoSlideControls,
          autoSlide:slideshowSettings.autoSlide,
          autoSliderDirection:slideshowSettings.autoSliderDirection,
          autoSlideStartText:slideshowSettings.autoSlideStartText,
          autoSlideStopText:slideshowSettings.autoSlideStopText,
          forceAutoSlide:slideshowSettings.autoSlideStopWhenClicked,
          pauseOnHover:slideshowSettings.autoSlidePauseOnHover,

          dynamicTabs:slideshowSettings.dynamicTabs,
          dynamicTabsAlign:slideshowSettings.dynamicTabsAlign,
          dynamicTabsPosition:slideshowSettings.dynamicTabsPosition,
          panelTitleSelector:slideshowSettings.panelTitleSelector,

          dynamicArrows:slideshowSettings.dynamicArrows,
          hoverArrows:slideshowSettings.hoverArrows,

          responsive:slideshowSettings.responsive,
          mobileNavigation:slideshowSettings.mobileNavigation,
          mobileNavDefaultText:slideshowSettings.mobileNavDefaultText,
          mobileUIThreshold:slideshowSettings.mobileUIThreshold,
          hideArrowsWhenMobile:slideshowSettings.hideArrowsWhenMobile,
          hideArrowsThreshold:slideshowSettings.hideArrowsThreshold,
          useCSSMaxWidth:slideshowSettings.useCSSMaxWidth,
          swipe:slideshowSettings.swipe,

          hashLinking:false,
          crossLinks:false,
          preloader:false,
        });
      });
    }
  };
})(jQuery);
