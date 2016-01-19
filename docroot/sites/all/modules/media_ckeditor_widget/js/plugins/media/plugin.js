/*
Copyright (c) 2003-2013, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/
(function() {
  var mediaPluginDefinition = {
    icons: 'media',
    requires: 'button,widget',

    init: function(editor) {
        // Make an intances object so media wysiwyg can use it.
      Drupal.wysiwyg.instances[editor.name] = {
        insert: function(markup) {
          var editorElement = CKEDITOR.dom.element.createFromHtml(markup);
          CKEDITOR.instances[editor.name].insertElement(editorElement);
          CKEDITOR.instances[editor.name].widgets.initOn(editorElement, 'mediabox');
        }
      };
      // Add Editor command.
      editor.addCommand('media', {
        // Pattern any class (*), any style {*} and any attribute [*]
        allowedContent: 'img(*)[*]',
        exec: function(editor) {
          var data = {
            format: 'html',
            node: null,
            content: ''
          };
          var selection = editor.getSelection();

          if (selection) {
            data.node = selection.getSelectedElement();
            if (data.node) {
              data.node = data.node.$;
            }
            if (selection.getType() == CKEDITOR.SELECTION_TEXT) {
              if (CKEDITOR.env.ie && CKEDITOR.env.version < 10) {
                data.content = selection.getNative().createRange().text;
              } else {
                data.content = selection.getNative().toString();
              }
            } else if (data.node) {
              data.content = data.node.parentNode.innerHTML;
            }
          }
          var settings = {
            global: {}
          }; // media wysiwyg needs it for some reason.
          Drupal.wysiwyg.plugins.media.invoke(data, settings, editor.name);
        }
      });
      // Add UI button
      editor.ui.addButton('Media', {
        label: 'Add media',
        command: 'media',
        icon: this.path + 'images/icon.png'
      });

      // Create widget
      editor.widgets.add('mediabox', {
        button: 'Create a mediabox',
        editables: {},
        allowedContent: '*',
        upcast: function(element) {
          return element.attributes['data-media-element'] && element.hasClass('media-element');
        },
        // Create token value.
        downcast: function(element) {
          element.attributes.width = null;
          element.attributes.height = null;
          var token = Drupal.wysiwyg.plugins.media.detach(element.getOuterHtml());
          text = new CKEDITOR.htmlParser.text(token);
          return text;
        }
      });
      // prepare data for upcast.
      editor.on('setData', function(e) {
        e.data.dataValue = Drupal.wysiwyg.plugins.media.attach(e.data.dataValue);
      });
      // On doubleclick select media and edit.
      editor.on('doubleclick', function(e) {
        if(e.data.element.hasAttribute('data-media-element')) {
          var data = {
            format: 'html',
            node: e.data.element.$,
            content: ''
          };
          var settings = {
            global: {}
          }; // media wysiwyg needs it for some reason.
          Drupal.wysiwyg.plugins.media.invoke(data, settings, editor.name);
        }
      });
    }
  };

  CKEDITOR.plugins.add('media', mediaPluginDefinition);
})();
