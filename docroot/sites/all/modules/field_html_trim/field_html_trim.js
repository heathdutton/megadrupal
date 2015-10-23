/**
 * @file
 * Drupal behaviour for applying states-like functionality to show fieldset.
 */
(function($) {

Drupal.behaviors.fieldHtmlTrim = {
  attach: function (context, settings) {

    // Find the elements.
    $htmlTrimCheckbox = $('.html-trim-checkbox');
    $htmlTrim = $('.html-trim-fieldset');

    // Set initial display of html trim settings.
    if ($htmlTrimCheckbox.is(':checked')) {
      $htmlTrim.show();
    }
    else {
      $htmlTrim.hide();
    }

    // Toggle display of the html trim settings.
    $htmlTrimCheckbox.change(function() {
      if ($(this).is(':checked')) {
        $htmlTrim.show();
      }
      else {
        $htmlTrim.hide();
      }
    });

  }
}

})(jQuery);
