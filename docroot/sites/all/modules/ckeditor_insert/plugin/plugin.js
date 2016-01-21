CKEDITOR.plugins.add('insert', {
  init : function(editor) {
    if (document.getElementById('ckeditor_file_insert_light')) {
      editor.addCommand('insertFileDialog', {
        exec : function(editor) {
          jQuery('#ckeditor_insert_fade').addClass('file-dialog-is-open');
          jQuery('#ckeditor_file_insert_light,#ckeditor_insert_fade').show();
        }
      });
      editor.ui.addButton('insertFileDialog', {
        label : 'Insert a file',
        command : 'insertFileDialog',
        icon : this.path + 'images/insertfile.png'
      });
    }
    if (document.getElementById('ckeditor_image_insert_light')) {
      editor.addCommand('insertImageDialog', {
        exec : function(editor) {
          jQuery('#ckeditor_insert_fade').addClass('image-dialog-is-open');
          jQuery('#ckeditor_image_insert_light,#ckeditor_insert_fade').show();
        }
      });
      editor.ui.addButton('insertImageDialog', {
        label : 'Insert an image',
        command : 'insertImageDialog',
        icon : this.path + 'images/insertimage.png'
      });
    }
  }
});
