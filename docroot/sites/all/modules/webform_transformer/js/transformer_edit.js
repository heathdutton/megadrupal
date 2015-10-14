(function ($) {

/**
 * @file
 * The admin related javascript
 *
 * @author Glenn De Backer <glenn at coworks dot be>
 *
 */

Drupal.behaviors.transformer_webform = {
  attach: function (context, settings) {

    // defining my own global namespace
    var global = {};

    // get 'pointer' to dropdown element
    global.patternDropdown = $('#edit-extra-field-predefined-pattern');

    // get 'pointer' to pattern input element
    global.patternInput = $('#edit-extra-field-pattern');

    /*
     * Generates a demo pattern
     */
    generateDemoPattern = function() {
        // split pattern
        var patternChars = $(global.patternInput).val();
        var demoPattern = '';

        // iterate through pattern
        for (var i = 0, l = patternChars.length; i < l; i++) {
          if (patternChars[i] == '#') {
            demoPattern += Math.floor(((Math.random()) * 10)+1);
          }
          else {
            demoPattern += patternChars[i];
          }
        }

        // update description field
        global.patternInput.parent().find('.description:first').html(Drupal.t('example') + ' : ' + demoPattern);
    };

    // check if there was already a pattern defined
    if (global.patternInput.val().length > 0) {
      generateDemoPattern();
    }

    // bind change event to dropdown
    $(global.patternDropdown).bind('change',function() {
      if (this.value != '_none') {
        // set value
        $('#edit-extra-field-pattern').val(this.value);

        // generate demo pattern
        generateDemoPattern();
      }
    });

    // bind keyup event to pattern field
    $(global.patternInput).bind('keyup',function() {
      generateDemoPattern();
    });
  }
};

})(jQuery);
