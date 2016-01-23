/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.overlay = {
    show: function(options, title) {
      title = title || '';
      var width = $(window).width() - 40, height = $(window).height() - 40;
      if ($.isEmptyObject(options)) {
        var $modal = $('<div>').addClass('imageeditor-modal').appendTo('body');
        $.fancybox.open({
          type: 'html',
          content: $modal,
          title: Drupal.t('Image Editor') + (title ? ': ' + title : ''),
          width: width,
          height: height,
          autoSize: false,
          afterClose: function() {
            $modal.remove();
          }
        });
      }
      else {
        $.fancybox.open({
          type: 'iframe',
          href: Drupal.imageeditor.buildUrl(options),
          title: Drupal.t('Image Editor') + (title ? ': ' + title : ''),
          width: width,
          height: height
        });
      }
    },
    hide: function() {
      $.fancybox.close();
    }
  };
})(jQuery);
