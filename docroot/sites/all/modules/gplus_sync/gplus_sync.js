
function gplus_sync_sign_on_callback(authResult) {
  try {
    if (authResult['access_token']) {
      if (authResult.code) {
        console.log('Google+ Sync Authentication code generated status: ok');
        console.log('Google+ Sync Authentication code: ' + authResult.code.substr(0, 10) + '...');
        jQuery.post(Drupal.settings.basePath + 'gplus_sync_sign_on', {code: authResult.code}, function(result){
          var result = jQuery.parseJSON(result);
          if (result.status == 'login') {
            console.log('Google+ Sync Login to Drupal: yes');
            console.log('Google+ Sync Login with UID: ' + result.uid);
          }
          if (result.status == 'error') {
            console.log('Google+ Sync Login to Drupal: no');
            console.log('Google+ Sync Login error code: ' + result.error_code);
            console.log('Google+ Sync Login error text: ' + result.error_text);
          }
          if (result.redirect && result.error_code != 1) {
            window.location = result.redirect;
          } else {
            console.log('Google+ Sync Login redirect status: canceled');
          }
        });
      }
    } else if (authResult['error']) {
      console.log('Google+ Sync Authentication error: ' + authResult['error']);
    }
  } catch(err) {
  }
}