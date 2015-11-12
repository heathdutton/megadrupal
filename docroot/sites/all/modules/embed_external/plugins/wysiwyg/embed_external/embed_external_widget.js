/**
 * @file
 * Provides a CKEditor widget plugin so that embedded content doesn't get messed up by editors.
 */
(function() {
  CKEDITOR.plugins.add('embed_external_widget', {
    requires: "widget",
    init: function (editor) {
      editor.widgets.add('embed-external-widget', {
        editables: {},
        template: '<div class="embed-external-wrapper"><iframe class="embed-external"></iframe></div>',
        allowedContent: 'div(!embed-external-wrapper)[*]',
        mask: true,

        upcast: function (element) {
          return element.name == 'div' && element.hasClass('embed-external-wrapper');
        },
        downcast: function (element) {
          return element;
        }
      });
    }
  });
})();
