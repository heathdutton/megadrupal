(function ($) {
  Drupal.behaviors.views_showcaseBehavior = {
    attach: function(context){
      //change the active slide class name
      $.fn.cycle.updateActivePagerLink = function(pager, currSlideIndex) {
          $(pager).find('.views-showcase-pager-item').removeClass('activeItem')
              .filter('.views-showcase-pager-item:eq('+currSlideIndex+')').addClass('activeItem');
      };

      var sync      = Drupal.settings.views_showcase.sync      == "true" ? true : false;
      var listPause = Drupal.settings.views_showcase.listPause == "true" ? true : false;
      var pause     = Drupal.settings.views_showcase.pause     == "true" ? true : false;

      //if timeout is set to a negative value change it to 0
      if (parseInt(Drupal.settings.views_showcase.timeout) > 0) {
        var handled_timeout = parseInt(Drupal.settings.views_showcase.timeout);
      }
      else {
        var handled_timeout = 0;
      }

      $('ul.views-showcase-big-panel').cycle({
        fx: Drupal.settings.views_showcase.cycle,
        easing: Drupal.settings.views_showcase.easing,
        sync: sync,
        pauseOnPagerHover: listPause,
        pause: pause,
        timeout: handled_timeout,
        pager:  '.views-showcase-mini-list',
        pagerAnchorBuilder: function(idx, slide) {
            // return selector string for existing anchor
            return '.views-showcase-mini-list .views-showcase-pager-item:eq(' + idx + ')';
        }
      });
    }
  }

})(jQuery);
