(function ($) {
  Drupal.behaviors.social_network_user_detection = {
    attach: function (context, settings) {
      social_network_user_detection_add_core_js();
    }
  };
  // Hyves.
  HyvesSocialNetworkDetection = function (do_not_add_the_hyves_init, path, hyves_id) {
    social_network_user_detection_add_hyves_js(do_not_add_the_hyves_init, path, hyves_id);
  }
  // Facebook.
  FacebookSocialNetworkDetection = function (do_not_add_the_facebook_init, facebook_id) {
    social_network_user_detection_add_core_fb_js(do_not_add_the_facebook_init, facebook_id);
  }
})(jQuery);

function social_network_user_detection_add_core_fb_js(do_not_add_the_facebook_init, facebook_id) {
  if (do_not_add_the_facebook_init == 0) {
    window.fbAsyncInit = function() {
     FB.init({
       appId      : facebook_id,
       status     : true,
       cookie     : true,
       xfbml      : true
       });
       // Additional initialization code here
       FB.getLoginStatus(function(response){
         if (response.status != "unknown")
         {
           social_network_user_detection_record_login_status(4, "Facebook", true);
         }else{
           social_network_user_detection_record_login_status(4, "Facebook", false);
         }
       });
     }
 }
 else {
    social_network_user_detection_poll_fb_available();
 }
 if (do_not_add_the_facebook_init == 0) {
   // Load the SDK Asynchronously
  (function(d){
     var js, id = 'facebook-jssdk', ref = d.getElementsByTagName('script')[0];
     if (d.getElementById(id)) {return;}
     js = d.createElement('script'); js.id = id; js.async = true;
     js.src = "//connect.facebook.net/en_US/all.js";
     ref.parentNode.insertBefore(js, ref);
   }(document));
 }
}

function social_network_user_detection_add_hyves_js(do_not_add_the_hyves_init, path, hyves_id) {
  if (do_not_add_the_hyves_init == 0) {
    // Define the init
    Hyves.connect.init({
      // This is the consumer key bound to this application. The consumer key is
      // used for API integration
      consumerKey: hyves_id,
      // This file is used for cross domain communication within older browsers
      rpcRelay: path + "/hcrpc_relay.html"
    });
  }
  // Execute the HyvesConnect Login script
  var loginOptions;
  Hyves.connect.login.attempt(loginOptions, function(response){
    if (response.signature) {
      social_network_user_detection_record_login_status(5, "Hyves", true);
    }
  });
}

function social_network_user_detection_add_core_js() {
  // Google Account, Gmail, etc.
  jQuery('.social-network-user-detection-core-wrapper').html('<img style="display:none;" '
 + 'onload="social_network_user_detection_record_login_status(1, \'Google\', true)" '
 + 'onerror="social_network_user_detection_record_login_status(1, \'Google\', false)" '
 + 'src="https://accounts.google.com/CheckCookie?continue=https://www.google.com/intl/en/images/logos/accounts_logo.png" />');
  // Google+.
  jQuery('.social-network-user-detection-core-wrapper').append('<img style="display:none;" '
 + 'onload="social_network_user_detection_record_login_status(2, \'GooglePlus\', true)" '
 + 'onerror="social_network_user_detection_record_login_status(2, \'GooglePlus\', false)" '
 + 'src="https://plus.google.com/up/?continue=https://www.google.com/intl/en/images/logos/accounts_logo.png&type=st&gpsrc=ogpy0" />');
  // Twitter.
  jQuery('.social-network-user-detection-core-wrapper').append('<img style="display:none;" '
 + 'src="https://twitter.com/login?redirect_after_login=%2Fimages%2Fspinner.gif" '
 + 'onload="social_network_user_detection_record_login_status(3, \'Twitter\', true)" '
 + 'onerror="social_network_user_detection_record_login_status(3, \'Twitter\', false)" />');
}

function social_network_user_detection_async_version(slot, network, status) {
  if (status) {
    _gaq.push(["_setCustomVar", slot, network + "_State", "LoggedIn", 1]);
  }
  else {
    _gaq.push(["_setCustomVar", slot, network + "_State", "NotLoggedIn", 1]);
  }
}

function social_network_user_detection_old_version() {
  if (status) {
    pageTracker._setCustomVar(slot, network + "_State", "LoggedIn", 1);
  }
  else {
    pageTracker._setCustomVar(slot, network + "_State", "NotLoggedIn", 1);
  }
}

function social_network_user_detection_record_login_status(slot, network, status) {
  jQuery.post(Drupal.settings.basePath + "social_network_user_detection/ajax-set-logged-in", { network: network, status: status });

  if (Drupal.settings.social_network_user_detection.do_no_track_antything == 1) {
    if (Drupal.settings.social_network_user_detection.js_reporting_version == 'async') {
      social_network_user_detection_async_version(slot, network, status);
    }
    else {
      social_network_user_detection_old_version(slot, network, status);
    }
  }
}

function social_network_user_detection_poll_fb_available() {
  // This value is in ms.
  var interval = 500;
  window.setTimeout(function() {
    if (FB !== 'undefined' || typeof FB !== 'undefined') {
      FB.getLoginStatus(function(response){
      if (response.status != "unknown")
        {
          social_network_user_detection_record_login_status(4, "Facebook", true);
        }else{
          social_network_user_detection_record_login_status(4, "Facebook", false);
        }
      });
    }
  }, interval);
}
