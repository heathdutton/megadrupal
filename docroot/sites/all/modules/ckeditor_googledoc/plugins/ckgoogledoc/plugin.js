(function() {

CKEDITOR.plugins.add('ckgoogledoc',
{
  init: function(editor)
  {
    /* COMMAND */
    editor.addCommand('insertGoogledoc', new CKEDITOR.dialogCommand('ckgoogledocDialog'));

    /* BUTTON */
    editor.ui.addButton('CKGoogledoc',
    {
      label: 'Google docs',
      command: 'insertGoogledoc',
      icon: this.path + 'ckgoogledoc.png'
    });

    /* DIALOG */
    CKEDITOR.dialog.add('ckgoogledocDialog', function (editor)
    {
      return {
        title : 'Embedd Google Docs iFrame',
        minWidth : 300,
        minHeight : 200,
        contents :
        [{
          id : 'tab1',
          label : 'Settings',
          elements :
          [{
            type : 'text',
            id : 'url',
            label : 'Document URL',
            validate : CKEDITOR.dialog.validate.notEmpty("Document URL should be provided and has to be a well formated URL")
          },
          {
            type : 'text',
            id : 'width',
            label : 'Document Width in pixels',
            onShow : function() { this.setValue('600'); },
            validate : CKEDITOR.dialog.validate.notEmpty("Document width should be provided")
          },
          {
            type : 'text',
            id : 'height',
            label : 'Document Height in pixels',
            onShow : function() { this.setValue('780'); },
            validate : CKEDITOR.dialog.validate.notEmpty("Document height should be provided")
          }]
        }],
        onOk : function()
          {
            var dialog = this;
            var url = dialog.getValueOf('tab1', 'url');
            var width = dialog.getValueOf('tab1', 'width');
            var height = dialog.getValueOf('tab1', 'height');

            var inplaceTag = '<iframe src="http://docs.google.com/viewer?url=' + encodeURIComponent(url) + '&embedded=true" width="' + parseInt(width) + '" height="' + parseInt(height) + '" style="border: none;">';

            editor.insertHtml(inplaceTag);
          }
        };
//dialog.add
      });
//init:
    }
//plugin.add
  });
})();
