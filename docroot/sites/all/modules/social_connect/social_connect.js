/**
 * @file social_connect.js
 */
(function ($) {
  HyvesConnect = function (Key, image, path) {
    // Define where the login button should be displayed if the autoAttempt fails or is disabled.
    var divNode = document.getElementById('HyvesLoginDiv');
    // Check IE version (we only support IE 8 and greater).
    var Browser = {
      Version: function() {
        var version = 999; // we assume a sane browser.
        if (navigator.appVersion.indexOf("MSIE") != -1)
          // Bah, IE again, lets downgrade version number.
          version = parseFloat(navigator.appVersion.split("MSIE")[1]);
        return version;
      }
    }

    if (Browser.Version() < 8) {
      divNode.innerHTML = 'HyvesConnect need IE 8 or greater version.';
      return;
    }

    // Define the init.
    Hyves.connect.init({
        // This is the consumer key bound to this application. The consumer key is used for API integration.
        consumerKey: Key,
        // This file is used for cross domain communication within older browsers.
        rpcRelay: location.protocol + "//" + location.host + "/hyves_hrpc_relay"
    });

    // Define the login option for the connect.
    var loginOptions = {
        // Basic profile information: username, profile picture, profile page.
        profileInformation: true,
        // Contact information: Home adress, email adress, phone number.
        contactInformation: true,
        // Define if you need API access or just want to use the login functionality.
        apiAccess: true,
        // Define which API methods you need. This is ignored if apiAccess is set to false.
        apiScope: "methods=users.get"
    };

    // Define the options for the button which will be displayed when autoAttempt fails or is disabled.
    var buttonOptions = {
        // Set to true if you want to automatically attempt to login. Set to false to show the connect button instead.
        autoAttempt: false
    };

    // Define your own or leave empty if you want to use the default image.
    if (image != '') {
      buttonOptions['image'] = image;
    }

    // Execute the HyvesConnect Login script.
    Hyves.connect.login.button(loginOptions, buttonOptions, function(response){
        document.getElementById('social_connect').innerHTML = '<img src="/'+path+'/images/wait.gif">';
        // Retrieve user info.
        var user_info = {
              source: 'hyves',
              username: getUsername(response.getField(Hyves.constants.response.profileField.profileurl)),
              userid: response.getField(Hyves.constants.response.field.userid),
              language: response.getField(Hyves.constants.response.field.language),
              nickname: response.getField(Hyves.constants.response.profileField.nickname),
              profilepicture: response.getField(Hyves.constants.response.profileField.profilepicture),
              profilepictureIcon: response.getField(Hyves.constants.response.profileField.profilepictureIcon),
              profilepictureIconLarge: response.getField(Hyves.constants.response.profileField.profilepictureIconLarge),
              profileurl: response.getField(Hyves.constants.response.profileField.profileurl),
              firstname: response.getField(Hyves.constants.response.profileField.firstname),
              lastname: response.getField(Hyves.constants.response.profileField.lastname),
              birthdate: response.getField(Hyves.constants.response.profileField.birthdate),
              gender: response.getField(Hyves.constants.response.profileField.gender),
              address: response.getField(Hyves.constants.response.contactField.address),
              postalcode: response.getField(Hyves.constants.response.contactField.postalcode),
              city: response.getField(Hyves.constants.response.contactField.city),
              email: response.getField(Hyves.constants.response.contactField.email),
              phonenumber: response.getField(Hyves.constants.response.contactField.phonenumber),
              accessToken: response.getField(Hyves.constants.response.oauth.accessToken)
        }

       $.ajax({
          type: 'POST',
          global: true,
          url: '/social_connect',
          data: { user_info: user_info},
          dataType: 'json',
          cache: false,
          success: function(data){
            if ((data.destination == undefined) ||(data.destination == 'reload')) {
              window.location.reload();

            }
            else {
              window.location.href = data.destination;
            }
          }
        });
    }, divNode);
  }

  FacebookConnect = function (Key, action, path) {

    // Create needed facebook root div.
    var fbDiv = document.createElement("div");
    fbDiv.id = "fb-root";
    document.body.appendChild(fbDiv);

    FB.init({
      appId: Key,
      status: true,
      cookie: true,
      xfbml: true
    });
    FB.getLoginStatus(function(response) {

      if (response.session) {
        // Destroy old user session if no action presented and session exist.
        FB.logout(function(response) {});

        if(action == 'login') {

          // User is logged. Get basic info.
          document.getElementById('social_connect').innerHTML = '<img src="/'+path+'/images/wait.gif">';
          var user_info = {source: 'facebook'};
          FB.api('/me', function(user) {
            if(user != null) {

              // Create keymap.
              var keymap = {
                  'username' : 'id',
                  'email' : 'email',
                  'languages' : 'languages',
                  'first_name' : 'first_name',
                  'last_name' : 'last_name',
                  'name' : 'name',
                  'link' : 'link',
                  'locale' : 'locale',
                  'birthday' : 'birthday',
                  'bio' : 'bio',
                  'hometown' : 'hometown.name',
                  'location' : 'location.name',
                  'work' : 'work',
                  'political' : 'political',
                  'favorite_athletes' : 'favorite_athletes',
                  'favorite_teams' : 'favorite_teams',
                  'quotes' : 'quotes',
                  'religion' : 'religion',
                  'sports' : 'sports',
                  'website' : 'website',
                  'timezone' : 'timezone'
                }

                for (var key in keymap) {
                    var val = keymap[key];
                    if (user[val]) {
                      user_info[key] = user[val];
                    }
                }

                if (user.gender) {
                  if (user.gender == 'male') {
                    var gender = 'M';
                  }
                  else if (user.gender == 'female') {
                    var gender = 'F';
                  }
                  user_info['gender'] = gender;
                }

                if (user.id) {
                  user_info['profilepicture'] = 'http://graph.facebook.com/' + user.id + '/picture?type=normal';
                  user_info['profilepicturesmall'] = 'http://graph.facebook.com/' + user.id + '/picture?type=small';
                  user_info['profilepicturenormal'] = 'http://graph.facebook.com/' + user.id + '/picture?type=normal';
                  user_info['profilepicturelarge'] = 'http://graph.facebook.com/' + user.id + '/picture?type=large';
                }
                $.ajax({
                  type: 'POST',
                  global: true,
                  url: '/social_connect',
                  data: { user_info: user_info},
                  dataType: 'json',
                  cache: false,
                  success: function(data){
                    if ((data.destination == undefined) ||(data.destination == 'reload')) {
                      window.location.reload();

                    }
                    else {
                      window.location.href = data.destination;
                    }
                  }
                });
              }
          });
        }
      }
    });
  }

  // Helper function for getting subdomain. In case for Hyves subdomain=username.
  getUsername = function (url) {
    // If there, remove white space from both ends.
    url = url.replace(new RegExp(/^\s+/),""); // Start.
    url = url.replace(new RegExp(/\s+$/),""); // End.

    // If found, convert back slaches to forward slashes.
    url = url.replace(new RegExp(/\\/g),"/");

    // If there, removes 'http://', 'https://' or 'ftp://' from the start of the string.
    url = url.replace(new RegExp(/^http\:\/\/|^https\:\/\/|^ftp\:\/\//i),"");

    // If there, removes 'www.' from the start of the string.
    url = url.replace(new RegExp(/^www\./i),"");

    // Remove complete string from first forward slash on.
    url = url.replace(new RegExp(/\/(.*)/),"");

    // Removes '.??.??' or '.???.??' from end - e.g. '.CO.UK', '.COM.AU'.
    if (url.match(new RegExp(/\.[a-z]{2,3}\.[a-z]{2}$/i))) {
        url = url.replace(new RegExp(/\.[a-z]{2,3}\.[a-z]{2}$/i),"");

    // Removes '.??' or '.???' or '.????' from end - e.g. '.US', '.COM', '.INFO'.
    } else if (url.match(new RegExp(/\.hyves.nl$/i))) {
        url = url.replace(new RegExp(/\.hyves.nl$/i),"");
    }

    return(url);
  }
})(jQuery);