/**
 * @file
 * Pop theme javascript.
 */

(function ($) {

  /**
   * ImgSizer
   * http://unstoppablerobotninja.com/entry/fluid-images/
   */
  Drupal.behaviors.popImgSizer = {
    attach: function(context, settings) {
      $(window).load(function () {
        imgSizer.collate($('img'));
      });
    }
  };

  /**
   * Grid Overlay
   * Adding grid overlay for development.
   */
  Drupal.behaviors.popGridOverlay = {
    attach: function(context, settings) {
      if ($('body.grid-overlay-enabled').size() > 0) {
        var grid = '<div id="pop-grid"></div>';
        var grid_switch = '<div id="pop-grid-switch">Show Grid</div>';
        $('body.grid-overlay-enabled').append(grid_switch);
        $('#page').append(grid);

        var pageWidth = $('#page').width();
        var colWidth = 68;
        var colSep = 24;
        var colCount = 1;

        $('#pop-grid').css({width: pageWidth});

        for(colLeft = 0; colLeft <= pageWidth; colLeft = (colWidth + colSep) * (colCount - 1)) {
          var colW = ((colWidth + colSep) * colCount ) - colSep;
          var colSpan = '<span class="pop-col pop-col-'+colCount+'">col: ' + colCount + '<br/>w: ' + colW + 'px</span>';

          $('#pop-grid').append(colSpan);
          $('#pop-grid .pop-col-'+colCount).css({ width: colWidth, left: colLeft });
          colCount++;
        };

        $('#pop-grid-switch').toggle(
          function() {
            $(this).text("Hide Grid").attr('rel','on');
            $('#pop-grid').show();
          }, function() {
            $(this).text('Show Grid').attr('rel','off');
            $('#pop-grid').hide();
          }
        );
      }
    }
  };

  /**
   * Equalize the heights of elements.
   *
   * Usage: $(object).equalHeights([minHeight], [maxHeight]);
   *
   * Known problem: Resize or dynamic content will break the equal height.
   */
  $.fn.equalHeights = function(minHeight, maxHeight) {
    tallest = (minHeight) ? minHeight : 0;
    this.each(function() {
      if ($(this).outerHeight() > tallest) {
        tallest = $(this).outerHeight();
      }
    });

    if ((maxHeight) && tallest > maxHeight) tallest = maxHeight;

    return this.each(function() {
      var extras = $(this).outerHeight() - $(this).height();
      if ($.browser.msie && $.browser.version < 7) { $(this).height(tallest-extras); }
      $(this).css({'min-height':tallest-extras + 'px'/*, 'overflow':'auto'*/});
    });
  }

  Drupal.behaviors.popEqualHeights = {
    attach: function(context, settings) {
      if ($('body.pop-equal-heights').size() > 0) {
        $('.column').equalHeights();
      }
    }
  };
})(jQuery);
