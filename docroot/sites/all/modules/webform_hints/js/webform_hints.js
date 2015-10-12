/**
 * @file
 * jQuery fallback to add placeholder style values to form elements controlled
 * by the Webform Hints module.
 */

(function ($) {
  Drupal.behaviors.webformHints = {
    attach: function (context, settings) {
      /*
       * We only run the formHints plugin if the browser
       * has no support for the HTML5 placeholder attribute
       */
      if (!supports_input_placeholder()) {
        $('form.webform-hints').find('.webform-hints-field').DefaultValue();
      }
    }
  };

  /*
   * Checks if the browser supports the HTML5 placeholder attribute
   * (Borrowed from http://diveintohtml5.org/detect.html#input-placeholder)
   */
  function supports_input_placeholder() {
    var i = document.createElement('input');
    return 'placeholder' in i;
  }
})(jQuery);
