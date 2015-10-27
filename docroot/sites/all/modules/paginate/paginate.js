/**
 * @file
 * Paginate
 *
 * Allow to redirect on valid requested page number.
 *
 * @author devendra.yadav <dev.firoza@gmail.com>
 */

(function($) {
  Drupal.behaviors.paginate = {
    attach: function(context) {
      $(".ipaginate input[type='text']", context).bind("keydown keyup keypress", function(event) {
        var keynum;
        var regex = new RegExp("([?|&])page=.*?(&|$)", "i");
        var url = Drupal.settings.paginate.url;

        var curpage = Drupal.settings.paginate.curpage;
        var maxpage = Drupal.settings.paginate.maxpage;
        var pagenum = parseInt($(this).val());

        // IE8 and earlier
        if (window.event) {
          keynum = event.keyCode;

        }
        // IE9/Firefox/Chrome/Opera/Safari
        else if (event.which) {
          keynum = event.which;
        }

        if (pagenum > maxpage) {
          $(this).val(1);
        }

        if (!((keynum >= 48 && keynum <= 57) || (keynum >= 96 && keynum <= 105) || (keynum == 13) || (keynum == 8) || (keynum == 46) || (keynum == 17) || (keynum == 116))) {
          $(".ipaginate span#errmsg").html("Digits only").show().fadeOut("slow");
          return false;
        }

        if (keynum == 13 && pagenum <= 0) {
          return false;
        }

        if (keynum == 13 && curpage != pagenum && event.type != "keyup") {
          if (url.match(regex)) {
            param = 'page';
            pagenum = (pagenum - 1);
            url = url.replace(regex, '$1page=' + pagenum + '$2');
          }
          window.location.href = url;
        }
        if (keynum == 13) {
          return false;
        }
      });
    }
  };
})(jQuery);
