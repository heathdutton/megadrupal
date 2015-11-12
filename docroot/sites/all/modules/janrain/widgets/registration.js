//<?php
/**
 * @file
 */
//?>

// Initialize plex object jic (no clobber).
'object' != typeof window.janrain && (window.janrain = {});
'object' != typeof janrain.plex && (janrain.plex = {});

// @todo move to drupal behaviors?
janrain.plex.login = function(oauthCode) {
  var base_path = Drupal.settings.basePath;
  if (!Drupal.settings.janrain.clean_url) {
    base_path = Drupal.settings.basePath + '?q=';
  }
  if (typeof oauthCode !== "string") console && console.error('Bork!');
  jQuery.get(base_path + 'services/session/token',
    function (drupalToken) {
      if (typeof drupalToken !== "string") console && console.log(drupalToken);
      jQuery.ajax({
          url: base_path + 'janrain/registration/code.json',
          type:'post',
          cache: false,
          xhrFields:{withCredentials:true},
          beforeSend: function (req) {req.setRequestHeader('X-CSRF-Token', drupalToken);},
          error: function (jqxhr, status, error) {console.log(error);},
          data:{code:oauthCode},
          success: function (resp) {
            console && console.log(resp);
            document.getElementById('user_login').submit();
          }
      });
    });
}

// do not invoke this outside of janrainCaptureWidgetOnLoad or in beforeJanrain...
janrain.plex.refreshToken = function() {
  if (!Drupal.settings.janrain.user_is_logged_in) {
    return;
  }
  var base_path = Drupal.settings.basePath;
  if (!Drupal.settings.janrain.clean_url) {
    base_path = Drupal.settings.basePath + '?q=';
  }
  jQuery.ajax({
    url: base_path + 'services/session/token',
    async:false,
    error: function (jqxhr, status, error) {console.error(error);},
    success: function (drupalToken) {
      console && console.log(drupalToken);
      jQuery.ajax({
        url: base_path + 'janrain/registration/session_token.json',
        type:'post',
        xhrFields:{withCredentials:true},
        beforeSend: function (req) {req.setRequestHeader('X-CSRF-Token', drupalToken);},
        error: function (jqxhr, status, error) {console.error(error);},
        //must synchronize this or widget loads before session started.
        async:false,
        success: function (resp) {
          console && console.log(resp);
          // feed the token to where it needs to go.
          janrain.capture.ui.createCaptureSession(resp[0]);
        }
      });
    }
  });
}

// profile update sync function
janrain.plex.profileUpdate = function (updateEvent) {
  // Skip for update failures and anonymous users.
  if ('success' !== updateEvent.status || !Drupal.settings.janrain.user_is_logged_in) {
    return;
  }
  var base_path = Drupal.settings.basePath;
  if (!Drupal.settings.janrain.clean_url) {
    base_path = base_path.concat('?q=');
  }
  jQuery.ajax({
    url: base_path + 'services/session/token',
    error: function (jqxhr, status, error) {console.error(error);},
    success: function (drupalToken) {
      jQuery.ajax({
        url: base_path + 'janrain/registration/profile_update.json',
        type:'post',
        xhrFields:{withCredentials:true},
        beforeSend: function (req) {req.setRequestHeader('X-CSRF-Token', drupalToken);},
        error: function (jqxhr, status, error) {console.error(error);},
        success: function (resp) {
          console && console.log(resp);
        }
      });
    }
  });
}
