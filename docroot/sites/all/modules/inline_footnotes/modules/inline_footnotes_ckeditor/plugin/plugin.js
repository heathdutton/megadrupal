/**
 * A CKeditor plugin to insert footnotes as in-place <fn> elements (consumed by Footnotes module in Drupal).
 *
 * This is a rather sophisticated plugin to show a dialog to insert
 * <fn> footnotes or edit existing ones. It produces and understands
 * the <fn>angle bracket</fn> variant and uses the fakeObjects API to
 * show a nice icon to the user, while producing proper <fn> tags when
 * the text is saved or View Source is pressed.
 *
 * If a text contains footnotes of the [fn]square bracket[/fn] variant,
 * they will be visible in the text and this plugin will not react to them.
 *
 * This plugin uses Drupal.t() to translate strings and will not as such
 * work outside of Drupal. (But removing those functions would be the only
 * change needed.) While being part of a Wysiwyg compatible module, it could
 * also be used together with the CKEditor module.
 *
 * Drupal Wysiwyg requirement: The first argument to CKEDITOR.plugins.add()
 * must be equal to the key used in $plugins[] in hook_wysiwyg_plugin().
 */
(function () {
    CKEDITOR.plugins.add('footnotes', {
        requires: ['fakeobjects', 'dialog'],
        icons: 'footnotes',
        onLoad: function () {
            CKEDITOR.addCss(
                    '.cke_footnote' +
                    '{' +
                    'background-image: url(' + CKEDITOR.getUrl(this.path + 'images/fn_icon2.png') + ');' +
                    'background-position: center center;' +
                    'background-repeat: no-repeat;' +
                    'width: 16px;' +
                    'height: 16px;' +
                    '}'
                    );
        },
        init: function (editor) {
            editor.addCommand('createfootnotes', new CKEDITOR.dialogCommand('createfootnotes', {
                allowedContent: 'fn[value]'
            }));
            editor.addCommand('editfootnotes', new CKEDITOR.dialogCommand('editfootnotes', {
                allowedContent: 'fn[value]'
            }));

            // Drupal Wysiwyg requirement: The first argument to editor.ui.addButton()
            // must be equal to the key used in $plugins[<pluginName>]['buttons'][<key>]
            // in hook_wysiwyg_plugin().
            editor.ui.addButton && editor.ui.addButton('footnotes', {
                label: 'Add a footnote',
                command: 'createfootnotes',
                icon: 'footnotes'
            });

            if (editor.addMenuItems) {
                editor.addMenuGroup('footnotes', 100);
                editor.addMenuItems({
                    footnotes: {
                        label: 'Edit footnote',
                        command: 'editfootnotes',
                        icon: 'footnotes',
                        group: 'footnotes'
                    }
                });
            }
            if (editor.contextMenu) {
                editor.contextMenu.addListener(function (element, selection) {
                    if (!element || element.data('cke-real-element-type') != 'fn')
                        return null;

                    return {footnotes: CKEDITOR.TRISTATE_ON};
                });
            }

            editor.on('doubleclick', function (evt) {
                if (CKEDITOR.plugins.footnotes.getSelectedFootnote(editor))
                    evt.data.dialog = 'editfootnotes';
            });

            CKEDITOR.dialog.add('createfootnotes', this.path + 'dialogs/footnotes.js');
            CKEDITOR.dialog.add('editfootnotes', this.path + 'dialogs/footnotes.js');
        },
        afterInit: function (editor) {
            var dataProcessor = editor.dataProcessor,
                    dataFilter = dataProcessor && dataProcessor.dataFilter;

            if (dataFilter) {
                dataFilter.addRules({
                    elements: {
                        fn: function (element) {
                            return editor.createFakeParserElement(element, 'cke_footnote', 'fn', false);
                        }
                    }
                });
            }
        }
    });
})();

CKEDITOR.plugins.footnotes = {
    createFootnote: function (editor, origElement, text, value) {
        if (!origElement) {
            var realElement = CKEDITOR.dom.element.createFromHtml('<fn></fn>');
        }
        else {
            realElement = origElement;
        }

        if (text && text.length > 0)
            realElement.setHtml(text);
        if (value && value.length > 0)
            realElement.setAttribute('value', value);

        var fakeElement = editor.createFakeElement(realElement, 'cke_footnote', 'fn', false);
        editor.insertElement(fakeElement);
    },
    getSelectedFootnote: function (editor) {
        var selection = editor.getSelection();
        var element = selection.getSelectedElement();
        var seltype = selection.getType();

        if (seltype == CKEDITOR.SELECTION_ELEMENT && element.data('cke-real-element-type') == 'fn') {
            return element;
        }
    }
};