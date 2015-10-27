/**
 * @file
 * Javascript file for facebook callback function.
 */

function fb_callout() {

    // Init the FB JS SDK.
    FB.init({
        appId: fbapi,
        channelUrl: '',
        status: true,
        cookie: true,
        xfbml: true
    });

    var publish = {
        method: 'feed',
    };

    publish.link = location.href;
    publish.name = document.title;
    var description;
    var metas = document.getElementsByTagName('meta');
    for (var x = 0, y = metas.length; x < y; x++) {
        if (metas[x].name.toLowerCase() === "description") {
            description = metas[x];
        }
    }

    FB.ui(publish, function (response) {
        if (response && response.post_id) {
            share_callback('fb',token);
        }
    });
}

// Load the SDK Asynchronously.
(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) {
    return;
  }
  js = d.createElement(s); js.id = id;
  js.src = "http://connect.facebook.net/en_US/all.js";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));
