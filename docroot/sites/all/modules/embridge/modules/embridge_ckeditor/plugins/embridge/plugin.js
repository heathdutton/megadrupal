/**
Copyright (c) 2003-2013, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

/**
 * @file Plugin for inserting images from Drupal embridge module
 */
(function() {
  CKEDITOR.plugins.add('embridge',
  {
    requires: [ 'iframedialog' ],
    // Wrap Drupal plugin in a proxy plugin.
    init: function(editor)
    {
      editor.addCommand('embridgeDialog', new CKEDITOR.dialogCommand('embridgeDialog'));
      editor.ui.addButton('embridge',
      {
        label: 'Add EnterMedia Image',
        command: 'embridgeDialog',
        icon: this.path + 'images/icon.png',
        toolbar: 'insert'
      });
      CKEDITOR.dialog.add('embridgeDialog', this.path + 'library.js');
    }
  });
})();
