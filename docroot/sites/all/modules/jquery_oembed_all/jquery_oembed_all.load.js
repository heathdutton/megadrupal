(function($) {

    // Behavior to load oEmbed_all
    Drupal.behaviors.jquery_oembed_all = {
        attach: function(context) {
            if (context === document) {
                var selector = Drupal.settings.jqueryOembedAll.jqueryOembedAllSelector;
                $(selector, context)
                    .oembed( null, {
                        embedMethod: 'replace',
                        fallback: false
                    });
            }
        }
    };

}(jQuery));
