/**
 * @file
 * A JavaScript file for the theme.
 */

(function ($) {

  $(document).ready(function () {

    $('#main-menu li.level-1 > ul').hover(
      function () {
        $(this).parent().addClass('hover-trail');
      },
      function () {
        $(this).parent().removeClass('hover-trail');
      }
    );

  });

}(jQuery));