/*global Drupal: false, jQuery: false, CKEDITOR: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  CKEDITOR.plugins.add('imageeditor', {
    init: function (editor, pluginPath) {
      editor.addCommand('imageeditor',{
        exec: function (editor){
          var selection = editor.getSelection();
          console.log(selection.getSelectedElement().getAttribute('src'));
        }
      });

      var items = {};
      $.each(Drupal.settings.imageeditor_inline.editors, function(index, element) {
        items[index] = CKEDITOR.TRISTATE_OFF;
      });

      var menuitems = {};
      menuitems.imageeditor = {
        label: 'Image Editor',
        //command: 'imageeditor',
        group: 'imageeditor',
        getItems: function() {
          return items;
        }
        //icon: this.path + 'imageeditor.png'
      };

      $.each(Drupal.settings.imageeditor_inline.editors, function(index, element) {
        menuitems[index] = {
          label: Drupal.settings.imageeditor[index].name,
          command: 'imageeditor',
          group: 'imageeditor'
        };
      });

      //Create menu items.
      if (editor.addMenuItems) {
        editor.addMenuGroup('imageeditor', 100);
        editor.addMenuItems(menuitems);
      }

      //Add behaviour if context menu opens up.
      if (editor.contextMenu) {
        //function to be run when context menu is displayed
        editor.contextMenu.addListener(function (element, selection) {
          if ( !element || !element.is( 'img' ) || element.data( 'cke-realelement' ) || element.isReadOnly() ) {
            return null;
          }
          return { imageeditor : CKEDITOR.TRISTATE_OFF };
        });
      }
    }
  });
})(jQuery);
