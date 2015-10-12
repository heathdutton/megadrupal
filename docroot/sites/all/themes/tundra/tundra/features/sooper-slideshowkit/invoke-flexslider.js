$ = jQuery.noConflict(); // Make sure jQuery owns $ here

$(window).load(function() {
  setFlexAnimation = parseInt(Drupal.settings.slideshowKit.flexAnimation);
  if (setFlexAnimation == 2) {
    var setSlideDirection = 'vertical';
  } else {
    var setSlideDirection = 'horizontal';
  }

  if (setFlexAnimation) {
    setSlideAnimation = 'slide';
  } else {
    setSlideAnimation = 'fade';
  }
  $(Drupal.settings.slideshowKit.flexInvoke).each(function() {
    // Flexslider has a hardcoded reliance on the .slides class
    $(this).find("ul:first-child").addClass("slides");
    if ($(this).find(".slides").hasClass('carousel')) {
      flexItemWidth = parseInt(Drupal.settings.slideshowKit.flexItemWidth);
      flexItemMargin = parseInt(Drupal.settings.slideshowKit.flexItemMargin);
      flexMinItems = parseInt(Drupal.settings.slideshowKit.flexMinItems);
      flexMaxItems = parseInt(Drupal.settings.slideshowKit.flexMaxItems);
      flexMove = parseInt(Drupal.settings.slideshowKit.flexMove);
    } else {
      flexItemWidth = flexItemMargin = flexMinItems = flexMaxItems = flexMove = 0
    }

    $(this).flexslider({
      animation:            setSlideAnimation,
      slideDirection:       setSlideDirection,
      slideshowSpeed:       parseInt(Drupal.settings.slideshowKit.flexTimeout),
      animationDuration:    parseInt(Drupal.settings.slideshowKit.flexSpeed),
      randomize:            parseInt(Drupal.settings.slideshowKit.flexRandom),
      pauseOnHover:         parseInt(Drupal.settings.slideshowKit.flexPause),
      controlNav:           parseInt(Drupal.settings.slideshowKit.flexShowPager),
      directionNav:         parseInt(Drupal.settings.slideshowKit.flexPrevNext),
      itemWidth:            flexItemWidth,
      itemMargin:           flexItemMargin,
      minItems:             flexMinItems,
      maxItems:             flexMaxItems,
      move:                 flexMove
    });
  });
});