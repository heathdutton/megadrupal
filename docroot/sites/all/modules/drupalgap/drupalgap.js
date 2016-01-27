/**
 * @file
 * This file contains the JavaScript implementation for the DrupalGap module.
 */
(function ($) {
  Drupal.behaviors.drupalgap = {
    attach: function (context, settings) {
      try {
      }
      catch (error) {
        alert(Drupal.t(error));
      }
    }
  };
}(jQuery));

function drupalgap_system_connect_test() {
  try {
    var drupalgap_documentation_link = '<a href="http://drupal.org/node/2015065" target="_blank">View DrupalGap Troubleshooting Topics</a>';
    jQuery('#drupalgap-system-connect-status-message').html("<img src='" + Drupal.settings.basePath  + "misc/throbber.gif' />");
    // Obtain session token.
    var token_url = "?q=services/session/token"
    jQuery.ajax({
      url:token_url,
      type:"get",
      dataType:"text",
      error:function (jqXHR, textStatus, errorThrown) {
        if (!errorThrown) {
          errorThrown = Drupal.t('Token retrieval failed!');
        }
        var html = '<div class="messages error">' + errorThrown + '</div>';
            html += '<div class="messages warning">' + drupalgap_documentation_link + '</div>';
            jQuery('#drupalgap-system-connect-status-message').html(html);
      },
      success: function (token) {
        // Call system connect with session token.
        jQuery.ajax({
          url: Drupal.settings.drupalgap.services_endpoint_default + 'system/connect.json',
          type: "post",
          dataType: "json",
          beforeSend: function (request) {
            request.setRequestHeader("X-CSRF-Token", token);
          },
          error: function (jqXHR, textStatus, errorThrown) {
            console.log('woah');
            console.log(arguments);
            var msg = '';
            if (errorThrown) {
              msg += '<p>' + errorThrown + '</p>';
            }
            if (jqXHR.responseURL) {
              msg += '<p>' + jqXHR.responseURL + '</p>'
            }
            if (jqXHR.status) {
              msg += '<p>' + jqXHR.status + '</p>'
            }
            if (jqXHR.statusText) {
              msg += '<p>' + jqXHR.statusText + '</p>'
            }
            msg += '<p><a href="https://www.drupal.org/node/2015065" target="_blank">' +
              Drupal.t('Troubleshoot DrupalGap') +
            '</a></p>';
            var html = '<div class="messages error">' + msg + '</div>';
            jQuery('#drupalgap-system-connect-status-message').html(html);
          },
          success: function (data) {
            msg = Drupal.t("The system connect test was successful, <strong>DrupalGap is configured properly!</strong>");
            jQuery('#drupalgap-system-connect-status-message').html("<div class='messages status'>" + msg + "</div>");
          }
        });
      }
    });
  }
  catch (error) { console.log('drupalgap_system_connect_test - ' + error); }
}

