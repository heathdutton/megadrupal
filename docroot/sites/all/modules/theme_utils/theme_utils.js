(function($) {

Drupal.behaviors.themeUtils = {
  attach: function (context, settings) {

    var hoverText = Drupal.t('Click to hide');

    $('body').append('<div id="theme-utils" />');

    // Browser viewport
    if (settings.themeUtils.browserViewport && settings.themeUtils.browserViewport.show == 'true') {
      $('#theme-utils').append('<div id="theme-utils-browser-viewport" class="theme-utils-box" title="' + hoverText + '"></div>');

      // Calculate the browser width.
      function showWidth() {
        var widthPx = $(window).width();
        var widthEm = widthPx / settings.themeUtils.baseFontSize.size;
        $('#theme-utils-browser-viewport').html(widthPx + 'px / ' + widthEm + 'em');
      }
      showWidth();

      $(window).resize(function() {
        showWidth();
      });

      // Hide the viewport indicator by clicking on it.
      $('#theme-utils-browser-viewport').click(function() {
        $(this).fadeOut(500);
      });
    }

    // Media queries
    if ($(settings.themeUtils.mediaQueries).length) {
      $('#theme-utils').append('<div id="theme-utils-media-query" class="theme-utils-box" title="' + hoverText + '"></div>');

      $.each(settings.themeUtils.mediaQueries, function(idx, item) {
        var query = '<div class="query query-' + item.itemNum + '">' + item.query + '</div>';
        $('#theme-utils-media-query').append(query);
      });

      // Hide the media query indicator by clicking on it.
      $('#theme-utils-media-query').click(function() {
        $(this).fadeOut(500);
      });
    }

  }
}

})(jQuery);
