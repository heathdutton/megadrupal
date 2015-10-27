var ViewsFieldTooltip = ViewsFieldTooltip || { Library: {} };

(function ($) {

  // Add AJAX settings to qtip settings.
  ViewsFieldTooltip.Library.ajaxify = function(qtip, url) {
    qtip = qtip || {};
    $.extend(qtip, {
      content: {
        text: $('<iframe>', { src: url, frameborder: 0, width: "100%", height: "100%", seamless: "seamless" })
      }
    });
    return qtip;
  };

})(jQuery);
