/**
 * @file
 * Allows a user to save their Instagram user ID.
 */

// Configure JS Lint.
/*jslint browser: true, white: true */
/*global jQuery, Drupal */


// Accesses the server-side Instagram user ID API resource/endpoint.
Drupal.instagramProfile.userIdApi = function (method, data, callback) {
  "use strict";
  // Initialize the data variable.
  data = data || {};
  // Include the CSRF token for all API requests.
  data.csrf_token = Drupal.settings.instagramProfile.csrfToken;

  jQuery.ajax({
    type:    method,
    // Support Drupal installed in a sub-directory.
    url:     Drupal.settings.basePath + 'instagram_profile/user_id',
    data:    data,
    success: callback
  });
};

// Saves user's own Instagram user ID after authenticating on Instagram.com.
// Provide closure but execute this immediately so that DOM-intiation code can
// discover what state to initialise DOM into:
//   - No shared feed
//   - A shared feed
//   - Authenticating a new user for new feed, whether or not there is an
//     existing shared feed.
(function () {
  "use strict";
  var accessToken,
    // Functions.
    main, getAccessToken, saveUserId,
    // The key of the access token in the URL's hash.
    key = 'access_token=';

  // Gets the access token, then saves it to the server/database, if found.
  main = function () {
    accessToken = getAccessToken();
    if (accessToken) {
      // Show the throbbers (which were hid by
      // Drupal.instagramProfile.behaviors.view()), when the user is
      // authenticating.
      Drupal.instagramProfile.newInstagramUser = true;

      // Get the current user's Instagram user object.
      Drupal.instagramProfile.get('users/self', saveUserId, { access_token: accessToken });

      // Remove the access token from the URL so that they do not accidentally
      // share it by copying and pasting the URL.
      if (history.replaceState) {
        // If the browser supports History API, update history state to exclude
        // the access token.
        history.replaceState(null, document.title, location.href.replace('#' + key + accessToken, ''));
      }
      else {
        // Reset the hash.  (This leaves the hash character (`#`) in the URL.)
        window.location.hash = '';
      }
    }
  };

  // Looks for and retrieves the access token in the URL's hash.
  getAccessToken = function() {
    var part,
      // Split the hash by the ampersand character.
      parts = window.location.hash.substr(1).split('&');

    // Iterate over each section of the hash.
    while (parts.length) {
      part = parts.pop();
      // If this part starts with the access token's key: "access_token=";
      if (part.indexOf(key) === 0) {
        // Retrieve the value.
        return part.substr(key.length);
      }
    }
  };

  // Saves the Instagram user ID to the database and updates the UI.
  saveUserId = function (response) {
    Drupal.instagramProfile.newInstagramUser = false;
    Drupal.instagramProfile.attachBehaviors(document, parseInt(response.data.id, 10));
    Drupal.instagramProfile.userIdApi('POST', { instagram_user_id: response.data.id });
  };

  // Start execution.
  main();
}());

// Sets the text label of the configure/authenticate link.
Drupal.instagramProfile.behaviors.authenticate = function ($list, id) {
  "use strict";
  var label = id
    ? Drupal.t('Share a different Instagram account')
    : Drupal.t('Share your Instagram feed');

  $list.parent().find('.authenticate').text(label);
};

// Appends a button to disable the shared Instagram feed.
Drupal.instagramProfile.behaviors.disable = function ($list, id) {
  "use strict";
  // Check there is:
  //   - a shared feed to disable
  //   - not already a disable button
  if (id && $list.parent().find('.delete-feed').length < 1) {
    var $button = Drupal.theme('instagramProfileDisableButton');

    // Add the button in a wrapper.
    jQuery('<div></div>').append($button).insertBefore($list);

    // When the button is clicked;
    $button.bind('click', function (e) {
      e.preventDefault();
      // Delete the saved user ID in the database.
      Drupal.instagramProfile.userIdApi('DELETE', {}, function () {
        // Reload the page (and reset state) once the user ID has been saved.
        location.reload();
      });
    });
  }
};

// Initializes the demo feed.
Drupal.instagramProfile.behaviors.demoFeed = function ($list, id, settings) {
  "use strict";
  if (settings.demo && !id && !Drupal.instagramProfile.newInstagramUser) {
    $list.children().show();
    Drupal.instagramProfile.behaviors.view($list, settings.demo, settings);
  }
};

// Initializes the call-to-action markup.
Drupal.instagramProfile.behaviors.callToAction = function ($list, id, settings) {
  "use strict";
  if (!id && !Drupal.instagramProfile.newInstagramUser) {
    $list.parent().find('.authenticate').before(Drupal.theme('instagramProfileCallToAction', settings));
  }
};

//
// Default theme implementation for the call to action markup.
//
// Override this in custom javascript with:
// Drupal.theme.instagramProfileCallToAction = function(item, size) {
//   // ...
// };
//
Drupal.theme.prototype.instagramProfileCallToAction = function (settings) {
  "use strict";

  // Prepare variables.
  var markup = '',
    title    = Drupal.t('Add your photos & videos!'),
    alt      = Drupal.t('Instagram logo'),
    source   = settings.assetsDirectory + '/Instagram_Icon_Large.png',
    text     = Drupal.t('Feed your photos and videos here.');

  // The markup.
  markup += '<div class="cta">';
  markup += '  <h4>' + title + '</h4>';
  markup += '  <img class="instagram-logo" alt="' + alt + '" src="' + source + '" />';
  markup += '  <p>' + text + '</p>';
  markup += '</div>';

  return markup;
};

//
// Default theme implementation for the disable button.
//
// Override this in custom javascript with:
// Drupal.theme.instagramProfileDisableButton = function(item, size) {
//   // ...
// };
//
Drupal.theme.prototype.instagramProfileDisableButton = function () {
  "use strict";
  var label = Drupal.t('Stop showing your Instagram feed');
  return jQuery('<button class="disable">' + label + '</button>');
};
