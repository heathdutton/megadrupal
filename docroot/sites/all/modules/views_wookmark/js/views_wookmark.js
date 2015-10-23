(function($) {
    Drupal.behaviors.viewsWookmark = {
        attach: function(context, settings) {
            settings.views_wookmark = $.extend({
                align: 'center',
                direction: 'left',
                autoResize: true,
                container: $("#" + settings.views_wookmark.container_id),
                ignoreInactiveItems: true,
                itemWidth: 0,
                fillEmptySpace: false,
                flexibleWidth: 0,
                offset: 2,
                onLayoutChanged: undefined,
                resizeDelay: 50
            }, settings.views_wookmark || {});

            var handler = $('.views-wookmark-grid-list li.views-wookmark-grid-item');
            handler.wookmark(settings.views_wookmark);
        }
    };

})(jQuery)


