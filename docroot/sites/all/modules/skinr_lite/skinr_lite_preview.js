
(function ($) {
  $.fn.skinrLitePreview = function() {
    this.each(function() {
      var allOptions = Drupal.settings.skinrLite[$(this).attr('skinrlite')].options;
      
      if ($(this).is('select')) {
        $(this).children('option').each(function() {
          $('#color_scheme_form #preview').removeClass(allOptions[$(this).val()]['class'].join(' '));
        })
        $('#color_scheme_form #preview').addClass(allOptions[$(this).val()]['class'].join(' '));
      }
      else if ($(this).is(':checked')) {
        if ($(this).is('.skinr-lite.radios')) {
          $(this).parents('.form-radios.skinr-lite.radios').find('input.skinr-lite.radios').each(function() {
            $('#color_scheme_form #preview').removeClass(allOptions[$(this).val()]['class'].join(' '));
          })
        }
        $('#color_scheme_form #preview').addClass(allOptions[$(this).val()]['class'].join(' '));
      }
      else {
        $('#color_scheme_form #preview').removeClass(allOptions[$(this).val()]['class'].join(' '));
      }
      
    })
  }
  Drupal.behaviors.skinrLite = {
    attach: function (context, settings) {
      
      $('select.skinr-lite, input.skinr-lite').skinrLitePreview();
      
      $('select.skinr-lite, input.skinr-lite').change(function() {
        $(this).skinrLitePreview();
      })
    }
  }
})(jQuery);
