/**
 * @file
 * Overide of core's collapse.js
 */

(function ($) {
Drupal.behaviors.collapse = {
    attach: function (context, settings) {

        $('fieldset.collapsible', context).once('collapse', function () {
            var $fieldset = $(this);
            // Expand fieldset if there are errors inside, or if it contains an
            // element that is targeted by the URI fragment identifier.
            var anchor = location.hash && location.hash != '#' ? ', ' + location.hash : '';
            if ($fieldset.find('.error' + anchor).length) {
                $fieldset.removeClass('collapsed');
            }

            var summary = $('<span class="summary"></span>');
            $fieldset.
                bind('summaryUpdated', function () {
                    var text = $.trim($fieldset.drupalGetSummary());
                    summary.html(text ? ' (' + text + ')' : '');
                })
                .trigger('summaryUpdated');

            // Turn the legend into a clickable link, but retain span.fieldset-legend
            // for CSS positioning.
            var $legend = $('> legend .fieldset-legend', this);

            $('<span class="fieldset-legend-prefix element-invisible"></span>')
                .append($fieldset.hasClass('collapsed') ? Drupal.t('Show') : Drupal.t('Hide'))
                .prependTo($legend)
                .after(' ');

            // .wrapInner() does not retain bound events.
            var $link = $legend.contents()
                .appendTo($legend);

            $legend.append(summary);
        });
    }
};



})(jQuery);


