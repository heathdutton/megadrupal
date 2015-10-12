(function ($) {
  Drupal.behaviors.smileyslider = {
    attach: function (context) {
      $('.smiley-slider', context).hide();
      $('.slider', context).each(function() {
        var s = new SmileySlider(this, Drupal.settings.smileyslider.image);
        s.position(function (p) {
          $(this).next().val(Math.round(p * Drupal.settings.smileyslider.range));
        });
        // This is a weird construction but it's necessary to get the positioning to work.
        setTimeout(function() { s.position(Drupal.settings.smileyslider.default); }, 0);
      });
    }
  }
})(jQuery);
