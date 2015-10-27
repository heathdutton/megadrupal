(function ($) { // JavaScript should be compatible with other libraries than jQuery
  Drupal.behaviors.evanced_registration = { // D7 "Changed Drupal.behaviors to objects having the methods "attach" and "detach"."
    attach: function(context) {



      // Add/remove the default text when they click off/on the input box in the block.
      $("#evanced-registration-form #edit-phoneac, #evanced-registration-form #edit-areacode2").each(function() {
        if (this.value.length < 1 || this.value == 'Example: 803-555-1234') {
          this.value = 'Example: 803-555-1234';
          $(this).css({"color":"#adadad"});
          $(this).css({"font-style":"italic"});
        }
      });
      $("#evanced-registration-form #edit-phoneac, #evanced-registration-form #edit-areacode2").focus(function() {
        if (this.value.length < 1 || this.value == 'Example: 803-555-1234') {
          this.value = '';
          $(this).css({"color":"#000"});
          $(this).css({"font-style":"normal"});
        }
      });
      $("#evanced-registration-form #edit-phoneac, #evanced-registration-form #edit-areacode2").blur(function() {
        if (this.value.length < 1) {
          this.value = 'Example: 803-555-1234';
          $(this).css({"color":"#adadad"});
          $(this).css({"font-style":"italic"});
        }
      });
      $("#evanced-registration-form").submit(function() { // Don't submit the placeholder information along w/ the values entered.
        $("#evanced-registration-form #edit-phoneac, #evanced-registration-form #edit-areacode2").each(function() {
          if (this.value.length < 1 || this.value == 'Example: 803-555-1234') {
            this.value = '';
          }
        });
      });



    }
  };
})(jQuery);
