/**
 * Dialog.
 */

CKEDITOR.dialog.add( 'drupalimagefromwordDialog', function(editor) {
  return {
    title : Drupal.t('Error | Image from Word'),
    minWidth : 400,
    minHeight : 200,
    buttons: [CKEDITOR.dialog.okButton],
    contents : [
    {
      id : 'errors',
      label : Drupal.t('Embed media code'),
      title : Drupal.t('Embed media code'),
      elements :
      [
      {
        type : 'html',
        html : '<div id="messages-wrapper"></div>'
      }
      ]
    },
    ],
    onShow: function(dialog) {
      var messagesWrapper = jQuery(this.getElement().$).find('#messages-wrapper');
      jQuery(messagesWrapper).html('');
      // Update content of the dialog window.
      var config = dialog.sender._.editor.config;

      // If we have errors.
      if (config.drupalImageFromWord.errors) {
        jQuery.each(config.drupalImageFromWord.errors, function(key, error) {
          jQuery(messagesWrapper).append('<div class="messages error">' + error + '</div>');
        });
      }
      // If we have message.
      if (config.drupalImageFromWord.message) {
          jQuery(messagesWrapper).append('<div class="messages warning">' + config.drupalImageFromWord.message + '</div>');
      }
    }
  };
});

