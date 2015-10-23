(function ($) {

    $(document).ready(function() {
        smartQueueIntProcess();
    });

    function smartQueueIntProcess() {
        // Do AJAX call.
        var url = Drupal.settings.basePath + 'smartqueue-int/ajax/process';

        $.ajax({
            url: url,
            success: function(data) {
                // If more to process, do another AJAX call.
                if (!data.complete) {
                    smartQueueIntProcess();
                }
            }
        });

    }

})(jQuery);
