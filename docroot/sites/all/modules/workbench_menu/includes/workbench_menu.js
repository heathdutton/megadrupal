(function ($) {
  Drupal.behaviors.workbench_menu_clickable_label = {
    attach:function(context) {
      $('.menu_item.clickable.noscript', context).removeClass('noscript').each(function() {
        var item = $(this);

        $(item).children('a.menu_item-text').each(function() {
          $(this).removeAttr('href');

          $(this).click(function() {
            if ($(item).hasClass('menu_item-type-item_top')) {
              if ($(item).hasClass('expanded')) {
                $(item).removeClass('expanded');
                $(item).addClass('collapsed');
              } else {
                $(item).removeClass('collapsed');
                $(item).addClass('expanded');
              }
            }
            else if (!$(item).hasClass('active') && !$(item).hasClass('active-trail')) {
              if ($(item).hasClass('expanded')) {
                $(item).removeClass('expanded');
                $(item).addClass('collapsed');
              } else {
                $(item).removeClass('collapsed');
                $(item).addClass('expanded');
              }
            }
          });
        });
      });
    }
  }
})(jQuery);
