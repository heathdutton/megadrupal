/**
 * @file
 * Canvas js integration.
 */

(function ($) {
  Drupal.behaviors.drawingfield = {
    attach: function(context, settings) {
      settings = Drupal.settings.drawingfield;
      var imageSize = {width: settings.width, height: settings.height};
      var lc = LC.init(
      document.getElementsByClassName('literally export')[0],{imageSize: imageSize,backgroundColor: settings.backgroundColor,imageURLPrefix: settings.imageUrlPrefix});
      var localStorageKey = 'drawing'
      json = settings.drawingEditPath;
      localStorage.setItem(localStorageKey, json);
      if (localStorage.getItem(localStorageKey)) {
        lc.loadSnapshotJSON(localStorage.getItem(localStorageKey));
      }
      lc.on('drawingChange', function() {
        json = lc.getSnapshotJSON();
        var base64 = lc.getImage().toDataURL();
        var jsonBase64 = json + 'JSON' + base64;
        paintId = $(".form-group").find("input.output").attr('id');
        $("#" + paintId).val(jsonBase64);
      });
    }
  }
})(jQuery);
