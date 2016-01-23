;
(function ($) {
    Drupal.behaviors.secureshare = {
        attach:function (context, settings) {
            "use strict";

            if ('secureshare' in settings) {
                $.each(settings.secureshare, function (id, configuration) {
                    $('#' + id).once('secureshare', function () {
                        $(this).socialSharePrivacy(configuration);
                    });
                });
            }
        }
    };
})(jQuery);