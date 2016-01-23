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
        $.colorbox({
          inline: true,
          href: $modal,
          title: Drupal.t('Image Editor') + (title ? ': ' + title : ''),
          width: width,
          height: height,
          fixed: true,
          onClosed: function() {
            $modal.remove();
          }
        });
      }
      else {
        $.colorbox({
          iframe: true,
          href: Drupal.imageeditor.buildUrl(options),
          title: Drupal.t('Image Editor') + (title ? ': ' + title : ''),
          width: width,
          height: height,
          fixed: true
        });
      }
    },
    hide: function() {
      $.colorbox.close();
    }
  };
})(jQuery);
