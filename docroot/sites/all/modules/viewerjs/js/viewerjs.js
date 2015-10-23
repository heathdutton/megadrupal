/**
 * @file
 * File previewer using ViewerJS Library.
 */

(function ($) {
  /**
   * Center an element in the absolute middle of the screen.
   */
  $.fn.viewerjsCenterElement = function () {
    var paddingLeft = parseInt($(this).css('padding-left'), 10);
    var paddingRight = parseInt($(this).css('padding-right'), 10);
    var paddingTop = parseInt($(this).css('padding-top'), 10);
    var paddingBottom = parseInt($(this).css('padding-bottom'), 10);
    var borderLeft = parseInt($(this).css('borderLeftWidth'), 10);
    var borderRight = parseInt($(this).css('borderRightWidth'), 10);
    var borderTop = parseInt($(this).css('borderTopWidth'), 10);
    var borderBottom = parseInt($(this).css('borderBottomWidth'), 10);
    var left = parseInt($(window).width() / 2, 10) - parseInt(($(this).width() + paddingLeft + paddingRight + borderLeft + borderRight) / 2, 10);
    var top = parseInt($(window).height() / 2, 10) - parseInt(($(this).height() + paddingBottom + paddingTop + borderBottom + borderTop) / 2, 10);
    $(this).css({left: left + "px", top: top + "px", position: "fixed"});
    return $(this);
  };

  $.fn.viewerjsCreateOverlay = function () {
    if (!$(this).hasClass('viewerjs-processed')) {
      $(this).addClass('viewerjs-processed').click(function (e) {
        e.preventDefault();
        var id = $(this).attr('data-viewerjs-overlay-id');
        // Create the background.
        var fog = $('<div id="' + id + '-fog" class="viewerjs-overlay-fog"></div>');
        fog.appendTo("body").css({
          'top': '0px',
          'left': '0px',
          'width': $('body').outerWidth(true) + 'px',
          'height': $('body').outerHeight(true) + 'px',
          'position': 'fixed'
        });
        var overlay = $('<div class="viewerjs-overlay-element"></div>').attr({
          id: id
        }).css({
          width: $(this).attr('data-viewerjs-overlay-width'),
          height: $(this).attr('data-viewerjs-overlay-height')
        });
        overlay.html('<iframe id="' + id + '-iframe" src="' + $(this).attr('href') + '" width="' + $(this).attr('data-viewerjs-overlay-width') + '" height="' + $(this).attr('data-viewerjs-overlay-height') + '" allowfullscreen webkitallowfullscreen></iframe>');
        overlay.appendTo("body").viewerjsCenterElement();
        $(fog).click(function () {
          $(fog).remove();
          $(overlay).remove();
        });
        return overlay;
      });
    }
  };

  Drupal.behaviors.viewerjs = {
    attach: function (context, settings) {
      $("a.viewerjs-overlay").viewerjsCreateOverlay();
    }
  };

})(jQuery);
