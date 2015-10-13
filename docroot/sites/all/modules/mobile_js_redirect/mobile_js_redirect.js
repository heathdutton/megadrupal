var mobile_pattern = new RegExp(mobile_js_redirect_devices_regexp, 'i');
var mobile = (mobile_pattern.test(navigator.userAgent.toLowerCase()));

// Both of these variables have to be set for this mechanism to work.
// If this plugin is being used by a Drupal Gardens site, we can simply
// set these variables manually and upload this file to the site.
var mobile_url = mobile_js_redirect_mobile_url;
var desktop_url = mobile_js_redirect_desktop_url;

// check whether the noRedirect flag is set
var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
var no_redirect = false;
var secure = (window.location.protocol == "https:") ? true : false;
for (var i = 0; i < hashes.length; i++) {
  var hash = hashes[i].split('=');
  if (hash[0] != null && hash[0] == 'stopredirect' && hash[1] != null && hash[1] == 'true') {
    no_redirect = true;
    // Add session cookie to stop future redirect.
    jQuery.cookie('mobile_js_redirect.stopredirect', 1, { path: '/', secure: secure });
    break;
  } else if (jQuery.cookie('mobile_js_redirect.stopredirect') == 1) {
    no_redirect = true;
  }
}

if (mobile_url != null && desktop_url != null && !no_redirect) {
  // Remove protocol and path.
  var mobile_url_parts = mobile_url.replace('http://', '').replace('https://', '').split(/[/:?#]/);
  var desktop_url_parts = desktop_url.replace('http://', '').replace('https://', '').split(/[/:?#]/);

  if (mobile && window.location.hostname == desktop_url_parts[0]) {
    // The device is mobile but the URL is desktop, so redirect.
    document.location = mobile_url;
  } else if (!mobile && window.location.hostname == mobile_url_parts[0]) {
    // The device is desktop but the URL is mobile, so redirect.
    document.location = desktop_url;
  }
}
