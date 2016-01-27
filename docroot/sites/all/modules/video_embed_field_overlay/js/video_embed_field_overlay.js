(function($) {
  var console = window.console || false;
  /**
   * Finds the links to the videos and attaches the overlay behavior
   */
  Drupal.behaviors.videoEmbedFieldOverlay = {
    ready: [],
    timer: null,
    players: {},
    playOnPopUp: undefined,

    /**
     * Initial load function, preps videos for popup, sets up initial event chain.
     */
    attach: function (context, settings) {
      Drupal.behaviors.videoEmbedFieldOverlay.players = $('.video-overlay-source iframe', context);

      if ($.isFunction($.openDOMWindow)) {
        Drupal.behaviors.videoEmbedFieldOverlay.players.each(function(i, element) {
          if ($(element).attr('src').indexOf('vimeo.com') !== -1) {
            Drupal.behaviors.videoEmbedFieldOverlay.vimeoProcessElement(element);
          }
        });

        // Setup message handlers
        if (window.addEventListener) { // all browsers except IE before version 9
          window.addEventListener('message', Drupal.behaviors.videoEmbedFieldOverlay.onMessageReceived, false);
        }
        else if (window.attachEvent) { // IE before version 9
          var attached = window.attachEvent('onmessage', Drupal.behaviors.videoEmbedFieldOverlay.onMessageReceived, false);
          if (attached === false) {
            if (console) {console.log('Failed to attach the listener');}
          }
        }

        // Find all the trigger links
        $('.video-overlay-thumbnail a.overlay', context).bind('click', function(e) {
          // Prevent going to the URL
          e.preventDefault();

          // Get the id of the video
          // @TODO: Find cleaner way to get this.
          var id = $(this).parent().parent().find('.video-overlay-source iframe').attr('id').replace('overlay-video-', '');
          Drupal.behaviors.videoEmbedFieldOverlay.videoOnClick(id);
        });
      } else {
        if (console) {console.log('Cannot create DOMWindow');}
      }
    },

    /**
     * Triggers the DOM Windwow and the autoplay
     */
    videoOnClick: function (id) {
      var f = $('iframe#' + id).parent().get(0) || $('iframe').parent().get(0);
      var iframe = $('iframe#' + id).get(0) || $('iframe').get(0);

      // Setup the DOM Window
      $.openDOMWindow({
        loader:0,
        width:iframe.width || 640,
        height:iframe.height || 360,
        windowSourceID:f
      });

      // Play: Will not work on IE and some older versions of players.
      if (Drupal.behaviors.videoEmbedFieldOverlay.browserSupported() === true) {
        // Since we're moving the video from the page to the popup, we need to let the video know to
        // start playing AFTER it's been loaded in the new place, instead of sending the command directly.
        // We do this by telling this script which video to play once it's been loaded and ready to go.
        Drupal.behaviors.videoEmbedFieldOverlay.playOnPopUp = 'overlay-video-' + id;
      }
    },
    setupListeners: function (id) {
      var f = $('iframe#' + id).get(0) || $('iframe').get(0),
      url = '*';
      if (f.contentWindow.postMessage) {
        f.contentWindow.postMessage({
          method: 'addEventListener',
          value: 'play'
        }, url);
        f.contentWindow.postMessage({
          method: 'addEventListener',
          value: 'pause'
        }, url);
        f.contentWindow.postMessage({
          method: 'addEventListener',
          value: 'finish'
        }, url);
        Drupal.behaviors.videoEmbedFieldOverlay.setStatus(id, 'listening');
        if (console) {console.log('Listeners setup for ' + id);}
      } else {
        if (console) {console.log('Browser does not support postMessage');}
      }
    },
    /**
     * Notes:
     * The postMessage method is synchronous in Internet Explorer and 
     * asynchronous in other browsers. 
     */
    play: function (id) {
      var f = $('iframe#' + id).get(0) || $('iframe').get(0),
      url = '*';

      if (Drupal.behaviors.videoEmbedFieldOverlay.getStatus(id) !== 'play') {
        if (f.contentWindow.postMessage) {
          // Trigger video play
          var messageData = JSON.stringify({method: 'play', player_id: id});
          f.contentWindow.postMessage(messageData, url);
        }
        else {
          if (console) {console.log('Your browser does not support the postMessage method!');}
        }
      }
    },

    /**
     * Assigns a status to an array of player ids
     */
    setStatus: function (id, status) {
      Drupal.behaviors.videoEmbedFieldOverlay.ready[id] = status;

      if (console) {
        console.log('Video: ' + id + ' is ' + status);
      }

      // react to certain events.
      var eventHandler = 'onStatus' + status.charAt(0).toUpperCase() + status.slice(1);

      if (Drupal.behaviors.videoEmbedFieldOverlay[eventHandler] !== undefined) {
        Drupal.behaviors.videoEmbedFieldOverlay[eventHandler](id);
      }
    },

    /**
     * Retrieves the status for a given player id
     */
    getStatus: function (id) {
      return Drupal.behaviors.videoEmbedFieldOverlay.ready[id];
    },

    /**
     * Handles the messages received by the listener
     */
    onMessageReceived: function(e) {
      var data = jQuery.parseJSON(e.data);
      Drupal.behaviors.videoEmbedFieldOverlay.setStatus(data.player_id, data.event);
    },

    // Status event callbacks.

    /**
     * onStatusReady: Called when the video is ready to be interacted with.
     */
    onStatusReady: function(id) {
      Drupal.behaviors.videoEmbedFieldOverlay.setupListeners(id);

      if (Drupal.behaviors.videoEmbedFieldOverlay.playOnPopUp === id) {
        Drupal.behaviors.videoEmbedFieldOverlay.playOnPopUp = undefined;
        Drupal.behaviors.videoEmbedFieldOverlay.play(id);
      }
    },

    /**
     * onStatusFinish: Called when video playback is complete.
     */
    onStatusFinish: function(id) {
      $.closeDOMWindow();
    },

    /**
     * Checks to see if the current browser supports autoPlayback. Should be rewritten to use feature
     * detection instead of browser sniffing.
     */
    browserSupported: function () {
      if ($.browser.msie === true ) {
        if (window.console) {window.console.log('IE does not support the autoplay feature');}
        return false;
      }
      if ($.browser.mozilla === true && $.browser.version.slice(0,3) == '1.9') {
        if (window.console) {window.console.log('Older versions of Mozilla do not support the autoplay feature');}
        return false;
      }
      if ($.browser.opera === true) {
        if (window.console) {window.console.log('Opera does not support the autoplay feature');}
        return false;
      }
      return true;
    },

    /**
     * Handles Vimeo-specific code on the embed element. 
     *
     * For Vimeo popup, we need to drop in an #id if there isn't one already, and we need to also let
     * Vimeo know what the id is via a URL parameter. Also, we need to ensure that we let Vimeo know 
     * that we want to use their JS API via another URL parameter. Written in a way that ensures we 
     * don't lose any other url parameters passed in.
     */
    vimeoProcessElement: function(element) {
      var src = $(element).attr('src');
      var href = src.split('?')[0];
      var videoID = href.split('/video/')[1];
      
      $(element).attr('id', 'overlay-video-' + videoID);

      var urlAttributes = src.split('?')[1];
      var urlAttributesObject = urlAttributes.split('&');
      var urlKeyedAttributes = {};

      for (var i in urlAttributesObject) {
        var components = urlAttributesObject[i].split('=');
        urlKeyedAttributes[components[0]] = components[1];
      }

      urlKeyedAttributes['api'] = 1;
      urlKeyedAttributes['player_id'] = 'overlay-video-' + videoID;

      var newURLAttributes = [];
      for (i in urlKeyedAttributes) {
        newURLAttributes.push(i + "=" + urlKeyedAttributes[i]);
      }
      $(element).attr('src', href + '?' + newURLAttributes.join('&'));
    }
  };
})(jQuery);
