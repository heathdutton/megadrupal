(function($){

var process_slideshow = function(context, o) {
  var $e = $('.view-content', context);
  var pager_dynamic = false;

  // query for slide item
  o.slideExpr = o.slideExpr ? o.slideExpr : '.views-row';

  // Pager
  if (typeof o.pager !== 'undefined') {
    switch (o.pager) {
      case 'before':
      case 'after':
        pager_dynamic = true;
        pager_position = o.pager;
        break;
    }

    if (pager_dynamic) {
      var pager_id = 'goSlideshowPager_'+ o.view_name+'_'+ o.display_id;

      o.pager === 'before' ? $e.before(slideshow_pager_wrap(pager_id)) : $e.after(slideshow_pager_wrap(pager_id));
      o.pager = '#' + pager_id;
      o.pagerAnchorBuilder = function(idx, slide) {
        var text = idx + 1;

        if (typeof o.pagerExpr !== 'undefined') {
          text = $('<div></div>');
          $('.views-row:eq('+ idx+')', $e).find(o.pagerExpr).appendTo(text);
          text = text.html();
        }

        return '<li class="pager-item"><a href="#">'+ text +'</a></li>';
      };
    }
  }

  delete o[''];
  delete o[''];
  delete o[''];
  delete o[''];
  delete o[''];
  delete o.view_name;
  delete o.display_id;

  $e.cycle(o);
}

var slideshow_pager_wrap = function(pager_id) {
  return '<div class="item-list"><ul class="pager" id="'+ pager_id +'"></ul></div>';
}

Drupal.behaviors.goSlideshow = {};
Drupal.behaviors.goSlideshow.attach = function(context) {
  if (typeof Drupal.settings.go_slideshow.views !== 'undefined') {
    for (var view_name in Drupal.settings.go_slideshow.views) {
      for (var display_id in Drupal.settings.go_slideshow.views[view_name]) {
        var query_wrapper = '.view-id-'+ view_name +'.view-display-id-' + display_id;
        var $wrapper = $(query_wrapper + ':not(goSlideshowProcessed)', context);
        if ($wrapper.size()) {
          $wrapper.addClass('goSlideshowProcessed');
          Drupal.settings.go_slideshow.views[view_name][display_id].view_name = view_name;
          Drupal.settings.go_slideshow.views[view_name][display_id].display_id = display_id;
          process_slideshow($wrapper, Drupal.settings.go_slideshow.views[view_name][display_id]);
        }
      }
    }
  }
};

})(jQuery);
