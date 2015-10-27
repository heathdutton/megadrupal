/**
 * @file
 * Used for color settings.
 */

(function ($) {
  Drupal.color = {
    logoChanged: false,
    callback: function(context, settings, form, farb, height, width) {
      // Change the logo to be the real one.
      if (!this.logoChanged) {
        $('#preview #preview-logo img').attr('src', Drupal.settings.color.logo);
        this.logoChanged = true;
      }
      // Remove the logo if the setting is toggled off.
      if (Drupal.settings.color.logo == null) {
        $('div').remove('#preview-logo');
      }

      // Text preview.
      var color_base = $('#palette input[name="palette[base]"]', form).val();
      var color_secondary = $('#palette input[name="palette[secondary]"]', form).val();
      var color_content = $('#palette input[name="palette[content]"]', form).val();
      var color_background = $('#palette input[name="palette[background]"]', form).val();

      // Background.
      // Base color.
      $("#preview-nav, #preview-footermenus, #preview-welcome, #preview .bg-color-base").css("background-color",color_base);
      $("#preview-footercopyright").css("background-color",color_secondary);
      $(".preview-content").css("background-color",color_background);

      // Color.
      $(".preview-content, #preview .clr-content").css("color",color_content);
      $(".clr-base").css("color",color_base);

      // Border color.
      $("#preview .bdr-clr-secondary, #preview blockquote").css("border-color",color_secondary);
    }
  };
})(jQuery);
