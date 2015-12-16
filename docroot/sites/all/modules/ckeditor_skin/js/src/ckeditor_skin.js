(function() {
  "use strict";

  // Override defaul functions.
  CKEDITOR.getUrlDefault = CKEDITOR.getUrl;
  CKEDITOR.skins.loadDefault = CKEDITOR.skins.load;

  CKEDITOR.getSkinUrl = function(resource) {
    var result = /^(.*\/skins\/([a-zA-Z0-9_-]+)\/).*\.css$/.exec(resource);

    if (result !== null) {
      var skinPath = result[1];
      var skinName = result[2];

      if (typeof Drupal.settings.ckeditor.skin_paths !== 'undefined' && (skinName in Drupal.settings.ckeditor.skin_paths)) {
        var realPath = Drupal.settings.ckeditor.skin_paths[skinName];
        resource = resource.replace(skinPath, realPath);
      }
    }

    return CKEDITOR.getUrlDefault(resource);
  };

  // Hijack the default load function to check for custom skin path.
  CKEDITOR.skins.load = function(editor, skinPart, callback) {
    CKEDITOR.getUrl = CKEDITOR.getSkinUrl;
    CKEDITOR.skins.loadDefault(editor, skinPart, callback);
    CKEDITOR.getUrl = CKEDITOR.getUrlDefault;
  };
}) ();
