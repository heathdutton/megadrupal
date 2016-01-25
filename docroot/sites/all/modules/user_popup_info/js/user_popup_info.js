Drupal.behaviors.user_popup_info = {
  attach: function(context) {
    var sent_ajax = null;

    // Check if user links are to be auto detected.
    if (Drupal.settings.user_popup_info.auto_detect_links == 1) {
      var length = jQuery('a').length;
      if (length > 0) {

        // Add processing class on each link.
        jQuery('a').each(function() {
          jQuery(this).addClass('user-popup');
        });
      }
    }

    // Send AJAX request to fetch user info.
    jQuery('a.user-popup').mouseenter(function() {
      var link = jQuery(this).attr('href');
      var element = jQuery(this);

      // Remove any previously existing popup info.
      var existing = jQuery('.user-popup-processed').length;
      if (existing > 0) {

        // Abort ajax when new is to be fired.
        if (sent_ajax) {
          sent_ajax.abort();
        }
        jQuery('.user-popup-processed').remove();
        jQuery('#user-popup-processed-id').remove();
      }

      // Remove any unwanted tooltip.
      element.attr('title', '');

      // Check original user path, if false, check for user alias path.
      if (user_popup_info_user_profile(link)) {
        element.css('position', 'relative');

        // Make sure this link is not in the excluded list.
        var excluded = Drupal.settings.user_popup_info.excluded_links;
        if (excluded.length == 0) {
          excluded = '/user/logout';
        }

        if (excluded.match(link) == null) {

          // Add loader.
          element.append(jQuery('<div id="user-popup-processed-id"><img src="' + Drupal.settings.user_popup_info.throbber + '" />Loading.....</div>'));

          // Delay AJAX request for 500 ms between different calls.
          setTimeout(function() {
            sent_ajax = jQuery.ajax({
              async: false,
              url: Drupal.settings.user_popup_info.site_url + '/user-popup-info',
              type: 'post',
              data: {
                user_popup_path: link
              },
            }).done(function(response) {
              if (!jQuery('.user-popup-processed').length) {

                // Show user info in small popup.
                if (response !== 'no-result') {
                  if (jQuery('#user-popup-processed-id').length === 0) {
                    element.append(jQuery('<div class="user-popup-processed">' + response + '</div>').css('position', 'absolute'));
                  }
                  else {
                    jQuery('#user-popup-processed-id').replaceWith(jQuery('<div class="user-popup-processed">' + response + '</div>').css('position', 'absolute').prepend(jQuery('<div id="user-popup-info-triangle"></div>')));
                  }
                }
                else {
                  jQuery('#user-popup-processed-id').remove();
                }
              }
              else {
                jQuery('#user-popup-processed-id').remove();
              }
            });
          }, 500);
        }
      }
    });

    // Remove user popup when mouse leaves from over that link.
    jQuery('a.user-popup').mouseleave(function() {

      // Abort ajax request on mouse leave.
      if (sent_ajax) {
        sent_ajax.abort();
      }

      jQuery('.user-popup-processed').remove();
      jQuery('#user-popup-processed-id').remove();
    });

    // Remove user popup when mouse clicked on body.
    jQuery('body').click(function() {

        // Abort ajax request on mouse leave.
        if (sent_ajax) {
          sent_ajax.abort();
        }
        jQuery('.user-popup-processed').remove();
        jQuery('#user-popup-processed-id').remove();
    });

    // Perform nothing when mini profile is clicked.
    jQuery('.user-popup-processed').live('click', function() {
      return false;
    });
  }
}

/**
 * Function to check if link contains words with user/.
 * @param {string} link
 * @returns {Boolean}
 */
function user_popup_info_user_profile(link) {
  link = link.replace(/[/]*/, '');

  if (user_popup_info_user_link_original_valid(link)) {
    return true;
  }
  else if (user_popup_info_user_link_alias_valid(link)) {
    return true;
  }
  else {
    return false;
  }
}

/**
 * Function to remove any unwanted leading path words.
 * @param {string} link
 * @returns {Boolean}
 */
function user_popup_info_user_link_original_valid(link) {
  strpos = link.indexOf('user/');

  // true - exists in the user path.
  if (strpos !== -1) {
    substr = link.substr(strpos, link.length - strpos);
    if (substr.indexOf('/')) {

      // Check if this is not an admin url.
      substr_admin = substr.indexOf('admin');
      if (substr_admin === -1) {
        return true;
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}

/**
 * Function to remove any unwanted leading path words.
 * @param {string} link
 * @returns {Boolean}
 */
function user_popup_info_user_link_alias_valid(link) {
  var alias = Drupal.settings.user_popup_info.user_alias_support;

  if (alias != 'user/') {
    strpos = link.indexOf(alias);

    // true - exists in the user path.
    if (strpos !== -1) {
      return true;
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}