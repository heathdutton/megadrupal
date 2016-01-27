$ = jQuery.noConflict(); // Make sure jQuery owns $ here

/**
 * Using window.load instead of document.ready is a requirement for compatibility
 * with fluid images. If images (used as slides) have no dimensions set webkit
 * browsers will fail to properly scale the slider.
 */
var started = 0;
var pagerEl = '.cycle-pager';
var pagerAnchor = null;
var cyclePrev = null;
var cycleNext = null;
var pagerAnchor = null;
$(window).load(function() {
  cycleStart();
  $(window).resize(function() {
    cycleStart();
  });
});
function cycleStart() {
  // Remove contextual links, they mess up the cycle pager
  $('.block').has(Drupal.settings.slideshowKit.cycleInvoke).find('.contextual-links-wrapper').remove();

  if (parseInt(Drupal.settings.slideshowKit.cycleShowPager) && (started != 1)) {
    $(Drupal.settings.slideshowKit.cycleInvoke).each(function() {
      $(this).after('<div class="wrap-cycle-pager"><ol class="cycle-pager pager-' + $(this).find('>li').length + '">');
      pagerAnchor = function(idx, slide) {return '<li><a href="#">' + (idx+1) + '</a></li>';}
    });

  }
  if (parseInt(Drupal.settings.slideshowKit.cycleImgPager) && (started != 1)) {
    pagerAnchor = function(idx, slide) {return '<li><a href="#"><img src="' + slide.src + '" width="100" height="100" /></a></li>';}
  }
  if (parseInt(Drupal.settings.slideshowKit.cyclePrevNext) && (started != 1)) {
    $(Drupal.settings.slideshowKit.cycleInvoke).after('<div class="cycle-previous"></div><div class="cycle-next"></div>');
    cyclePrev = '.cycle-previous';
    cycleNext = '.cycle-next';
  }

  if (started) {
    pagerEl = null;
  }

  /**
   * @code
   * force the slideshow to cover entire width
   */
  $(Drupal.settings.slideshowKit.cycleInvoke).width('100%');

  $(Drupal.settings.slideshowKit.cycleInvoke).each(function() {
    $(this).cycle({
      fx:                   Drupal.settings.slideshowKit.cycleFx,
      timeout:              parseInt(Drupal.settings.slideshowKit.cycleTimeout),
      continuous:           parseInt(Drupal.settings.slideshowKit.cycleContinuous),
      speed:                parseInt(Drupal.settings.slideshowKit.cycleSpeed),
      pagerEvent:           Drupal.settings.slideshowKit.cyclePagerEvent,
      easing:               Drupal.settings.slideshowKit.cycleEasing,
      random:               parseInt(Drupal.settings.slideshowKit.cycleRandom),
      pause:                parseInt(Drupal.settings.slideshowKit.cyclePause),
      pauseOnPagerHover:    parseInt(Drupal.settings.slideshowKit.cyclePauseOnPagerHover),
      delay:                parseInt(Drupal.settings.slideshowKit.cycleDelay),
      pager:                started ? null : $(this).parent().find('.cycle-pager'),
      pagerAnchorBuilder:   pagerAnchor,
      prev:                 cyclePrev,
      next:                 cycleNext
    });
  });

  /**
   * @ code
   * This is required to force the browse to upscale image slides at window.resize
   * (e.g. when switching from portrait to landscape mode)
   * Custom pager code piggyback-rides on this
   */
  $(Drupal.settings.slideshowKit.cycleInvoke).each(function() {
    var $customPager = sliderHeight = 0;
    slideIdx = 1;
    $(this).find('>*').each(function() {
        // Do custom pager thing if we find a .pager element
        if ($(this).find('.pager').length > 0) {
          customPager = $(this).find('.pager').html();
          $(this).find('.pager').remove();
          $(this).parent().parent().find('.cycle-pager li:nth-child('+ slideIdx +') a').html(customPager)
        }
        if ($(this).outerHeight() > sliderHeight) sliderHeight = $(this).outerHeight();

        slideIdx++;
    });
    $(this).width('100%').height(sliderHeight);
  });

  // Keep track of restarts
  started = 1;
}
