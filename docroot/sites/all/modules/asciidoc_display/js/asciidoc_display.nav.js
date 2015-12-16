/**
 * @file
 * JavaScript for the AsciiDoc table of contents.
 */

(function ($) {
    $(document).ready(function() {
        // Mark each LI that has children to be expand/contractable, and
        // add a toggle to it.
        $('.asciidoc-display-toc .toc li').each(function() {
            if ($(this).children('ul').length > 0) {
                $(this).addClass('has-children');
                if ($(this).hasClass('active-trail')) {
                    toadd = 'minus';
                }
                else {
                    toadd = 'plus';
                }
                $(this).prepend('<p class="toggle ' + toadd + '-toggle">Expand/Contract</p>');
            }
            else {
                $(this).addClass('no-children');
            }
        });

        $('.toggle').click(function() {
            $(this).toggleClass('plus-toggle');
            $(this).toggleClass('minus-toggle');
            $(this).parent().children('ul').toggle();
        });
    });
})(jQuery);
