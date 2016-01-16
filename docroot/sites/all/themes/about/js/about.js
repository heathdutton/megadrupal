;
(function($) {

  $('#about-story').css({
    'background-color' : Drupal.settings.about.backgroundColor,
    'color' : Drupal.settings.about.color,
    'font-family' : Drupal.settings.about.fontFamily,
    'font-size' : Drupal.settings.about.fontSize
  });
  
  $('#about-story').transify({ opacityNew : Drupal.settings.about.backgroundOpacity });
  $('.transify').corner();

})(jQuery);