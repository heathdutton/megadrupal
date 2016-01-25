(function ($) {
/**
 * fft_template Behaviors.
 */
Drupal.behaviors.fft_template = {
  attach: function (context, settings) {
    $("select.fft-template:not(.fft-processed)").addClass('fft-processed').change(function(event) {
      var template = this.value;
      $("textarea.fft-settings").val("");
      if (Drupal.settings.fft[template] != 'undefined'){
        $("textarea.fft-settings").val(Drupal.settings.fft[template]);
      }
    });

    if ($("textarea.fft-settings").val() == "") {
      $("select.fft-template.fft-processed").trigger('change');
    }
  }
};

})(jQuery);

