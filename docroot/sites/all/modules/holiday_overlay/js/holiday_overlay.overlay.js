(function ($) {

Drupal.behaviors.holidayOverlay = {
  attach: function(context, settings) {

    // Add the overlay and message.
    $('body').prepend('<div id="holiday-overlay">' +
        '<div class="message-box">' +
          '<a class="holiday-overlay-close" href="#"></a>' +
          '<div class="message">' + Drupal.settings.holidayOverlayMessage + '</div>' +
        '</div>' +
      '</div>');

    // Get the overlay by ID.
    var $overlay = $("#holiday-overlay");

    // Adjust height to height of viewport.
    $overlay.css("height", $(window).height());
    $overlay.css("width", $(window).width());

    // Wait a half-second, then fade in the overlay.
    $overlay.delay(500).fadeIn(800);

    // Close the overlay when the close button is clicked.
    $(".holiday-overlay-close").click(function() {
      $overlay.fadeOut(100);
      return false;
    });

    // Adjust height over overlay as window is resized.
    $(window).bind("resize", function() {
       $overlay.css("height", $(window).height());
       $overlay.css("width", $(window).width());
    });
  }
};

})(jQuery);