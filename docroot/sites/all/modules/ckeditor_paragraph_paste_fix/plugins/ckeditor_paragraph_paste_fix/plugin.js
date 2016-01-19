/**
 * @file
 * Written by Albert Jankowski - https://www.drupal.org/user/1327432
 */

(function() {
  /**
   * Check for a paste event and replace <p>&nbsp;</p> with blank
   */
  CKEDITOR.on('instanceReady', function(e) {
    e.editor.on('paste', function(ev) {
      // Replaces <p>&nbsp;</p> with ''
      ev.data.dataValue = ev.data.dataValue.replace(/&nbsp;/g,'');
      // Replaces <p><br></p> with ''
       ev.data.dataValue = ev.data.dataValue.replace(/<p><br><\/p>/g,'');
    }, null, null, 9);
  });

  CKEDITOR.plugins.add('ckeditor_paragraph_paste_fix', {
  });
})();
