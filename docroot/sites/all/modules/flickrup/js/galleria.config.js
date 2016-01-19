/**
 * @file
 * Script for configure Galleria plugin.
 */

(function ($) {
  Drupal.behaviors.flickrup_galleria_plugin = {
    attach: function (context, settings) {
      Galleria.loadTheme(settings.flickrup_path_to_plugin_theme);
      Galleria.configure({
        transition: 'fade',
        trueFullscreen: false
      });
      // Initialize Galleria.
      for (var i = 0, length = settings.flickrup_field_name.length; i < length; i++) {
        Galleria.run('.field-name-' + settings.flickrup_field_name[i], {
          height: settings.flickrup_galleria_height[i],
          width: settings.flickrup_galleria_width[i]
        });
      }
      Galleria.ready(function () {
        var gallery = this;
        this.addElement('fscr');
        this.appendChild('stage', 'fscr');
        var fscr = this.$('fscr')
          .click(function () {
            gallery.toggleFullscreen();
          });
        this.addIdleState(this.get('fscr'), {opacity: 0});
      });
    }
  }
})(jQuery);
