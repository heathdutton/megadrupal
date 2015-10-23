(function ($) {

  Drupal.behaviors.collageformatter = {};
  Drupal.behaviors.collageformatter.attach = function(context, settings) {
    var $color_picker = $('.collageformatter-color-picker').not('.collageformatter-processed');
    if ($color_picker.length > 0) {
      $color_picker.addClass('collageformatter-processed');
      $color_picker.each(function () {
        $(this).farbtastic($(this).prev().find('input')
          .focus(function(event) {
            $(this).parent().next().show();
          })
          .blur(function(event) {
            $(this).parent().next().hide();
          })
        ).hide();
      });
    }
  };
})(jQuery);
