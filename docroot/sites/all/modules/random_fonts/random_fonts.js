/**
 * @file
 * JS for the Random Fonts (random_fonts) module.
 */

(function($) {
  Drupal.behaviors.randomFonts = {
    attach: function (context, settings) {

      var selectors = Drupal.settings.randomFonts.selectors;
      var count = Drupal.settings.randomFonts.count;
      var noAdmin = Drupal.settings.randomFonts.noAdmin;
      $(selectors).each(function(i) {
        // Bail out if the element is a child of toolbar and is to be excluded.
        if (noAdmin && $(this).parents('#toolbar').length) {
          return;
        }
        var rand = Math.floor(Math.random() * count);
        $(this).addClass('random_font_' + rand);
        $(this).attr('title', Drupal.settings.randomFonts['f' + rand]);
      });
      // Remove title attributes from links for attributes on ul/ol to be seen.
      $('a').each(function(i) {
        $(this).removeAttr('title');
      });

    }
  }
}(jQuery));
