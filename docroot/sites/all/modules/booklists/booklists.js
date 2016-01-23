(function ($) { // JavaScript should be compatible with other libraries than jQuery
  Drupal.behaviors.booklists = { // D7 "Changed Drupal.behaviors to objects having the methods "attach" and "detach"."
    attach: function(context) {

      // Let's set the number of items to show based on the screen width.
      var width = $(window).width();
      var carouselNumItems = 4; // Default to 5.
      if (width < 960) {
        carouselNumItems = 3;
      }
      $('body').imagesLoaded(function() {
        booklists_fix_syndetics();
        // Kick off the cycle.
        initializeCycle(carouselNumItems);
        // Turn the image hovers on.
        initializeHovers();
        // Allow the switcher to work & load other blocks via AJAX.
        initializeSwitcher(carouselNumItems);
        // Booklists - customSelect Switcher.
        if ($('.block-booklists').length > 0) {
          var currentBlockID = $('.block-booklists').attr('id');
          if (currentBlockID) {
            var simpleID = currentBlockID.replace('block-booklists-booklists-', ''); // simpleID will look like "amazon-block-2", etc.
            $('.block-booklists .switcher').val(simpleID); // Set the current value of the switcher to match the block that it's inside of.
            $('.block-booklists .switcher').customSelect({
              mapClass: false, // Copy any existing classes from the given select element (defaults to true)
              mapStyle: false // Copy the value of the style attribute from the given select element (defaults to true)
            });
          }
        }
      });

      function initializeCycle(carouselNumItems) {
        $('.booklists-responsive').cycle({
          autoHeight: 'calc', // Calculate the height based on the tallest image.
          loader: 'wait', // ...For all images to arrive before staring slideshow.
          swipe: true,
          carouselVisible: carouselNumItems
        });
      }
      function initializeHovers() {
        $('img.booklists_cover').hover(
          function() { // "Mouse in"
            // Change the caption text to whatever the alt text is for the image the user hovers over.
            var title = $(this).attr('alt');
            // Put in the attribution information in non-italics if it exists.
            var attribution = $(this).attr('data-booklists-attribution');
            if (attribution) {
              $('#booklists-alt-caption').html(title + '<div class="booklists-attribution">' + attribution + '</div>');
            }
            // Otherwise just put in the title in italics.
            else {
              $('#booklists-alt-caption').text(title);
            }
          },
          function() { // "Mouse out"
            $('#booklists-alt-caption').text('');
          }
        );
      } // End function initializeHovers()
      function initializeSwitcher(carouselNumItems) {
        $('.block-booklists .switcher').change(function() {
          var switcherID = $(this).val();
          // Change the ID attribute to match what was chosen...
          // Then add a "loading" image to the page...
          // Then load in the new block...
          $(this).parents('.block-booklists').attr('id', 'block-booklists-booklists-' + switcherID).html('<div style="text-align: center;"><img src="/sites/all/modules/contrib/booklists/images/ajax-loader.gif" alt="Loading" /></div>').load('/sites/all/modules/contrib/booklists/includes/booklists.block.php?b2e=' + switcherID, function() {
            // Kick off the cycle now that we've loaded a new carousel.
            initializeCycle(carouselNumItems);
            // Turn the image hovers on again.
            initializeHovers();
            // Allow the switcher to work again.
            initializeSwitcher(carouselNumItems);
            // Booklists - customSelect Switcher.
            var currentBlockID = $('.block-booklists').attr('id');
            var simpleID = currentBlockID.replace('block-booklists-booklists-', ''); // simpleID will look like "amazon-block-2", etc.
            $('.block-booklists .switcher').val(simpleID); // Set the current value of the switcher to match the block that it's inside of.
            $('.block-booklists .switcher').customSelect({
              mapClass: false, // Copy any existing classes from the given select element (defaults to true)
              mapStyle: false // Copy the value of the style attribute from the given select element (defaults to true)
            });
            $('.block-booklists span.customSelect').eq(1).remove(); // Since this is post-ajax loading, let's remove any unnecessary customSelect spans that exist in the DOM.
          });
          return false;
        });
      } // End function initializeSwitcher()

      // Fixes syndetics images that are only 1px by 1px to use the pac default cover image instead.
      function booklists_fix_syndetics() {
        // Check to see if syndetics actually has a cover image (so we can avoid displaying their default image if not).
        $('img.booklists-syndetics').each(function(){
          var alt = $(this).attr('alt');
          // Get on screen image
          var screenImage = $(this);

          // Create new offscreen image to test natural width (some are 1x1 px).
          var theImage = new Image();
          theImage.src = screenImage.attr('src');
          
          // Get accurate measurements from that.
          var imageWidth = theImage.width;
          theImage.onload = function() {
            if (imageWidth < 10) {
              screenImage.attr('src', '/sites/all/modules/contrib/booklists/images/default_cover.png');
            }
          }

        });
      }

    }
  };
})(jQuery);
