
/**
 * @file Plugin for inserting MediaCore tags with mediacorechooser
 */
(function ($) {
  function loadScript(url) {
    var script = document.createElement('script');
    script.src = url;
    (document.body || document.head || document.documentElement).appendChild(script);
  }

  CKEDITOR.plugins.add( 'mediacorechooser', {

    requires : [],

    init: function( editor ) {

      var t = this;
      t.editor = editor;

      var chooserJsUrl = t.editor.config['mediacore_chooser_js_url'];
      var chooserUrl = t.editor.config['mediacore_chooser_url'];

      loadScript(chooserJsUrl);
      var params = {
        'url': chooserUrl,
        'mode': 'popup'
      };

      // Add Button
      editor.ui.addButton( 'mediacorechooser', {
        label: 'MediaCore',
        command: 'mediacorechooser',
        icon: this.path + 'mcore-icon.png'
      });

      // Add Command
      editor.addCommand( 'mediacorechooser', {
        exec : function () {
          if (!window.mediacore) {
            alert('mediacore.loaderror. Ensure you have the correct MediaCore URL.');
            return;
          }
          if (!t.chooser) {
            t.chooser = mediacore.chooser.init(params);
            t.chooser.on('media', function(media) {
              insert(media.public_url, t.editor);
            });
            t.chooser.on('error', function(err) {
              throw err;
            });
          }
          t.chooser.open();
        }
      });
    }
  });

  function insert(url, editor) {
    var selection = editor.getSelection(),
    ranges    = selection.getRanges(),
    range, textNode;

    editor.fire( 'saveSnapshot' );

    var str = '[mediacore:' + url + ']';


    for ( var i = 0, len = ranges.length ; i < len ; i++ )
    {
      range = ranges[ i ];
      range.deleteContents();

      textNode = CKEDITOR.dom.element.createFromHtml( str );
      range.insertNode( textNode );
    }

    range.moveToPosition( textNode, CKEDITOR.POSITION_AFTER_END );
    range.select();

    editor.fire( 'saveSnapshot' );
  }

})(jQuery);
