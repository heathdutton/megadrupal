
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
      $('#preview', form).css('background-color', $('#palette input[name="palette[background]"]', form).val());

      // Text preview.
      $('#preview #preview-content', form).css('color', $('#palette input[name="palette[text]"]', form).val());
      $('#preview #preview-content a', form).css('color', $('#palette input[name="palette[link]"]', form).val());

      // Header background.
      $('#preview #preview-header', form).css('background-color', $('#palette input[name="palette[2nddarkestgray]"]', form).val());

      // Sidebar block.
      $('#preview #preview-sidebar #preview-block h2', form).css('background', $('#palette input[name="palette[3rdmediumgray]"]', form).val());
      $('#preview #preview-sidebar #preview-block', form).css('border-color', $('#palette input[name="palette[lightestgray]"]', form).val());

      // Footer wrapper background.
      $('#preview #preview-footer-wrapper', form).css('background-color', $('#palette input[name="palette[darkestgray]"]', form).val());

      // CSS3 Gradients.
      var gradient_start = $('#palette input[name="palette[2ndmediumgray]"]', form).val();
      var gradient_end = $('#palette input[name="palette[3rddarkestgray]"]', form).val();

      $('#preview #preview-header-top', form).attr('style', "background: " + gradient_start + "; background: -webkit-gradient(linear, 0% 0%, 0% 100%, from(" + gradient_start + "), to(" + gradient_end + ")); background: -moz-linear-gradient(-90deg, " + gradient_start + ", " + gradient_end + ");");

      $('#preview #preview-site-name', form).css('color', $('#palette input[name="palette[2ndlightestgray]"]', form).val());
    }
  };
})(jQuery);
