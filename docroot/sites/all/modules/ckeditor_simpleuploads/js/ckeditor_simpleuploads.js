/**
 * @file
 * Remove width and height from inline images added by simpleuploads plugin.
 */


jQuery(document).ready(function() {

  if (typeof CKEDITOR !== 'undefined') {

    CKEDITOR.on('instanceReady', function(e) {
      var editor = e.editor;

      // When the upload has finished the plugin has finished
      // and the element is ready on the page.
      editor.on('simpleuploads.finishedUpload' , function(ev) {
        var element = ev.data.element;
        element.addClass("img-responsive");

        element.removeAttribute("width");
        element.removeAttribute("height");
      });
    });
  }
});
