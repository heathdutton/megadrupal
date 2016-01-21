/*!
 * jQuery plugin to load and display a recent supporters listing on your website
 * https://github.com/moreonion/jquery-recent-supporters
 *
 * Includes Sizzle.js
 * http://sizzlejs.com/
 *
 * Copyright (c) 2013 more onion
 * Released under the MIT license
 * https://github.com/moreonion/jquery-recent-supporters/blob/master/LICENSE
 *
 */


(function ( $ ) {

  $.fn.recentSupporters = function( options ) {

    // the default poll function
    // it is attached to settings.poll which gets called on every poll
    var pollSupporters = function () {
      // no polling if there is no URL to poll
      if (settings.pollingURL.length < 1) {
        return;
      }

      // do the AJAX request on the polling URL
      jQuery.ajax({
        contentType: 'application/json',
        url: settings.pollingURL,
        success: function(data) {
          // on success: update the container
          updateRecentSupportersContainer(data);
          setNextPoll();
        },
        error: function(data) {
          // on 403 or 404 do *not* set a next polling try
          if (data.status === '403') { // forbidden
            return;
          } else if (data.status === '404') { // not found
            return;
          } else if (pollErrorCount >= settings.maxPollErrorCount ) {
            return;
          } else {
            // do nothing, be quiet, try again but slow down polling
            settings.pollingIntervalMultiplier = settings.pollingIntervalMultiplier * 1.1;
            pollErrorCount = pollErrorCount + 1;
            setNextPoll();
          }
        }
      });

    }

    // These are the defaults.
    var defaults = {
      createListIfMissing: true,
      pollingURL: '',
      pollingInterval: 5000,
      pollingIntervalMultiplier: 1.07,
      poll: pollSupporters,
      cycleSupporters: true,
      cycleInterval: 4000,
      cycleEasing: 'linear',
      updateTimeagoInterval: 5700,
      maxSupportersVisible: 6,
      maxSupportersInContainer: 15,
      showCountry: true,
      maxPollErrorCount: 15,
      nodeID: '0',
      texts: {},
      countries: {}
    }
    var settings = $.extend({}, defaults, options );

    var nextPoll = settings.pollingInterval;
    var $container = this;
    var $ul = $('ul.recent-supporters', $container);
    var lastSupporterTimestamp = getMostRecentTimestamp();
    var pollErrorCount = 0;

    // check if easing is available
    // if not set it to 'linear'
    if (!$.easing || !$.easing[settings.cycleEasing]) {
      settings.cycleEasing = 'linear';
    }


    /**
     * sets the next poll via a timeout
     */
    function setNextPoll () {
      // calculate next poll based on settings
      // and set the timeout for next poll
      nextPoll = Math.floor(nextPoll * settings.pollingIntervalMultiplier);
      setTimeout(function() {
        settings.poll.call();
      }, nextPoll);
    }

    /**
     * Updates the recent supporters container (called upon AJAX success)
     * - checks if any new supporters needs to be inserted
     * - creates ul if needed
     * - calls function to add supporters
     *
     * @param {Object} data The received data, namespaced by an integer
     */
    function updateRecentSupportersContainer (data) {
      var supporters = data.supporters;
      var newSupporters = $.map(supporters, function (s, i) {
        if (parseInt(s.timestamp, 10) > lastSupporterTimestamp) {
          return s;
        }
      });

      // create the ul if the container was initialized empty
      if (settings.createListIfMissing && containerEmpty() && newSupporters.length > 0) {

        // subsitute the placeholder if available
        var $noActivityYet = $('.no-activity-yet', $container);
        if ($noActivityYet.length > 0) {
          $noActivityYet.after('<ul class="recent-supporters">');
          $noActivityYet.hide();
          $ul = $('.recent-supporters', $container);
        } else {
          $container.append('<ul class="recent-supporters">');
          $ul = $('.recent-supporters', $container);
        }
      }

      // sort the supporters by timestamp (in case they are not already)
      // (if more than 1 as 1 needs not to be sorted :)
      if (newSupporters.length > 1) {
        var newSortedSupporters = newSupporters.sort(function(a,b) { 
          return parseFloat(a.timestamp) - parseFloat(b.timestamp);
        });
      }

      // add new sorted supporters
      $.each(newSupporters, function (i, s) {
        addNewSupporter(s);
      });
    }

    /**
     * Adds a new supporter item into the DOM.
     * - checks for settings,
     * - resets visibility if needed,
     * - initializes timeago
     *
     * @param {Object} supporter The supporter object to be inserted.
     */
    function addNewSupporter (supporter) {
      var li = $('li:visible', $ul);
      var $newElement;
      var lastTimestamp = getMostRecentTimestamp();
      var liCount = $('li.supporter', $ul).length;
      // get the most recent supporter element
      // only the first one, if timestamp matches more than one supporter
      var $mostRecentElement = $('li span[data-timestamp="'+lastTimestamp+'"]', $ul).first().parent();

      // insert element hidden to not disturb any ongoing cycling
      $newElement = supporterElement(supporter);
      $newElement.hide();
      // if we created the ul just now, there is no mostRecentElement yet
      if ($mostRecentElement.length > 0) {
        $mostRecentElement.before($newElement);
      } else {
        $ul.append($newElement);
        setVisibleSupporters();
      }
      if ($.fn.timeago) {
        $('.time', $ul).timeago('updateFromDOM');
      }

      // remove supporters from DOM when maxSupporterInContainer treshold is hit
      // needs to remove only the last one as addNewSupporter will only add one at best
      if (settings.maxSupportersInContainer <= liCount) {
        var firstTimestamp = getLeastRecentTimestamp();
        // get the least recent supporter element
        // only the first one, if timestamp matches more than one supporter
        var $leastRecentElement = $('li span[data-timestamp="'+firstTimestamp+'"]', $ul).first().parent();
        $leastRecentElement.remove();

        // need to reset visibility as we do not check if removed element is
        // currently visible or not
        setVisibleSupporters();
      }

      // ensure the correct visibility
      setVisibleSupporters();

      // if we do cycle, we do not show the element immediatly but rely on
      // the cycling to show our new element; if we do not cycle we have to
      // take care for ourself to show the new element
      if (!settings.cycleSupporters) {
        $newElement.slideDown({duration: 200, easing: settings.cycleEasing});
        // hide any elements which are too much
        if ($('li', $ul).length > settings.maxSupportersVisible) {
          $('li:visible', $ul).last().slideUp({duration: 200, easing: settings.cycleEasing});
        }
      }

      // update variable used in next poll
      if (lastSupporterTimestamp < supporter.timestamp) {
        lastSupporterTimestamp = supporter.timestamp;
      }
    }

    /**
     * Generates a new supporter element from supporter object.
     *
     * @param {Object} supporter A single supporter object.
     * @return {Object} Returns a jQuery li object ready to be inserted into the DOM.
     */
    function supporterElement (supporter) {
      var $li = $('<li>');
      var timestamp = supporter.timestamp || Math.floor(new Date().getTime() / 1000);
      var lastName = '<span class="last_name">'+supporter.last_name+'</span>';
      var name = [supporter.first_name, lastName].join(" ")
      var nameHtml = '<span class="name">'+name+'</span>';

      var text = nameHtml;
      if (typeof supporter.action_type !== 'undefined' && supporter.action_type) {
        var tokens = {
          "!supporter_name": nameHtml,
          "!action_title": '<span class="title">'+supporter.action_title+"</span>"
        }
        if (typeof supporter.action_url !== 'undefined') {
          tokens["!action_title"] = '<span class="title"><a href="'+supporter.action_url+'" title="'+supporter.action_title+'">'+supporter.action_title+'</a></span>';
        }
        if (typeof settings.texts[supporter.action_type] !== 'undefined' && settings.texts[supporter.action_type].length > 0) {
          text = replaceTokensInText(settings.texts[supporter.action_type], tokens);
        }
      }
      var supporterHtml = text;
      supporterHtml += "\n"+'<span class="time" data-timestamp="'+timestamp+'" title="'+supporter.rfc8601+'">'+timestamp+'</span>';
      $li.addClass('supporter clearfix').append(supporterHtml);

      if (typeof supporter.action_type !== 'undefined' && supporter.action_type.length > 0) {
        $li.addClass('action-type-'+supporter.action_type);
      }

      if (settings.showCountry) {
        var countryCode = supporter.country ? supporter.country.toLowerCase() : "no-cc";
        var countryName = supporter.country in settings.countries ? settings.countries[supporter.country] : '';
        $li.prepend('<span title="'+countryName+'" class="country flag flag-'+countryCode+'">'+supporter.country+'</span> ');
      }

      return $li;
    }

    /**
     * Subsitutes tokens in a string with provided replacement strings.
     *
     * @param {String} text A string with tokens to replace (tokens like "!token")
     * @param {Object}  tokens A object with token strings (e.g. "!token") as keys, and substitution strings (e.g. "Subsitution string") as values.
     * @return {String} Returns the text string with any found tokens replaced, but leaves tokens where no substitution was provided.
     */
    function replaceTokensInText (text, tokens) {
      text = text.replace(/(![A-Za-z_-]+)/g, function(a,b) {
        return (typeof tokens[b] !== 'undefined') ? tokens[b] : b;
      });
      return text
    }

    /**
     * Checks if the container is empty.
     * e.g. when loaded without any supporters in initial markup
     *
     * @return {Boolean} Returns true if empty, false otherwise
     */
    function containerEmpty () {
      return $('ul.recent-supporters', $container).length > 0 ? false : true;
    }

    /**
     * Get the most recent timestamp of the supporters.
     *
     * @return {Integer} Returns the most recent timestamp
     */
    function getMostRecentTimestamp () {
      var li = $('li .time', $ul);
      var timestamps = li.map(function (i, el) {
        return parseInt(el.getAttribute('data-timestamp'), 10);
      });
      return Math.max.apply(Math, timestamps.get());
    }

    /**
     * Get the least recent timestamp of the supporters.
     *
     * @return {Integer} Returns the least recent timestamp
     */
    function getLeastRecentTimestamp () {
      var li = $('li .time', $ul);
      var timestamps = li.map(function (i, el) {
        return parseInt(el.getAttribute('data-timestamp'), 10);
      });
      return Math.min.apply(Math, timestamps.get());
    }

    /**
     * Sets the visibility of the supporters.
     * Show only the maxSupportersVisible number of supporters.
     */
    function setVisibleSupporters() {
      $('li.supporter', $ul).show().slice(settings.maxSupportersVisible).hide();
    }

    /**
     * Cycles to the next supporter. Cycles only 1 by 1 supporter.
     */
    function cycleSupporters() {
      var supporterElements = $('li.supporter', $ul);
      // move last element to top in list and show hide
      // if there is at least one supporter more loaded then shown
      if (supporterElements.length > settings.maxSupportersVisible) {
        var minHeight = $container.height();
        $container.css({minHeight: minHeight + 'px'});
        $('li:visible', $ul).last().slideUp({duration: 200, easing: settings.cycleEasing});
        supporterElements.last().hide().detach().prependTo($ul).slideDown({duration: 200, easing: settings.cycleEasing});
      }
    }

    // set number of visible supporters
    setVisibleSupporters();

    // start cycling if enabled
    if (settings.cycleSupporters) {
      setInterval(function() {
        cycleSupporters();
      }, settings.cycleInterval);
    }

    // start the polling if we have the requested box in the markup
    if (this.length > 0) {
      settings.poll.call();

      // initialize timeago (if available)
      if ($.fn.timeago) {
        $('.time', $ul).timeago('updateFromDOM');
        // set update interval
        setInterval(function() {
          $('.time', $ul).timeago('updateFromDOM');
        }, settings.updateTimeagoInterval);
      }
    }

    return this;
  }
})(jQuery);

