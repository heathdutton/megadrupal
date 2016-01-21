/**
 * @file
 * Javascript for imagecache reflect.
 */

(function ($) {
/**
 * Attach farbtastic colorpicker.
 */
Drupal.behaviors.imagecacheReflect = {
  attach: function(context) {
    $(".edit-imagecache-reflect-colorpicker").live("focus", function(event) {
      var edit_field = this;
      var picker = $(this).closest('div').parent().find(".imagecache-reflect-color");

      // Hide all color pickers except this one.
      $(".imagecache-reflect-color").hide();
      $(picker).show();
      $.farbtastic(picker, function(color) {
        edit_field.value = color;
      }).setColor(edit_field.value);
    });
  }
}

})(jQuery);
