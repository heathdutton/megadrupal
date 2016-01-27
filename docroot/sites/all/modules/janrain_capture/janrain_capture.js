(function ($) {

Drupal.janrainCapture = {
  closeProfileEditor: function() {
    window.location.href = Drupal.settings.janrainCapture.profile_sync_url;
  },
  closeRecoverPassword: function() {
    window.location.reload();
  },
  token_expired: function() {
    window.location.href = Drupal.settings.janrainCapture.token_expired_url;
  },
  bp_ready: function() {
      var ssojs = null;
      var ssotrue = false;
      var channelId = Backplane.getChannelID();
    jQuery('script').each(function() {
    if(jQuery(this).attr('src')) {
      ssojs = jQuery(this).attr('src');
        if ( undefined != ssojs && ssojs.search(/sso.js/i) > 0 ) {
          ssotrue = true;
          return false;
        }
    }
      });
    if (ssotrue) {
      // do sso -
    //console.log('Federated');
    JANRAIN.SSO.CAPTURE.check_login({
        sso_server: "https://" + Drupal.settings.janrainCapture.sso_address,
        client_id: janrainCaptureClientId,
        redirect_uri: janrainCaptureRedirectUri,
        logout_uri: janrainCaptureLogoutUri,
        xd_receiver: janrainCaptureXdReceiver,
        bp_channel: channelId
      });
      }
    jQuery("a.janrain_capture_signin").each(function(){
      jQuery(this).attr("href", $(this).attr("href") + "&bp_channel=" + channelId).click(function(){
      Backplane.expectMessages("identity/login");
    });
      });
  },
  logout: function() {
    if (typeof(Drupal.settings.janrainCapture.sso_address) != 'undefined') {
      JANRAIN.SSO.CAPTURE.logout({
        sso_server: "https://" + Drupal.settings.janrainCapture.sso_address,
        logout_uri: Drupal.settings.janrainCapture.logout_url
      });
    }
    if (typeof(Backplane) != 'undefined') {
        Backplane.resetCookieChannel();
    }
  },
  addDestination: function(destination) {
    if ($.bbq) {
      $.bbq.pushState({"destination": destination});
    }
  },
  removeDestination: function() {
    if ($.bbq) {
      $.bbq.removeState('destination');
    }
  }
};

Drupal.janrainCapture.prototype = {
  passwordRecover: function(url) {
    // Placeholder to be overwritten by the CaptureUI Module
  },
  resize: function(jargs) {
    // Placeholder to be overwritten by the CaptureUI Module
  }
}

Drupal.behaviors.janrainCapture = {
  attach: function(context, settings) {
    if (settings.janrainCapture.enforce) {
      // Modify all /user/login and /user/register links to use Capture.
      var ver = Drupal.settings.janrainCapture.ver;
      var bp = Drupal.settings.basePath;
      var links = $('a[href^="'+ bp +'user/login"], a[href^="'+ bp +'user/register"]').once('janrain-capture');
      var regex = /(?:\?|&)destination\=([^\&]*)/;
      if (ver == '1.0') {
        //legacy
        var length = links.length;
        if (links.length !== 0) {
          var i, link;
          for (i = 0; i < length; i++) {
            link = links[i];
            $(link).addClass('janrain_capture_anchor janrain_capture_signin');
            // If our login/register link has a destination param, we need to store
            // this in its data property so that we can use the jQuery BBQ plugin
            // to add it as a hash on the url.
            var match = regex.exec($(link).attr('href'));
            if (match && match.length == 2) {
              var destination = match[1];
              $(link).data("destination", destination);
            }
          }
        }
        if (typeof(settings.janrainCapture.backplane_server) != 'undefined'
            && typeof(settings.janrainCapture.backplane_bus_name) != 'undefined') {
          Backplane(Drupal.janrainCapture.bp_ready);
          Backplane.init({
            serverBaseURL: settings.janrainCapture.backplane_server,
            busName: settings.janrainCapture.backplane_bus_name
          });
        }
      }
    }
    window.CAPTURE = Drupal.janrainCapture;
  }
};

// Override the resize method on the Drupal.janrainCapture object so
// as to use a fancybox for it.
Drupal.janrainCapture.resize = function(jargs) {
  var args = $.parseJSON(jargs);
  $("#fancybox-inner, #fancybox-wrap, #fancybox-content, #fancybox-frame")
    .css({
      width: args.w,
      height: args.h
    });
  $.fancybox.resize();
  $.fancybox.center();
}

// Override the passwordRecover method on the Drupal.janrainCapture object so
// as to use a fancybox for it.
Drupal.janrainCapture.passwordRecover = function(url) {
  $.fancybox({
    type: "iframe",
    href: url,
    padding: 0,
    scrolling: "no",
    autoScale: true,
    width: 666,
    autoDimensions: false
  });
}

Drupal.behaviors.janrainCaptureUi = {
  attach: function(context, settings) {
    // Make all Capture signin and profile links appear in a fancybox.
    if ($.fn.fancybox) {
      $(".janrain_capture_anchor", context).once("capture-ui", function(){
        var data = $(this).data();
        $(this).addClass('iframe').fancybox({
          padding: 0,
          scrolling: "no",
          autoScale: true,
          width: 666,
          autoDimensions: false,
          onStart: function() {
            if (data.destination) {
              Drupal.janrainCapture.addDestination(data.destination);
            }
          },
          onClosed: function() {
            if (data.destination) {
              Drupal.janrainCapture.removeDestination(data.destination);
            }
          }
        });
      });
    }
  }
};

})(jQuery);
