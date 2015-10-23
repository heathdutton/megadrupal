/**
 * @file
 * Contains Local Storage CKeditor plugin.
 */

(function ($) {
  CKEDITOR.plugins.add('html5ls',
    {
      icons: 'html5ls',
      init: function (editor) {
        var $el = $(editor.element.$);
        var html5ls = $el.html5ls();
        var html5lsCkeditor = $el.html5lsCkeditor();

        // Hide Local Storage Plain plugin if exists.
        if ($el.html5lsPlain) {
          $el.html5lsPlain().hide();
        }

        // Add command at first.
        editor.addCommand('html5ls', {
          exec: html5lsCkeditor.invoke.bind(html5lsCkeditor)
        });

        // Add button.
        editor.ui.addButton('html5ls',
          {
            label: Drupal.t('Local Storage'),
            command: 'html5ls',
            icon: this.path + 'images/local-storage.png'
          });

        // Set initial state for button.
        editor.getCommand('html5ls').setState(
          html5ls.getState()
            ? CKEDITOR.TRISTATE_OFF
            : CKEDITOR.TRISTATE_ON
        );

        if (editor.dataProcessor) {
          editor.dataProcessor.toDataFormat = CKEDITOR.tools.override(editor.dataProcessor.toDataFormat, function (originalToDataFormat) {
            // Convert WYSIWYG mode content to raw data.
            return function (data, fixForBody) {
              data = originalToDataFormat.call(this, data, fixForBody);
              html5lsCkeditor.detach();
              return data;
            };
          });
        }
      }
    });
  
  
})(jQuery);
