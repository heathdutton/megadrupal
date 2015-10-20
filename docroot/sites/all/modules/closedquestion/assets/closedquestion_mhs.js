// Closed Question movie question
//
// @requires jquery   See http://www.jquery.com
// @requires video.js See http://www.videojs.com/
// @license: GPL-2    See http://www.gnu.org/licenses/gpl-2.0.html
//
// (C) 2013 Kryt BV http://www.kryt.nl
// Based on jQuery Plugin Boilerplate, version 1.1, May 14th, 2011 by Stefan Gabos
(function ($) {
  "use strict";

  Drupal.behaviors.closedQuestionMHS = {
    "attach": function (context) {
      var questionId;

      var settings = Drupal.settings.closedQuestion.mhs;

      for (questionId in settings) {
        /* create cqMoviewQuestion objects */
        var answerContainerId = questionId + "answerContainer";
        var $answerContainer = $("#" + answerContainerId);

        var $videoElement = $answerContainer.find('video').first();
        var $answerFormElement = $('[name=' + questionId + 'answer]');
        var $submitButton = $answerFormElement.closest('form').find('#edit-submit');

        $videoElement.cqMovieQuestion($answerFormElement, $submitButton, settings[questionId]);
      }
    }
  };

  $.cqMovieQuestion = function ($videoElement, $answer_element, $submit_button, settings) {

    /* define vars
     */

    /* this object will be exposed to other objects */
    var publicObj = this;

    //the version number of the publicObj
    publicObj.version = '1.0';

    /* this object holds functions used by the publicObj boilerplate */
    var _helper = {
      /**
       * Call hooks, additinal parameters will be passed on to registered publicObjs
       * @param {string} name
       */
      "doHook": function (name) {
        var i;
        var publicObjFunctionArgs = [];

        /* call function */
        if (_globals.publicObjs !== undefined) {
          /* remove first two arguments */
          for (i = 1; i < arguments.length; i++) {
            publicObjFunctionArgs.push(arguments[i]);
          }

          $.each(_globals.publicObjs, function (cqMovieQuestion, extPlugin) {
            if (extPlugin.__hooks !== undefined && extPlugin.__hooks[name] !== undefined) {
              extPlugin.__hooks[name].apply(publicObj, publicObjFunctionArgs);
            }
          });
        }
      },
      /**
       * Initializes the publicObj
       */
      "doInit": function () {
        _globals.$videoElement.trigger('cqMovieQuestion.beforeInit', publicObj, $videoElement, settings);
        publicObj.init();
        _globals.$videoElement.trigger('cqMovieQuestion.init', publicObj);
      },
      /**
       * Loads an external script
       * @param {string} libName
       * @param {string} errorMessage
       */
      "loadScript": function (libName, errorMessage) {
        /* remember libname */
        _cdnFilesToBeLoaded.push(libName);

        /* load script */
        $.ajax({
          "type": "GET",
          "url": _globals.dependencies[libName].cdnUrl,
          "success": function () {
            /* forget libname */
            _cdnFilesToBeLoaded.splice(_cdnFilesToBeLoaded.indexOf(libName), 1); //remove element from _cdnFilesToBeLoaded array

            /* call init function when all scripts are loaded */
            if (_cdnFilesToBeLoaded.length === 0) {
              _helper.doInit();
            }
          },
          "fail": function () {
            console.error(errorMessage);
          },
          "dataType": "script",
          "cache": "cache"
        });
      },
      /**
       * Registers a publicObj
       * @param {string} name Name of publicObj, must be unique
       * @param {object} object An object {("functions": {},) (, "hooks: {})}
       */
      "registerPlugin": function (name, object) {
        var publicObj;
        var hooks;

        /* reorder publicObj */
        hooks = $.extend(true, {}, object.hooks);
        publicObj = object.functions !== undefined ? object.functions : {};
        publicObj.__hooks = hooks;

        /* add publicObj */
        _globals.publicObjs[name] = publicObj;
      },
      /**
       * Calls a publicObj function, all additional arguments will be passed on
       * @param {string} cqMovieQuestion
       * @param {string} publicObjFunctionName
       */
      "callPluginFunction": function (cqMovieQuestion, publicObjFunctionName) {
        var i;

        /* remove first two arguments */
        var publicObjFunctionArgs = [];
        for (i = 2; i < arguments.length; i++) {
          publicObjFunctionArgs.push(arguments[i]);
        }

        /* call function */
        _globals.publicObjs[cqMovieQuestion][publicObjFunctionName].apply(null, publicObjFunctionArgs);
      },
      /**
       * Checks dependencies based on the _globals.dependencies object
       * @returns {boolean}
       */
      "checkDependencies": function () {
        var dependenciesPresent = true;
        for (var libName in _globals.dependencies) {
          var errorMessage = 'jquery.cqMovieQuestion: Library ' + libName + ' not found! This may give unexpected results or errors.';
          var doesExist = $.isFunction(_globals.dependencies[libName]) ? _globals.dependencies[libName] : _globals.dependencies[libName].doesExist;
          if (doesExist.call() === false) {
            if ($.isFunction(_globals.dependencies[libName]) === false && _globals.dependencies[libName].cdnUrl !== undefined) {
              /* cdn url provided: Load script from external source */
              _helper.loadScript(libName, errorMessage);
            }
            else {
              console.error(errorMessage);
              dependenciesPresent = false;
            }
          }
        }
        return dependenciesPresent;
      }
    };
    /* keeps track of external libs loaded via their CDN */
    var _cdnFilesToBeLoaded = [];

    /* this object holds all global variables */
    var _globals = {};

    /* handle settings */
    var defaultSettings = {
      "videojs_config": {
        "controls": false
      },
      "answersAreReviewable": false,
      "movieCanBePaused": false,
      "startAt": null,
      "stopAt": null,
      "startTitle": "Click to start movie",
      "clickSubmitTreshold": null
    };

    _globals.settings = {};

    if (settings.startTitle === "") {
      delete settings.startTitle;
    }

    if ($.isPlainObject(settings) === true) {
      _globals.settings = $.extend(true, {}, defaultSettings, settings);
    }
    else {
      _globals.settings = defaultSettings;
    }

    /* this object contains a number of functions to test for dependencies,
     * doesExist function should return TRUE if the library/browser/etc is present
     */
    _globals.dependencies = {
      /* check for jQuery 1.6+ to be present */
      "jquery1.6+": {
        "doesExist": function () {
          var jqv, jqv_main, jqv_sub;
          if (window.jQuery) {
            jqv = jQuery().jquery.split('.');
            jqv_main = parseInt(jqv[0], 10);
            jqv_sub = parseInt(jqv[1], 10);
            if (jqv_main > 1 || (jqv_main === 1 && jqv_sub >= 6)) {
              return true;
            }
          }
          return false;
        },
        "cdnUrl": "http://code.jquery.com/jquery-git1.js"
      },
      "videojs": {
        "doesExist": function () {
          if (typeof videojs !== 'undefined') {
            console.log(videojs);
            return true;
          }
          return false;
        }
      }
    };
    _helper.checkDependencies();

    //this object holds all publicObjs
    _globals.publicObjs = {};

    /* register DOM elements
     * jQuerified elements start with $
     */

    /* the container element */
    _globals.$container;

    /* the Drupal answer form element */
    _globals.$drupalAnswerInput = $($answer_element);

    /* the Drupal submit button */
    if ($submit_button.length !== 0) {
      _globals.$drupalSubmitButton = $submit_button;
    } else {
      console.error('ClosedQuestion movie hotspot: Submit button not defined ', $submit_button);
      return;
    }
    /* the overlay shown at the beginning of the movie */
    _globals.$initOverlay;

    /* the video play button */
    _globals.$playButton;

    /* the click counter */
    _globals.$questionClickCounter;

    /* the video element */
    _globals.$videoElement = $($videoElement);

    /* the video container */
    _globals.$videoContainer = null;

    /*
     * Other globals
     */

    /* the Video.js player object */
    _globals.videoJsPlayer = null;

    /* the maximum number of choices before the submit button is automatically pressed */
    _globals.clickSubmitTreshold;

    /* counter for click events */
    _globals.numberOfClickEvents = 0;

    /* video element dimensions */
    _globals.videoElementOriginalWidth;
    _globals.videoElementOriginalHeight;

    /**
     * Init function
     **/
    publicObj.init = function () {
      /* init the videojs player */
      videojs($videoElement, _globals.settings.videojs_config).ready(function () {
        /* get player and video container */
        _globals.videoJsPlayer = this;
        _globals.$videoContainer = _globals.$videoElement.parent(); //created by videojs
        _globals.$videoContainer.addClass('cqMovieQuestionVideoContainer');


        /* parse start time */
        if (_globals.settings.startAt !== null) {
          _globals.settings.startAt = parseInt(_globals.settings.startAt, 10);

          /* go to start time  */
          _globals.videoJsPlayer.on('loadeddata', function () {
            publicObj.goToTime(parseInt(_globals.settings.startAt, 10));
          });
        }

        /* parse end time */
        if (_globals.settings.stopAt !== null) {
          _globals.settings.stopAt = parseInt(_globals.settings.stopAt, 10);
        }

        /* create container and add video container as first child */
        _globals.$container = $('<div class="cqMovieQuestionContainer"></div>');
        _globals.$container.insertBefore(_globals.$videoContainer);
        _globals.$container.append(_globals.$videoContainer);

        /* add play button to container */
        _globals.$playButton = $('<div class="cqMovieQuestionPlayButton">' + Drupal.t('Pause video') + '</div>');

        _globals.$initOverlay = $('<div class="cqMovieQuestionOverlay" style="display:none">' + Drupal.t(_globals.settings.startTitle) + '</div>');
        _globals.$initOverlay.bind('click', function () {
          $(this).hide();
          publicObj.play();
        });
        _globals.$container.append(_globals.$initOverlay);

        /* make sure the video controls are never visible */
        if (_globals.settings.movieCanBePaused === false) {
          _globals.$videoContainer.find('.vjs-control-bar').empty().height(0);
        }
        else {
          _globals.$container.append(_globals.$playButton);
        }

        /* calculate offset and width of video */
        _globals.videoElementOriginalWidth = _globals.$videoElement.width();
        _globals.videoElementOriginalHeight = _globals.$videoElement.height();

        /* max choices, after which answer will be submitted */
        if (_globals.settings.clickSubmitTreshold !== null) {
          _globals.clickSubmitTreshold = parseInt(_globals.settings.clickSubmitTreshold, 10);

          if (_globals.clickSubmitTreshold > 1) {
            var $questionClickCounter;
            var i;

            _globals.$questionClickCounter = $('<div class="cqMovieQuestionClickCounter"><p>Click counter</p></div>');

            _globals.$container.append(_globals.$questionClickCounter);

            for (i = 0; i < _globals.clickSubmitTreshold; i++) {
              _globals.$questionClickCounter.append('<input type="checkbox" />')
            }
          }
        }

        /* mess with Drupal submit button */
        _globals.$drupalSubmitButton.attr('disabled', true);
        _globals.$drupalSubmitButton.bind('mousedown', function () {
          publicObj.reset();
        });

        /* reset answer */
        if (_globals.settings.answersAreReviewable !== true) {
          publicObj.reset();
        }

        /*
         Add events
         */

        /* to play button */
        _globals.$playButton.bind('click', function () {
          if (_globals.videoJsPlayer.paused() === true) {
            publicObj.play();
          }
          else {
            publicObj.pause();
          }
        });

        /* to video container */
        _globals.videoJsPlayer.on('click', function (event) {
          onVideoClick(event);
        });

        /* to video player */
        _globals.videoJsPlayer.on('fullscreenchange, resize', function (event) {
          onVideoResize(event);
        });

        _globals.videoJsPlayer.on('play', function (event) {
          onVideoPlay(event);
        });

        _globals.videoJsPlayer.on('pause', function (event) {
          onVideoPause(event);
        });

        _globals.videoJsPlayer.on('ended', function (event) {
          onVideoEnd(event);
        });

        _globals.videoJsPlayer.on('timeupdate', function (event) {
          onVideoTimeUpdate(event, _globals.videoJsPlayer.currentTime());
        });
      });
    };

    /**
     * Registers a publicObj
     * @param {string} name Name of publicObj, must be unique
     * @param {object} object An object {("functions": {},) (, "hooks: {}) (, "targetcqMovieQuestions": [])}
     */
    publicObj.registerPlugin = function (name, object) {
      _helper.registerPlugin(name, object);
    };

    /**
     * Calls a publicObj function, all additional arguments will be passed on
     * @param {string} cqMovieQuestion
     * @param {string} publicObjFunctionName
     */
    publicObj.callPluginFunction = function (cqMovieQuestion, publicObjFunctionName) {
      /* call function */
      _helper.callPluginFunction.apply(null, arguments);
    };

    /**
     * Starts playing the video
     **/
    publicObj.play = function () {
      publicObj.goToTime(parseInt(_globals.settings.startAt, 10));
      if (_globals.$questionClickCounter !== undefined) {
          _globals.$questionClickCounter.find('input').prop('checked',false);
      }
      _globals.videoJsPlayer.play();
    };

    /**
     * Pauses the video
     **/
    publicObj.pause = function () {
      _globals.videoJsPlayer.pause();
    };

    /**
     * Go to a certain time in the video
     * @param {float} time
     **/
    publicObj.goToTime = function (time) {
      _globals.videoJsPlayer.currentTime(time);
    };

    /**
     * Resets the video question
     **/
    publicObj.reset = function () {
      publicObj.pause();
      publicObj.goToTime(0);
      _globals.numberOfClickEvents = 0;
      _globals.storedClickEvents = {};
      _globals.videoJsPlayer.currentTime(0);
      fillAnswerElement();

      _globals.$initOverlay.show();
    };

    /**
     * Add a click event to the stack
     * @param {integer} time
     * @param {float} x
     * @param {float} y
     * @param {object} gui_element
     * @return {integer} The (internal) id of the event
     **/
    function addClickEvent(time, x, y, gui_element) {
      _globals.storedClickEvents[time] = {
        "x": x,
        "y": y,
        "gui_element": gui_element
      };
      fillAnswerElement();

      _globals.numberOfClickEvents++;

      /* update click counter */
      if (_globals.$questionClickCounter !== undefined) {
        _globals.$questionClickCounter.find('input').eq(_globals.numberOfClickEvents - 1).prop('checked', true);
      }

      /* see if we exceed treshold and should submit */
      if (_globals.numberOfClickEvents === _globals.clickSubmitTreshold) {
        onClickSubmitTreshold();
      }

      _globals.$drupalSubmitButton.removeAttr('disabled');

      return time;
    }

    /**
     * Remove a click event from the stack
     * @param {integer} id
     * @return boolean
     **/
    function removeClickEvent(id) {
      if (_globals.storedClickEvents[id] !== undefined) {
        _globals.storedClickEvents[id] = null;

        fillAnswerElement();
        _globals.numberOfClickEvents--;

        return true;
      }

      return false;
    }

    /**
     * Puts question answer in answer element
     **/
    function fillAnswerElement() {
      var i = 1, time, click_event, answer;
      _globals.$drupalAnswerInput.val('');
      for (time in _globals.storedClickEvents) {
        click_event = _globals.storedClickEvents[time];

        answer = 'd' + i + ',' + click_event.x + ',' + click_event.y + ',' + time + ';';
        _globals.$drupalAnswerInput.val(_globals.$drupalAnswerInput.val() + answer);
        i++;
      }
    }

    /**
     * Event handlers
     **/

    /**
     * Called when user clicks video
     * @param {object} event
     **/
    function onVideoClick(event) {
      if (!_globals.videoJsPlayer.paused()) {
        var videoElement_offset = _globals.$videoElement.offset();
        var correctedClientX = event.pageX - videoElement_offset.left;
        var correctedClientY = event.pageY - videoElement_offset.top;

        var percentageWidth = correctedClientX / _globals.$videoElement.width() * 100;
        var percentageHeight = correctedClientY / _globals.$videoElement.width() * 100;

        var ui_element = $('<div class="cqMovieQuestionCrosshair" />');

        /* add event to stack */
        addClickEvent(_globals.videoJsPlayer.currentTime(), percentageWidth, percentageHeight, ui_element);

        /* add pointer */
        _globals.$videoContainer.append(ui_element);
        ui_element.css({
          "top": correctedClientY - ui_element.height() / 2,
          "left": correctedClientX - ui_element.width() / 2
        });
        ui_element.fadeOut();
      }

      event.preventDefault();
      return false;
    }

    /**
     * Called when the number of max choices is reached
     **/
    function onClickSubmitTreshold() {
      publicObj.pause();
      submitAnswer();
    }

    /**
     * Submits the answer by pressing the Drupal form submit button
     **/
    function submitAnswer() {
      _globals.$drupalSubmitButton.mousedown(); //see CqQuestionAbstract.class::insertSubmit
    }


    /**
     * Called when video is resized
     * @todo make this work
     * @param {object} event
     **/
    function onVideoResize(event) {
      return;
      //      var x_ratio, y_ratio, time, click_event;
      //      var videoElement_height, videoElement_width, new_top, new_left;
      //
      //      /* re-calculate offset of video */
      //      videoElement_offset = _globals.$videoElement.offset();
      //
      //      /* re-position click event gui elements */
      //      videoElement_width = _globals.$videoElement.width();
      //      videoElement_height = _globals.$videoElement.height();
      //
      //      //adjust for new video element size ratio
      //      x_ratio = videoElement_width  / _globals.videoElementOriginalWidth;
      //      y_ratio = videoElement_height / _globals.videoElementOriginalHeight;
      //
      //      for (time in _globals.storedClickEvents) {
      //        click_event = _globals.storedClickEvents[time];
      //
      //       new_top  = click_event.y * videoElement_height * y_ratio / 100;
      //        new_left = click_event.x * videoElement_width  * x_ratio / 100;
      //
      //        click_event.gui_element.css({
      //          "top" : new_top,
      //          "left": new_left
      //        });
      //      }
    }


    function onVideoPlay(event) {
    }

    /**
     * Fired when the current playback position has changed
     * @param {object} event
     * @param {float} currentTime
     */
    function onVideoTimeUpdate(event, currentTime) {
      /* pause video when end time set in settings */
      if (_globals.settings.stopAt !== null && currentTime >= _globals.settings.stopAt) {
        addClickEvent(currentTime, 0, 0, null);
        publicObj.pause();
      }
    }

    function onVideoPause(event) {
      if (_globals.settings.movieCanBePaused !== true) {
        submitAnswer();
      }
    }

    function onVideoEnd(event) {
      submitAnswer();
    }

    /**
     * Triggers an event. Additional arguments to this function are passed
     *  on to the listeners.
     * @param {string} eventName
     **/
    function trigger(eventName) {
      var numberOfEventHandlers, i;
      if (_globals.eventHandlers[eventName] !== undefined) {
        numberOfEventHandlers = _globals.eventHandlers[eventName].length;
        for (i = 0; i < numberOfEventHandlers; i++) {
          _globals.eventHandlers[eventName].apply(null, arguments.shift());
        }
      }
    }

    /* initialize cqMovieQuestion
     */
    if (_cdnFilesToBeLoaded.length === 0) {
      _helper.doInit();
    }
  };

  $.fn.cqMovieQuestion = function ($answer_element, $submit_button, settings) {
    return this.each(function () {
      if (undefined === $(this).data('cqMovieQuestion')) {
        var publicObj = new $.cqMovieQuestion(this, $answer_element, $submit_button, settings);
        $(this).data('cqMovieQuestion', publicObj);
      }
    });
  };

})(jQuery);