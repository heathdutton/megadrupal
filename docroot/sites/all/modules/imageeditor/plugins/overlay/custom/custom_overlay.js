/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.overlay = {
    show: function(options) {
      $('<div>').addClass('imageeditor-external').appendTo('body');
      var $mdiv = $('<div>').addClass('imageeditor-modal').attr({
        width: ($(window).width() - 80) + 'px',
        height: ($(window).height() - 80) + 'px'
      }).appendTo('body'),
      $div_close = $('<div>').addClass('imageeditor-close').appendTo($mdiv);

      if (!$.isEmptyObject(options)) {
        var $iframe = $('<iframe>').addClass('imageeditor-iframe').attr({
          width: ($(window).width() - 80) + 'px',
          height: ($(window).height() - 80) + 'px',
          src: Drupal.imageeditor.buildUrl(options)
        }).appendTo($mdiv);
      }

      $div_close.click(
        function() {
          Drupal.imageeditor.overlay.hide();
        }
      );
    },
    hide: function() {
      $('.imageeditor-close').remove();
      $('.imageeditor-modal').remove();
      $('.imageeditor-external').remove();
    }
  };
})(jQuery);
