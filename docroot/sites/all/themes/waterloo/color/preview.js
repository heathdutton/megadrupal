
(function ($) {
  Drupal.color = {
    logoChanged: false,
    callback: function(context, settings, form, farb, height, width) {

      // Main colour as foreground
      $('h1#preview-site-name, #preview-content h2', form).css('color', $('#palette input[name="palette[main]"]', form).val());

      // Main colour as background
      $('#preview-main-menu, #preview-content ul.links, #preview-sidebar-first input#edit-submit', form).css('backgroundColor', $('#palette input[name="palette[main]"]', form).val());

      // Body Text
      $('#preview-content p, #preview-sidebar-first label', form).css('color', $('#palette input[name="palette[text]"]', form).val());

      // Links
      $('#preview-content p a, #preview-sidebar-first a, #preview-lower-footer a', form).css('color', $('#palette input[name="palette[link]"]', form).val());
    }
  };
})(jQuery);
