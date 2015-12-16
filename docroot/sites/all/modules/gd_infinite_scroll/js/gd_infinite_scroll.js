// $Id:

(function ($) {
var gd_infinite_scroll_was_initialised = false;
Drupal.behaviors.gd_infinite_scroll = {
  attach:function() {
    // Make sure that autopager plugin is loaded
    if($.autopager) {
      if(!gd_infinite_scroll_was_initialised) {
        gd_infinite_scroll_was_initialised = true;
        var settings = Drupal.settings.gd_infinite_scroll;
    
        var content_selector  = settings.content_selector;
        var items_selector    = settings.content_selector + ' ' + settings.items_selector;
        var pager_selector    = settings.pager_selector;
        var next_selector     = settings.next_selector;
        var img_location      = settings.content_selector;
        var img_path          = settings.img_path;
        var load_more         = settings.load_more;
        var load_more_markup  = settings.load_more_markup;
        var img               = '<div id="gd_infinite_scroll-ajax-loader"><img src="' + img_path + '" alt="loading..."/></div>';
        
        if (!$(content_selector)[0]) {
            return;
        }
        
        var autoloading = true;
        var load_more_button = $('<a></a>');
        if (load_more) {
            autoloading = false;
            load_more_button = $(load_more_markup);
        }
        $(pager_selector).hide();
        var handle = $.autopager({
          autoLoad: autoloading,
          appendTo: content_selector,
          content: items_selector,
          link: next_selector,
          page: 0,
          start: function() {
            $(img_location).after(img);
          },
          load: function(current, next) {
            $('div#gd_infinite_scroll-ajax-loader').remove();
            Drupal.attachBehaviors(this);
            if (load_more && !next.url) {
                load_more_button.hide();
            }
          }
        });
      
        if (!load_more) {
            // Trigger autoload if content height is less than doc height already
            var prev_content_height = $(content_selector).height();
            do {
              var last = $(items_selector).filter(':last');
              if(last.offset().top + last.height() < $(document).scrollTop() + $(window).height()) {
                last = $(items_selector).filter(':last');
                handle.autopager('load');
              }
              else {
                break;
              }
            }
            while ($(content_selector).height() > prev_content_height);
        }            
        else if ($(next_selector)[0]){        
            load_more_button.addClass('gd-infinite-scroll-load-more').appendTo($(img_location).parent()).click(function(e) {
                // do load
                handle.autopager('load');
                e.preventDefault();
            });
        }
        
      }
    }
    else {
      alert(Drupal.t('Autopager jquery plugin in not loaded.'));
    }
  }
}

})(jQuery);
