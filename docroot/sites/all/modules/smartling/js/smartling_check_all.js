(function ($) {
  Drupal.behaviors.smartlingCheckAll = {
    attach: function (context, settings) {
      for (var i = 0; i < settings.smartling.checkAllId.length; ++i) {
        var $checkboxWrapper = $(settings.smartling.checkAllId[i], context);
        if ($('#smartling-check-all-' + i).length || $checkboxWrapper.length < 1) {
          continue;
        }
        if ($checkboxWrapper.children().length > 5) {
          $checkboxWrapper.addClass('big-select-languages-widget');
        }
        $checkboxWrapper.prepend('<a href="#" id="smartling-check-all-' + i + '">' + Drupal.t('Check/uncheck all') + '</a>');

        $('#smartling-check-all-' + i).click(function () {
          if ($(this).attr('checked_all') == "0") {
            $(this).parent().find(':checkbox').attr("checked", true).each(function () {
              this.click()
            });
            $(this).attr('checked_all', 1);
          }
          else {
            $(this).parent().find(':checkbox').attr("checked", false).each(function () {
              this.click()
            });
            $(this).attr('checked_all', 0);
          }
          return false;
        });
      }
    }
  };
})(jQuery);
