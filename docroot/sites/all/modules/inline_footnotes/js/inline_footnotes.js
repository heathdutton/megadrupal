/**
 * Add active footnotes
 */

(function ($) {
    "use strict";

    Drupal.behaviors.footnotes_ckeditor = {
        attach: function (context, settings) {
            // javascript is enabled. (just to be sure)
            $('html').addClass('js');
            
            // find any footnotes
            $('fn').each(function (index, element) {
                // insert an active span and an invisible-by-default div
                var self = $(element);
                var insert = '';
                var header, label, close, content;

                // meta data
                label = '<span class="fn-label">' + self.attr('value') + '</span>';
                close = '<span class="fn-close">' + Drupal.t('Hide') + '</span>';
                header = '<span class="fn-header">' + label + close + '</span>';
                content = '<span class="fn-notes">' + self.html() + '</span>';

                // prepare presentation of the footnote
                insert = insert + '<span class="fn-content">' + header + content + '</span>';

                // insert after fn element
                self.after(insert);
            });

            // user interaction
            $('.fn-header, .fn-close').click(function (event) {
                var footnote = $(this).parents('.fn-content');

                // before show: hide other elements
                if (!footnote.hasClass('collapsed')) {
                    $('.fn-content').removeClass('collapsed');
                }

                // toggle element
                footnote.toggleClass('collapsed');

                // stop processing
                event && event.preventDefault && event.preventDefault();
                return false;
            });
        }
    };
})(jQuery)