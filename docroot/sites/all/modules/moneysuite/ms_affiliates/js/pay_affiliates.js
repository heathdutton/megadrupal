/**
 * @file
 * Licensed under the GNU GPLv2 License
 */

(function ($) {
  $(document).ready(function() {
    $(".ms-view-request").toggle(
      function () {
        $(this).siblings(".ms-request-info").addClass("ms-hover");
        return false;
      },
      function () {
        $(this).siblings(".ms-request-info").removeClass("ms-hover");
        return false;
      }
    );
  });
})(jQuery);
