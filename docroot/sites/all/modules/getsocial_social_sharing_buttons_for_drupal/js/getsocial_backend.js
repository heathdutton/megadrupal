/**
 * @file
 * Javascript global binds.
 *
 * Helper funtions to make ajax calls and manipulate DOM.
 */

(function () {
  'use strict';

  jQuery(document).ready(function ($) {

    // Insert key form when user email is already registered.
    jQuery('#save-key').bind('click', function (e) {
      e.preventDefault();
      var getSocialApiUrl = Drupal.settings.getsocial_share_buttons.getSocialApiUrl;
      var getSocialApiKey = jQuery('#edit-insert-key').val();
      jQuery('[name=api-key]').attr('value', getSocialApiKey);

      jQuery.ajax({url: getSocialApiUrl + 'sites/' + getSocialApiKey,
                success: function (result) {
                  jQuery('[name=api-result]').attr('value', JSON.stringify(result));
                  jQuery('.get-social-register-form').trigger('submit');
                }
            });
    });

    jQuery('#activate-account').bind('click', function (e) {
      e.preventDefault();
      jQuery('.form-button-group').hide();
      jQuery('.loading-create').show();
      jQuery('#result-info-bar').css('visibility', 'hidden');
      jQuery('.notification-bar').hide();
      jQuery('.create-gs-account').hide();
      jQuery('.loading-create').addClass('active');
      jQuery.post(jQuery(this).attr('href'), {source: 'drupal'}, function (data) {
        if (data.errors) {

          jQuery('.form-item-url-input').hide();
          jQuery('.form-item-email-input').hide();
          jQuery('.form-button-group').hide();

          jQuery('.form-item-insert-key').show();
          jQuery('.submit-save-key').show();

          jQuery('#result-info-bar p').html(data.errors[0]);
          jQuery('#result-info-bar').addClass('error');
          jQuery('#result-info-bar').css('visibility', 'visible');

        }
        else {

          // Save the api key to do the second request.
          var getSocialApiUrl = Drupal.settings.getsocial_share_buttons.getSocialApiUrl;
          var getSocialApiKey = data.api_key;

          jQuery('[name=api-key]').attr('value', getSocialApiKey);

          jQuery.ajax({url: getSocialApiUrl + 'sites/' + getSocialApiKey,
                      success: function (result) {
                        jQuery('[name=api-result]').attr('value', JSON.stringify(result));
                        jQuery('.loading-create').hide();
                        jQuery('.success-message').show();
                        jQuery('.get-social-register-form').trigger('submit');

                        setTimeout(
                            jQuery('.get-social-register-form').trigger('submit'),
                            3000
                        );
                      }
                  });
        }
      });
    });
  });
})();
