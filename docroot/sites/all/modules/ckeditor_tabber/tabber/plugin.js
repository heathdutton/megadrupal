(function () {
  // Register plugin
  CKEDITOR.plugins.add('tabber', {
    init: function (editor) {

      // Add single button
      editor.ui.addButton('addTab', {
        command: 'addTab',
        icon: this.path + 'icons/tabber.png',
        label: Drupal.t('Insert tabs')
      });

      // Add CSS for edition state
      var cssPath = this.path + 'tabber.css';
      editor.on('mode', function () {
        if (editor.mode == 'wysiwyg') {
          this.document.appendStyleSheet(cssPath);
        }
      });

      // Prevent nesting DLs by disabling button
      editor.on('selectionChange', function (evt) {
        if (editor.readOnly)
          return;
        var command = editor.getCommand('addTab'),
          element = evt.data.path.lastElement && evt.data.path.lastElement.getAscendant('dl', true);
        if (element)
          command.setState(CKEDITOR.TRISTATE_DISABLED);
        else
          command.setState(CKEDITOR.TRISTATE_OFF);
      });

      var allowedContent = 'dl dd dt (ckeditor-tabber)';

      // Command to insert initial structure
      editor.addCommand('addTab', {
        allowedContent: allowedContent,

        exec: function (editor) {
          var dl = new CKEDITOR.dom.element.createFromHtml(
            '<dl class="ckeditor-tabber">' +
            '<dt>Tab title 1</dt>' +
            '<dd><p>Tab content 1.</p></dd>' +
            '<dt>Tab title 2</dt>' +
            '<dd><p>Tab content 2.</p></dd>' +
            '</dl>');
          editor.insertElement(dl);
        }
      });

      // Other command to manipulate tabs
      editor.addCommand('addTabBefore', {
        allowedContent: allowedContent,

        exec: function (editor) {
          var element = editor.getSelection().getStartElement();
          var newHeader = new CKEDITOR.dom.element.createFromHtml('<dt>New tab title</dt>');
          var newContent = new CKEDITOR.dom.element.createFromHtml('<dd><p>New tab content</p></dd>');
          if (element.getAscendant('dd', true)) {
            element = element.getAscendant('dd', true).getPrevious();
          }
          else {
            element = element.getAscendant('dt', true);
          }
          newHeader.insertBefore(element);
          newContent.insertBefore(element);
        }
      });
      editor.addCommand('addTabAfter', {
        allowedContent: allowedContent,
        
        exec: function (editor) {
          var element = editor.getSelection().getStartElement();
          var newHeader = new CKEDITOR.dom.element.createFromHtml('<dt>New tab title</dt>');
          var newContent = new CKEDITOR.dom.element.createFromHtml('<dd><p>New tab content</p></dd>');
          if (element.getAscendant('dt', true)) {
            element = element.getAscendant('dt', true).getNext();
          }
          else {
            element = element.getAscendant('dd', true);
          }
          newContent.insertAfter(element);
          newHeader.insertAfter(element);
        }
      });
      editor.addCommand('removeTab', {
        exec: function (editor) {
          var element = editor.getSelection().getStartElement();
          if (element.getAscendant('dt', true)) {
            var a = element.getAscendant('dt', true);
            a.getNext().remove();
            a.remove();
          }
          else {
            var a = element.getAscendant('dd', true);
            a.getPrevious().remove();
            a.remove();
          }
        }
      });

      // Context menu
      if (editor.contextMenu) {
        editor.addMenuGroup('tabberGroup');
        editor.addMenuItem('tabBeforeItem', {
          label: Drupal.t('Add tab before'),
          icon: this.path + 'icons/tabber.png',
          command: 'addTabBefore',
          group: 'tabberGroup'
        });
        editor.addMenuItem('tabAfterItem', {
          label: Drupal.t('Add tab after'),
          icon: this.path + 'icons/tabber.png',
          command: 'addTabAfter',
          group: 'tabberGroup'
        });
        editor.addMenuItem('removeTab', {
          label: Drupal.t('Remove tab'),
          icon: this.path + 'icons/tabber.png',
          command: 'removeTab',
          group: 'tabberGroup'
        });

        editor.contextMenu.addListener(function (element) {
          if (element.getAscendant('dl', true)) {
            return {
              tabBeforeItem: CKEDITOR.TRISTATE_OFF,
              tabAfterItem: CKEDITOR.TRISTATE_OFF,
              removeTab: CKEDITOR.TRISTATE_OFF
            };
          }
        });
      }
    }
  });
})();
