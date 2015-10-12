/**
 * @file
 * Javascript for Tile.
 */

/**
 * Provides a farbtastic colorpicker for Tile.
 */
(function ($) {
  Drupal.behaviors.tile_colorpicker = {
    attach: function(context) {
      $(".edit-tile-colorpicker").live("focus", function(event) {
        var edit_field = this;
        var picker = $(this).closest('div').parent().find(".tile-colorpicker");
        // Hide all color pickers except this one.
        $(".tile-colorpicker").hide();
        $(picker).show();
        $.farbtastic(picker, function(color) {
          edit_field.value = color;
        }).setColor(edit_field.value);
      });
    }
  }
})(jQuery);
