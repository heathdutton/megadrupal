
(function($) {

  // Don't check for Drupal.visualSelectFile. If should exist. If it doesn't, it should crash(TM).

  // After clicking an image in the Views grid.
  var _onEmbed = Drupal.visualSelectFile.onEmbed;
  Drupal.visualSelectFile.onEmbed = function($td, editor, owner) {
    var file = Drupal.visualSelectFile.extractTDMetaInfo($td);
    if (!file) {
      return;
    }

    var format = $td.closest('.view').find('#edit-vsf-formatter').val();
    if (!format) {
      // Let VSF handle error message.
      return _onEmbed.apply(this, arguments);
    }

    // 'Parse' format.
    var style = format.split(':').pop();

    // Check 2 factors:
    // - Style requires crop.
    // - Image has no cropping for this style.
    var isCropStyle = Drupal.settings.visual_select_file.crop_styles.indexOf(style) != -1;
    var hasCropStyle = file[4].indexOf(style) != -1;
    if (isCropStyle && !hasCropStyle) {
      // Redirect to manualcrop form, which will redirect back here.
      var uri = 'file/' + file[0] + '/vsf_manualcrop/' + encodeURIComponent(style);

      var from = Drupal.settings.vsf_manualcrop.query;
      from['vsf_formatter'] = format;
      from['vsf_fid'] = file[0];
      var query = {
        "iframe": "",
        "manualcrop": "",
        "vsf_from": JSON.stringify(from)
      };

      location = Drupal.settings.basePath + uri + '?' + jQuery.param(query);
      return;
    }

    // All is good, do normal VSF embed stuff.
    return _onEmbed.apply(this, arguments);
  };

})(jQuery);
