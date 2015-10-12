(function ($) {
  Drupal.behaviors.hotblocks_itemcontrolsbehaviors = {
    attach: function (context, settings) {
      // Hover state classes for the block at the outer-most block level
      $('.block-hotblocks', context).hover(
          // Mouse enter
          function (e) { $('.hotblock', this).addClass('hover'); },
          // Mouse leave
          function (e) { $('.hotblock', this).removeClass('hover'); }
      );

      // Hover state classes for the block controls
      $('.hotblocks-controls', context).hover(
        // Mouse enter
        function (e) { $(this).closest('.hotblock').addClass('block-controls-hover'); },
        // Mouse leave
        function (e) { $(this).closest('.hotblock').removeClass('block-controls-hover'); }
      );

      // Hover state classes for individual hotblock items
      $('.hotblocks_item', context).hover(
          // Mouse enter
          function (e) { $(this).addClass('hover'); },
          // Mouse leave
          function (e) { $(this).removeClass('hover'); }
      );

      // Make hotblock controls 'sticky' on the page when scrolling down
      // todo - this should be a UI option on the settings page
      $('.hotblock-wrapper', context).each(function(index) {
        // Set fixed bool to false initially
        var fixedOverlay = false;

        var $wrapper = $(this);
        var $overlay = $wrapper.find(".hotblocks-controls");

        // Set up top threshold
        var topThreshold = $wrapper.offset().top;

        // Set up bottom threshold
        var bottomThreshold = topThreshold + $wrapper.height();


        // Fixed position for long items.
        $(window).scroll(function() {
          bottomThreshold = topThreshold + $wrapper.height();

          var belowTopThreshold = $(window).scrollTop() >= topThreshold;
          var belowBottomThreshold = $(window).scrollTop() >= bottomThreshold - 400;

          if (!fixedOverlay && belowTopThreshold && !belowBottomThreshold) {
            // Make fixed
            $overlay.addClass("fixed");
            fixedOverlay = true;
          }
          else if ((fixedOverlay && !belowTopThreshold) || (fixedOverlay && belowBottomThreshold)) {
            // Remove fixed
            $overlay.removeClass("fixed");
            fixedOverlay = false;
          }
        });
      });
    }
  }
})(jQuery);