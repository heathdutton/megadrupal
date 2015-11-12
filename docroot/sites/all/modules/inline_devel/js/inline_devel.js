/**
 * @file
 * Overriding the devel php execution form.
 */

(function ($) {

  /**
   * Apply the inline devel functionally on the devel php form.
   */
  Drupal.behaviors.applyInlineDevel = {
    attach: function() {
      $("#edit-code").inline_devel();
    }
  }

  /**
   * Display the variable suggester.
   */
  Drupal.behaviors.variableSuggestor = {
    attach: function() {

      // Expose the variable suggester.
      $("body").keydown(function(event) {
        if (event.ctrlKey == true  && event.shiftKey == true && event.which == 86) {
          event.preventDefault();

          if ($(".form-type-textfield.form-item-variables").css('display') == 'none') {
            $(".form-type-textfield.form-item-variables").show(500);
          }
          else {
            $(".form-type-textfield.form-item-variables").hide(500);
          }

          $("#edit-variables").val('');
          $("#edit-variables").focus();
        }
      });

      // Insert the variables to the devel php text area.
      $("#edit-variables").keyup(function(keyPressed) {
        if (keyPressed.which != 13) {
          return;
        }

        var data = $("#edit-code").elementData();
        var cursor = data.cursor;
        var word = "variable_get('" + $(this).val() + "')";

        $("#edit-code").focus();

        $("#edit-code").val(data.value.slice(0, cursor) + word + data.value.slice(cursor));
        $("#edit-code").setCursorPosition(cursor + word.length, cursor + word.length);
        $(".form-type-textfield.form-item-variables").hide(500);
      });
    }
  }

})(jQuery);


