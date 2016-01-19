(function ($) {
  "use strict";

  $(document).ready(function () {
    $('input.select-all').change(function () {
      var parent = $(this).parents('table')[0];
      if ($(this).is(':checked')) {
        $(parent).find('input[type="checkbox"]').attr('checked', 'checked');
      }
      else {
        $(parent).find('input[type="checkbox"]').removeAttr('checked');
      }
    });

    $('table input[type="checkbox"]').change(function () {
      if ($(this).hasClass('select-all')) {
        return;
      }

      var parent = $(this).parents('table')[0];
      $(parent).find('input.select-all').removeAttr('checked');
    });
  });
})(jQuery);
