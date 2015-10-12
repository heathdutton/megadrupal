(function ($) {
// START jQuery

Drupal.BlueJeans = Drupal.BlueJeans || {};

Drupal.behaviors.BlueJeans = {
  attach: function(context, settings) {

    // Loop over all conferences, updating their status.
    $('.bluejeans-conference').not('.bluejeans-conference-processed').each(function() {
      var $this = $(this);
      $this.addClass('bluejeans-conference-processed');
      var meetingId = $this.attr('data-meetingId');
      // see bluejeans_preprocess_bluejeans_conference() for
      // an explanation of why we add the 'key.' prefix here.
      var meeting = Drupal.settings.BlueJeans.meetings['key.' + meetingId];
      meeting.metadata = JSON.parse(meeting.metadata);
      meeting.$container = $this;
      Drupal.BlueJeans.updateMeetingStatus(meeting);
    });

    $('ul.bluejeans-conference-actions a.conference-action-popup').live('click', function(e) {
      // Prevent click from reaching default handler.
      e.preventDefault();

      // Open popup in a new window center screen.
      var width = 1024,
          height = 768,
          left  = ($(window).width()-width)/2,
          top   = ($(window).height()-height)/2,
          popup = window.open(
            $(this).attr('href'), 
            "popup-"+time(), 
            "width="+width+",height="+height+",top="+top+",left="+left
          );
    });
  }
}

/**
 * Get meeting state from BlueJeans service.
 * BJN doesn't support CORS, so we proxy calls through our server.
 */
Drupal.BlueJeans.getMeetingStatus = function(meeting, fn) {
  $.ajax({
    url: Drupal.settings.basePath + 'bluejeans/conference/' + meeting.meeting_id + '/status',
    dataType: 'json',
    success: function(data) {
      if (typeof(data.error) != 'undefined') {
        Drupal.BlueJeans.displayMessage(data.error, 'error', meeting.$container); 
      }
      else {
        fn(data.status);
      }
    },
    error: function(x,t,e) {
      Drupal.BlueJeans.displayMessage((t ? (t+': ') : '') + e, 'error', meeting.$container);
    }
  });
}
Drupal.BlueJeans.getMeetingEndpoints = function(meeting, fn) {
  $.ajax({
    url: Drupal.settings.basePath + 'bluejeans/conference/' + meeting.meeting_id + '/endpoints',
    dataType: 'json',
    success: function(data) {
      if (typeof(data.error) != 'undefined') {
        Drupal.BlueJeans.displayMessage(data.error, 'error', meeting.$container);
      }
      else {
        fn(data);
      }
    },
    error: function(x,t,e) {
      Drupal.BlueJeans.displayMessage((t ? (t+': ') : '') + e, 'error', meeting.$container);
    }
  });
}

/**
 * Display status message.
 */
Drupal.BlueJeans.displayMessage = function(message, type, $container) {
  $('.bluejeans-conference-messages', $container).append(
    $('<div />', { class: "messages " + type }).text(message)
  );
}

/**
 * Update meeting status and affordances based on its internal state:
 * upcoming, pending start, active, finished.
 */
Drupal.BlueJeans.updateMeetingStatus = function(meeting) {
  var clss = 'conference-status-unknown';
  var text = Drupal.t('refreshing status...');
  var actions = [];

  // If time < start, we're waiting for the meeting to start.
  if (time() < meeting.start) {
    clss = 'conference-status-upcoming';
    text = Drupal.t('upcoming');
    // Schedule this function again when the time comes for the meeting.
    setTimeout(
      function() { Drupal.BlueJeans.updateMeetingStatus(meeting, meeting.$container); },
      Math.max(0, 1000 * (meeting.start - time())) // in milliseconds
    );
    // If meeting owner, afford canceling meeting.
    if (Drupal.settings.BlueJeans.user.is_conference_owner) {
      actions.push({
        title: Drupal.t('cancel'),
        href: Drupal.settings.BlueJeans.basePathLocalized + 'conference/' + meeting.nid + '/' + meeting.meeting_id + '/cancel',
        attributes: {
          class: 'conference-action-cancel',
          query: {
            destination: 'node/' + meeting.nid
          }
        }
      });
      actions.push({
        title: Drupal.t('edit'),
        href: Drupal.settings.BlueJeans.basePathLocalized + 'conference/' + meeting.nid + '/' + meeting.meeting_id + '/edit',
        attributes: {
          class: 'conference-action-edit',
          query: {
            destination: 'node/' + meeting.nid
          }
        }
      });
    }

    // TODO Show countdown to meeting start.

    Drupal.BlueJeans.updateMeetingStatusUI(meeting.$container, clss, text);
    Drupal.BlueJeans.updateMeetingActionsUI(meeting.$container, actions);
  }
  else {
    // Meeting is about to start, is active, or has terminated: find out which.
    // First ask the BlueJeans service about the meeting status.
    Drupal.BlueJeans.getMeetingStatus(meeting, function(state) {
      if (state == 'active') {
        // Check meeting participants.
        Drupal.BlueJeans.getMeetingEndpoints(meeting, function(endpoints) {
          var found = false;
          $.each(endpoints, function(i, endpoint) {
            if (endpoint.name == Drupal.settings.BlueJeans.user.name) {
              found = true;
            }
          });
          if (!found) {
            // If not in meeting, afford joining meeting.
            var actions = [];
            actions.push({
              title: Drupal.t('join live'),
              href: 'http://bluejeans.com/' + meeting.metadata.numericMeetingId,
              attributes: {
                class: 'conference-action-join conference-action-popup',
                query: {
                  name: Drupal.settings.BlueJeans.user.name
                }
              }
            });

            clss = 'conference-status-active';
            text = Drupal.t('active');

            Drupal.BlueJeans.updateMeetingStatusUI(meeting.$container, clss, text);
            Drupal.BlueJeans.updateMeetingActionsUI(meeting.$container, actions);
          }
          else {
            // Show user is connected.
            clss = 'conference-status-connected conference-status-active';
            text = Drupal.t('connected');

            Drupal.BlueJeans.updateMeetingStatusUI(meeting.$container, clss, text);
          }
                  
          // Update status every 15 seconds.
          setTimeout(
            function() { Drupal.BlueJeans.updateMeetingStatus(meeting); },
            15000 // in milliseconds
          );
        });
      }
      else {
        // Meeting not active, so it's either about to start or has finished.
        // Check the meeting end time to know which.
        if (time() < meeting.end) {
          clss = 'conference-status-pending';
          text = Drupal.t('ready to start');
          // Update status every 15 seconds.
          setTimeout(
            function() { Drupal.BlueJeans.updateMeetingStatus(meeting); },
            15000 // in milliseconds
          );
          // If meeting owner, afford starting meeting.
          if (Drupal.settings.BlueJeans.user.is_conference_owner) {
            actions.push({
              title: Drupal.t('start'),
              href: 'http://bluejeans.com/' + meeting.metadata.numericMeetingId + '/' + Drupal.settings.BlueJeans.session.user.room.moderatorPasscode,
              attributes: {
                class: 'conference-action-start conference-action-popup',
                query: {
                  name: Drupal.settings.BlueJeans.user.name
                }              
              }
            });
          }
        }
        else {
          clss = 'conference-status-finished';
          text = Drupal.t('finished');
        }
      }

      Drupal.BlueJeans.updateMeetingStatusUI(meeting.$container, clss, text);
      Drupal.BlueJeans.updateMeetingActionsUI(meeting.$container, actions);
    });
  }
}

/**
 * Update meeting status on the page.
 */
Drupal.BlueJeans.updateMeetingStatusUI = function($container, clss, text) {
  // Update the status element.
  $('.bluejeans-conference-status', $container)
    .removeClassPrefix('conference-status')
    .addClass(clss)
    .text(text)
    .parents('.bluejeans-conference').removeClassPrefix('conference-status').addClass(clss + '-wrapper');
}

/**
 * Update meeting actions on the page.
 */
Drupal.BlueJeans.updateMeetingActionsUI = function($container, actions) {
  var $actions = $('.bluejeans-conference-actions', $container).empty();
  $.each(actions, function(i, action) {
    var href = action.href;
    if (typeof action.attributes.query != 'undefined') {
      var query = [];
      $.each(action.attributes.query, function(key, value) {
        query.push(encodeURIComponent(key) + '=' + encodeURIComponent(value));
      });
      href += '?' + query.join('&');
    }
    $actions.append(
      $('<li>').append(
        $('<a>').attr('href', href).text(action.title).addClass(action.attributes.class)
      )
    );
  });
}

// UTILS =======================================================================

// @see http://phpjs.org/functions/time/
function time () {
  // http://kevin.vanzonneveld.net
  // +   original by: GeekFG (http://geekfg.blogspot.com)
  // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   improved by: metjay
  // +   improved by: HKM
  // *     example 1: timeStamp = time();
  // *     results 1: timeStamp > 1000000000 && timeStamp < 2000000000
  return Math.floor(new Date().getTime() / 1000);
}

// @see https://gist.github.com/jakubp/2881585
$.fn.removeClassPrefix = function (prefix) {
  // Remove classes that have given prefix
  // Example:
  // You have an element with classes "apple juiceSmall juiceBig banana"
  // You run:
  // $elem.removeClassPrefix('juice');
  // The resulting classes are "apple banana"

  // NOTE: discussion of implementation techniques for this, including why simple RegExp with word boundaries isn't correct:
  // http://stackoverflow.com/questions/57812/jquery-remove-all-classes-that-begin-with-a-certain-string#comment14232343_58533
  this.each( function ( i, it ) {
    var classes = it.className.split(" ").map(function (item) {
      return item.indexOf(prefix) === 0 ? "" : item;
    });
    it.className = $.trim(classes.join(" "));
  });
  return this;
}

// END jQuery
})(jQuery);
