/**
 * @file
 * Attaches behaviors for the payment process on the CMS Updater settings page.
 */

(function ($) {
    Drupal.behaviors.cms_updater = {
        attach: function (context, settings) {
            window.addEventListener('message', receive_message);

            function receive_message(event) {
                if (event.origin !== Drupal.settings.cms_updater.service_url)
                    return;
                var result = event.data;
                if(result.indexOf('iFrameHeight-') == 0){
                    var height = result.substr(13);
                    height = parseInt(height) +10;
                    $('#buy-cms-updater-service').height(height+'px');

                }
                if(result.indexOf('cmsUpdaterKey-') == 0){
                    var key = result.substr(14);
                    // Insert the purchased key in the settings form field
                    $('#edit-cms-updater-key').val(key);
                    // Submit settings form
                    $('#edit-submit').click();
                }

            }
        }
    };


})(jQuery);
