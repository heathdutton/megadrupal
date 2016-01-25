(function ($, Drupal) {
  Drupal.behaviors.wysiwyg_codesnippet = {
    attach: function (context, settings) {
      $('pre code').each(function(i, e) {hljs.highlightBlock(e)});

      // Force overwrite of 'default' in the CKEDITOR global scope
      if (typeof CKEDITOR !== 'undefined') {
        CKEDITOR.config.codeSnippet_theme = Drupal.settings.wysiwyg.codeSnippet_theme;
      }
    }
  }
})(jQuery, Drupal);