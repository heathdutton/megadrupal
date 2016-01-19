(function ($) {

  Drupal.behaviors.prettypagination = {
    attach: function (context) {
      $('ul.pager:not(.prettypagination-class-processed)', context).each(function () {
        var pagerid;
        $(this).addClass('prettypagination-class-processed');
        $(this).children().each(function() {
          if($(this).hasClass('pager-ellipsis') || $(this).hasClass('pager-item')) {
            $(this).children().each(function() {
              if($(this).attr("id")) {
                pagerid = $(this).attr("id");
              }
            });
            $(this).remove();
          }
        });
        $(this).children().each(function() {
          if($(this).hasClass('pager-current')) {
            if($(this).prev().hasClass('pager-previous')) {
              $(this).prev().addClass("active");
              $(this).prev().prev().addClass("active");
            }else {
              $(this).before("<li class='pager-first first inactive'>" + Drupal.t("«") + "</li><li class='pager-previous inactive'>" + Drupal.t("‹") + "</li>");
            }
            if($(this).next().hasClass('pager-next')) {
              $(this).next().addClass("active");
              $(this).next().next().addClass("active");
            }else {
              $(this).after("<li class='pager-next inactive'>" + Drupal.t("›") + "</li><li class='pager-last last inactive'>" + Drupal.t("»") + "</li>");
            }
            for(pager1 in Drupal.settings.prettypagination) {
              if(Drupal.settings.prettypagination[pager1].id == pagerid) {
                $(this).replaceWith(Drupal.settings.prettypagination[pager1].pager_replace_data);
              }
            }
          }
        });
      });

      $(".prettypaginationinput",context).bind("keydown keyup keypress", function(event) {
        var keynum;
        var curpage = parseInt($(this).prev().prev().val());
        var pagenum = parseInt($(this).val());
        // IE8 and earlier
        if(window.event)
        {
          keynum = event.keyCode;
        }
        // IE9/Firefox/Chrome/Opera/Safari
        else if(event.which)
        {
          keynum = event.which;
        }
        if (keynum == 13 && curpage != pagenum && event.type != "keyup") {
          $(this).prev().attr('href',$(this).prev().attr('href').replace("prettypaginationinputpage",pagenum - 1));
          window.location.href = $(this).prev().attr('href');
        }
        if (keynum == 13) {
          return false;
        }
      });
    }
  };

})(jQuery);
