/**
 * @file
 * Visibility code for Social Media Bar.
 */

(function($) {
    // When we scroll, see if the bar is visible. If not, flip it to the vertical look.
    $(document).scroll(function() {
        if(!$('.socialmediabar-container').visible()) {
            $('.socialmediabar').addClass('socialmediabar-vertical');
        } else {
            $('.socialmediabar').removeClass('socialmediabar-vertical');
        }
    });
}(jQuery));
