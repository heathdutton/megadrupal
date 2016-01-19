
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
      $('#preview', form).css('backgroundColor', $('#palette input[name="palette[bg]"]', form).val());

      // Horizontal lines.
      $('#preview #preview-main-menu-links, #preview #preview-footer', form).css('border-color', $('#palette input[name="palette[highlight]"]', form).val());

      // Text preview (headings, content and links).
      $('#preview #preview-main .content, #preview #preview-footer .content', form).css('color', $('#palette input[name="palette[text]"]', form).val());
      $('#preview #preview-main #preview-page-title, #preview #preview-footer #preview-block-title', form).css('color', $('#palette input[name="palette[headings]"]', form).val());
      $('#preview #preview-main a', form).css('color', $('#palette input[name="palette[link]"]', form).val());

      // Link hovering.
      $('#preview #preview-main .content a').hover(function() {
        $(this).css('color', $('#palette input[name="palette[hover]"]', form).val());
      },
      function() {
        $(this).css('color', $('#palette input[name="palette[link]"]', form).val());
      });

      // Menu tabs - hover (jQuery can't add to :hover pseudo-class).
      $('#preview-main-menu-links a').hover(function() {
        $(this).css('background-color', $('#palette input[name="palette[highlight]"]', form).val());
      },
      function() {
        $(this).css('background-color', '#CCCCCC');
      });
    }
  };
})(jQuery);
