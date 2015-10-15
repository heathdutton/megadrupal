(function ($) {
  Drupal.behaviors.adSenseSearchDefaultReset = {
    attach: function (context) {
      var resetButton = '<button type="button" class="adSenseSearchDefaultReset">' + Drupal.t('Default') + '</button>';

      $('input[data-default-reset], select[data-default-reset]', context)
        .not('.adSenseSearchDefaultReset-processed')
        .addClass('adSenseSearchDefaultReset-processed')
        .after(resetButton);

      $('button.adSenseSearchDefaultReset', context).click(function () {
        var element = $(this).prev('.adSenseSearchDefaultReset-processed');
        element.val(element.attr('data-default-reset'));
      });
    }
  }
})(jQuery);
