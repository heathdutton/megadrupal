/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.overlay = {
    show: function(options, title) {
      title = title || '';
      var $modal = $('<div>').addClass('imageeditor-modal').appendTo('body'),
        $iframe = $('<iframe>'), width = 'auto', height = 'auto';
      if ($.isEmptyObject(options)) {
        width = $(window).width() - 80;
        height = $(window).height() - 80;
      }
      else {
        $iframe.attr({
          width: ($(window).width() - 80) + 'px',
          height: ($(window).height() - 160) + 'px',
          src: Drupal.imageeditor.buildUrl(options)
        });
        $iframe.appendTo($modal);
      }
      $modal.dialog({
        title: Drupal.t('Image Editor') + (title ? ': ' + title : ''),
        width: width,
        height: height,
        modal: true,
        //draggable: false,
        close: function(event, ui) {
          $modal.dialog('destroy').remove();
        }
      });
    },
    hide: function() {
      $('.imageeditor-modal').dialog('close');
    }
  };
})(jQuery);
