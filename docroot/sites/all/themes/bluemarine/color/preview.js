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

      // Solid background.
      $('#preview', form).css('backgroundColor', $('#palette input[name="palette[base]"]', form).val());

      // Text preview.
      $('h2, #preview-page-title', form).css('color', $('#palette input[name="palette[title]"]', form).val());
      $('#preview .preview-content', form).css('color', $('#palette input[name="palette[text]"]', form).val());
      $('#preview #preview-content a', form).css('color', $('#palette input[name="palette[link]"]', form).val());

      // Header background.
      $('#preview #preview-header', form).css('background-color', $('#palette input[name="palette[header]"]', form).val());

      // Sidebar block.
      $('#preview #preview-sidebar #preview-block', form).css('background-color', $('#palette input[name="palette[sidebar]"]', form).val());

      // Footer background.
      $('#preview #preview-footer', form).css('background-color', $('#palette input[name="palette[footer]"]', form).val());
    }
  };
})(jQuery);
