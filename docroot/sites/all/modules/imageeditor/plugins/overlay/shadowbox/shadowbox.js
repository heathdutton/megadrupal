/*global Drupal: false, jQuery: false, Shadowbox: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.overlay = {
    show: function(options, title) {
      title = title || '';
      var width = $(window).width() - 40, height = $(window).height() - 40;
      if ($.isEmptyObject(options)) {
        var $modal = $('<div>').addClass('imageeditor-modal').attr({id: 'imageeditor_modal', style: 'display: none;'}).appendTo('body');
        Shadowbox.open({
          player: 'inline',
          //content: $modal,
          content: '#imageeditor_modal',
          title: Drupal.t('Image Editor') + (title ? ': ' + title : ''),
          width: width,
          height: height,
          options: {
            onClose: function() {
              $modal.remove();
            }
          }
        });
      }
      else {
        Shadowbox.open({
          player: 'iframe',
          content: Drupal.imageeditor.buildUrl(options),
          title: Drupal.t('Image Editor') + (title ? ': ' + title : ''),
          width: width,
          height: height
        });
      }
    },
    hide: function() {
      Shadowbox.close();
    }
  };
})(jQuery);
