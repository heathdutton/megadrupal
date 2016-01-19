(function ($) {
  var views_infinite_table_scroll_was_initialised = false;
  Drupal.behaviors.views_infinite_table_scroll = {
    attach:function() {
      var settings = Drupal.settings.views_infinite_table_scroll[0];
      var view_selector    = 'div.view-id-' + settings.view_name + '.view-display-id-' + settings.display;
      if (!views_infinite_table_scroll_was_initialised || $(view_selector + ' .tbodyScroll-outer').length == 0) {
        views_infinite_table_scroll_was_initialised = true;
        var content_selector = view_selector + ' > ' + settings.content_selector;
        var table_selector   = view_selector + ' table';
        var tbody_selector   = view_selector + ' tbody';
        var items_selector   = content_selector + ' ' + settings.items_selector;
        var pager_selector   = view_selector + ' > div.item-list ' + settings.pager_selector;
        var next_selector    = view_selector + ' ' + settings.next_selector;
        $(view_selector + ' .pager').eq(0).hide();
        settings.nextpage = 1;
        settings.currentpage = 0;
        var pagetotal = settings.page_total - 1;
        if (parseInt(settings.tfoot_height)) {
          $(tbody_selector).eq(0).after('<tfoot>' + ($(table_selector + ' thead').eq(0).html()) + '</tfoot>');
        }
        else {
          settings.tfoot_height = 0;
        }
        $(table_selector).eq(0).tbodyScroll({
           thead_height : parseInt(settings.thead_height) + 'px',
           tfoot_height : parseInt(settings.tfoot_height) + 'px',
           tbody_height : parseInt(settings.tbody_height) + 'px',
        });
        $(view_selector + ' .tbodyScroll-inner').scroll(function() {
          if (settings.currentpage < pagetotal) {
            if ($(view_selector + ' .tbodyScroll-inner').scrollTop() >= $(view_selector + ' .tbodyScroll-inner')[0].scrollHeight - 250) {
              if (settings.nextpage) {
                settings.currentpage += 1;
                settings.nextpage = 0;
                $.ajax({
                  url: "/views/ajax",
                  type: "POST",
                  dataType:"json",
                  data: {
                    "view_name": settings.view_name ,
                    "view_display_id":settings.display,
                    "page" : settings.currentpage,
                    "view_args" : settings.view_args
                  },
                  success: function(view) {
                    $(tbody_selector).eq(0).append($(view[1].data).find('tbody').html());
                    jQuery.extend(Drupal.settings, Drupal.settings, view[0].settings);
                    Drupal.attachBehaviors(view_selector);
                    settings.nextpage = 1;
                  }
                })
              }
            }
          }
        });
      }
    }
  }
})(jQuery);
