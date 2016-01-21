
(function($) {
  $(document).ready(function() {
    // initiate farbtastic colorpicker
    var f = $.farbtastic('#pickerholder');
    $('.color_fieldset')
      .each(function () { f.linkTo(this); })
      .focus(function() {
        f.linkTo(this);
      });
  });
}) (jQuery);
