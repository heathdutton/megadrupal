/**
 * @file
 * Javascript for Circular Chart Module.
 */

/**
 * Provides a farbtastic colorpicker for the presets.
 */
(function ($) {
  Drupal.behaviors.circular_chart_colorpicker = {
    attach: function(context) {
      // Process the color picker and color the below associated fields.
      $(".edit-circle-chart-colorpicker").live("focus", function(event) {
        var edit_field = this;
        var edit_field_id = $(this).attr('id');
        var picker = $(this).parent('div').next();
        // Before showing the farbtastic hide others.
        $(".circle-chart-colorpicker").hide();
        $(picker).show();
        $.farbtastic(picker, function(color) {
          $(edit_field).val(color);
          $(edit_field).css('background-color', color);
        }).setColor(edit_field.value);
      });
    }
  }
})(jQuery);
