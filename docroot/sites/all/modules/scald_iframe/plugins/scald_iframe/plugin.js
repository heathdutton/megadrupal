(function ($) {
    CKEDITOR.plugins.add('scald_iframe', {
        init: function (editor, pluginPath) {
            // Make CKEditor use the scald_iframe styles so iframe atoms will get
            // displayed properly in the editor.
            editor.config.contentsCss.push(Drupal.settings.basePath + 'profiles/foundation/modules/custom/scald_iframe/scald_iframe.css')
        }
    });
})(jQuery);
