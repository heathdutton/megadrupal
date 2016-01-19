/**
 * @file
 * Plugin for inserting images with visual_select_file.
 */

(function($) {
  Drupal.visualSelectFile || (Drupal.visualSelectFile = {});

  CKEDITOR.plugins.add('visual_select_file', {
    "requires": ['fakeobjects'],
    "init": function(editor) {

      // If the VSF JS didn't load (probably because the user doesn't have access), don't show the button.
      if (!Drupal.visualSelectFile.openModal) {
        return;
      }

      var
        $textarea = $('#' + editor.name),
        field = $textarea.data('vsf-field');
      editor.vsf_field = field;

      // Add Button.
      editor.ui.addButton('visual_select_file', {
        "label": 'visual_select_file',
        "command": 'visual_select_file',
        "icon": this.path + 'visual_select_file.png'
      });

      // Add Command.
      editor.addCommand('visual_select_file', {
        exec: function(editor) {
          var path = Drupal.settings.basePath + 'admin/visual_select_file';
          path += '?ckeditor&iframe&vsf_field=' + editor.vsf_field + '#modal';

          Drupal.visualSelectFile.editor = editor;

          // Method 3 - in an iframe.
          Drupal.visualSelectFile.openModal(path, {
            "type": 'ckeditor',
            "field": editor.vsf_field,
          });
        }
      });

    }
  });

})(jQuery);
