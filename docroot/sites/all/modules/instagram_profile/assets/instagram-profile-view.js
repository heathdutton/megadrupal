/**
 * @file
 * Allows a user to save their Instagram user ID.
 */

// Configure JS Lint.
/*jslint white: true */
/*global jQuery, Drupal */


// Initializes Drupal.instagramProfile.
Drupal.instagramProfile = Drupal.instagramProfile || {
  behaviors: []
};

// Gets data from the Instagram API.
Drupal.instagramProfile.get = function (resource, callback, data) {
  "use strict";

  // Use JSONP, since Instagram has not implemented CORS.
  var url = 'https://api.instagram.com/v1/' + resource + '/?callback=?';

  // Initialize the data variable.
  data = data || {};

  // Specify a client ID if an access token has not been provided.  Most
  // resource endpoints accept just a client ID.  Some, such as `users/self`,
  // require an access token to identify the "current" user ("self").
  if (!data.accessToken) {
    data.client_id = Drupal.settings.instagramProfile.clientId;
  }

  jQuery.getJSON(url, data, callback);
};

// Attaches instagramProfile's behaviors to instagram-profile feeds/lists.
Drupal.instagramProfile.attachBehaviors = function (context, id) {
  "use strict";
  var behaviors = Drupal.instagramProfile.behaviors;

  if (!Drupal.instagramProfile.newInstagramUser) {
    // Get any feeds/lists on the page and iterate over them.
    jQuery(context).find('ul.instagram-profile-feed').once('instagram-profile-feed', function () {
      var $list, i;

      // This feed/list.
      $list = jQuery(this);

      // Invoke all behaviors with the current ID.
      for (i in behaviors) {
        if (behaviors.hasOwnProperty(i)) {
          behaviors[i]($list, id || $list.data('instagram-id'), Drupal.settings.instagramProfile);
        }
      }
    });
  }
};

// When Drupal attaches it's behaviors;
Drupal.behaviors.instagramProfile = {
  attach: function (context) {
    "use strict";
    // Attach instagramProfile's behaviors.
    Drupal.instagramProfile.attachBehaviors(context);
  }
};

// Loads an Instagram feed to view it's media items.
Drupal.instagramProfile.behaviors.view = function ($list, id, settings) {
  "use strict";
  if (!id) {
    // Hide the throbber if there is no ID to load.
    $list.children().hide();
  }
  else {
    // Retrieve the feed.
    Drupal.instagramProfile.get('users/' + id + '/media/recent', function (response) {
      var message;
      // Once retrieved;
      // Hide the throbber.
      $list.children().remove();

      if (response.data && response.data.length) {
        // Show the items.
        while (response.data.length) {
          // Most recent items are at the end of the feed data.
          $list.append('<li>' + Drupal.theme('instagramItem', response.data.shift()) + '</li>');
        }
      }
      else {
        // Or deal with an empty/error response.
        if (settings.userName) {
          message = Drupal.t('!name has no public items in their feed.', { '!name': settings.userName });
        }
        else {
          message = Drupal.t('You have no public items in your feed.  If you have no photos or videos, add some.  Otherwise, try making your Instagram feed public.');
        }
        $list.append('<li>' + message + '</li>');
      }
    });
  }
};

//
// Default theme implementation for an instagram media item.
//
// Override this in custom javascript with:
// Drupal.theme.instagramItem = function(item, size) {
//   // ...
// };
//
Drupal.theme.prototype.instagramItem = function (item, size) {
  "use strict";
  var image, alt,
      markup  = '',
      user    = item.user,
      // Not all media items have a caption property.
      caption = item.caption ? item.caption.text : '';

  // Default to low resolution images.
  size = size || 'low';
  size += '_resolution';
  image = item.images[size];

  // The image's alt="" attribute.
  alt = Drupal.t('@name&rsquo;s photo', { '@name': user.full_name });

  // The markup.  A real templating engine would make this much cleaner.
  markup += '<div class="profile">';
  markup += '  <img src="' + user.profile_picture + '" alt="' + alt + '" title="' + user.username + '" />';
  markup += '  <span class="name" title="' + user.username + '">' + user.full_name + '</span>';
  markup += '</div>';

  markup += '<img class="thumbnail" src="' + image.url + '" alt="' + caption + '" height="' + image.height + '" width="' + image.width + '" />';

  markup += '<p class="likes">' + item.likes.count + ' Likes</p>';
  markup += '<p class="caption">' + caption + '</p>';

  return markup;
};
