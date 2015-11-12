/**
 * @file
 * Javascript for the interface at netx tab.
 */

(function ($) {

    /**
     * Functionality for the administrative file listings.
     */
    Drupal.behaviors.netxAdmin = {
        attach: function (context) {
            $('.media-item', '#netx-preview').once('netx', function() {
                $(this).click(function() {
                    $('.media-item', '#netx-preview').removeClass('selected');
                    $(this).addClass('selected');
                });
            });
        }
    };

})(jQuery);
