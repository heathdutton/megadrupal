// Custom CKEditor config; don't let it mangle our markup.

CKEDITOR.editorConfig = function(config) {
  config.entities = false;
  config.basicEntities = false;
  config.htmlEncodeOutput = false;
}
