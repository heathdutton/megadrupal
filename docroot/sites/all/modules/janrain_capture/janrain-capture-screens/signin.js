function janrainCaptureWidgetOnLoad() {
  var $logout_link = jQuery('a[href="/user/logout"]');
  $logout_link.addClass('capture_end_session');

  function handleCaptureLogin(result) {
    console.log ("exchanging code for token...");

    jQuery.ajax({
      url: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'janrain_capture/oauth?code=' + result.authorizationCode,
      success: function(token) {
        console.log('code for token exchange completed');
        window.location.reload();
      },
      async: false
    });
  }
  janrain.events.onCaptureSessionFound.addHandler(function(result){
    //console.log ("capture session found");
  });

  janrain.events.onCaptureSessionNotFound.addHandler(function(result){
    //console.log ("capture session not found");
    if (typeof(Backplane) != 'undefined' && typeof(Backplane.getChannelID()) == 'undefined') {
      //console.log ("reset backplane channel");
      Backplane.resetCookieChannel();
    }
  });

  janrain.events.onCaptureLoginSuccess.addHandler(handleCaptureLogin);
  janrain.events.onCaptureSessionEnded.addHandler(function() {
    window.location.href = '/user/logout';
  });
  janrain.events.onCaptureRegistrationSuccess.addHandler(handleCaptureLogin);

  janrain.capture.ui.start();
}
