
/**
 *@file
 *Javascript functions for the Hotspot questions.
 */

/**
 * Generate the answer string from the draggables, for the question with the
 * given id and puts it in the answer form field for that question.
 *
 * @param questionId string
 *   The question id of the question to generate the answer string for.
 *
 * @return TRUE
 */
function cqCheckAnswerHS(questionId) {
  var returnString = "";
  var qsettings = Drupal.settings.closedQuestion.hs[questionId];
  var length = qsettings.ddDraggableStartPos.length;
  var i;
  for (i = 0; i < length; i++) {
    var draggableVar = qsettings.ddDraggableStartPos[i];
    returnString += "" + draggableVar.cqvalue + "," + draggableVar.x + "," + draggableVar.y + ";";
  }

  var answerElement = jQuery('[name="' + questionId + 'answer"]');
  answerElement.val(returnString);
  return true;
}

(function ($) {
  /**
   * Attach the code that puts all draggables in the right spot, and loads
   * the dragArea to a Drupal behaviour.
   */

  Drupal.behaviors.closedQuestionHS = {
    "attach": function (context) {
      var settings = Drupal.settings.closedQuestion.hs;

      for (var questionId in settings) {
        /* init */
        var qsettings = settings[questionId];
        if (qsettings['initialised']) {
          continue;
        }
        qsettings['initialised'] = true;

        if (context === document) {
          $('#' + questionId + 'answerContainer').bind('CQHotspot.addDot', function (e, id, left, top, byGUI) {
            if (byGUI === true) {
              qsettings.ddDraggableStartPos.push({
                "x": left,
                "y": top,
                "cqvalue": 'd' + id
              });
              cqCheckAnswerHS(questionId);
            }

          });

          $('#' + questionId + 'answerContainer').bind('CQHotspot.removeDot', function (e, id, byGUI) {
            if (byGUI === true) {
              qsettings.ddDraggableStartPos.splice(id, 1);
              cqCheckAnswerHS(questionId);
            }

          });


          $('#' + questionId + 'answerContainer').CQHotspot(qsettings);
        }
      }
    }
  };

  /**
   * CQHotspot
   * based on boilerplate version 1.3
   * @param {object} $ A jQuery object
   **/
  $.CQHotspot = function (element, settings) {

    /* define vars
     */

    /* this object will be exposed to other objects */
    var publicObj = this;

    //the version number of the plugin
    publicObj.version = '1.0';

    /* this object holds functions used by the plugin boilerplate */
    var _helper = {
      /**
       * Call hooks, additinal parameters will be passed on to registered plugins
       * @param {string} name
       */
      "doHook": function (name) {
        var i;
        var returnValue = true;
        var pluginFunctionArgs = [];

        /* remove first two arguments */
        for (i = 1; i < arguments.length; i++) {
          pluginFunctionArgs.push(arguments[i]);
        }

        /* call plugin functions */
        if (_globals.plugins !== undefined) {
          /* call plugins */
          $.each(_globals.plugins, function (_, extPlugin) {
            if (extPlugin.__hooks !== undefined && extPlugin.__hooks[name] !== undefined) {
              returnValue = (returnValue === false || extPlugin.__hooks[name].apply(publicObj, pluginFunctionArgs) === false) ? false : true;
            }
          });
        }

        /* trigger event on main element */
        returnValue = (returnValue === false || _globals.$element.triggerHandler('CQHotspot.' + name, pluginFunctionArgs) === false) ? false : true;

        return returnValue;
      },
      /**
       * Initializes the plugin
       */
      "doInit": function () {
        _helper.doHook('CQHotspot.beforeInit', publicObj, element, settings);
        publicObj.init();
        _helper.doHook('CQHotspot.init', publicObj);
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
       * Registers a plugin
       * @param {string} name Name of plugin, must be unique
       * @param {object} object An object {("functions": {},) (, "hooks: {})}
       */
      "registerPlugin": function (name, object) {
        var plugin;
        var hooks;

        /* reorder plugin */
        hooks = $.extend(true, {}, object.hooks);
        plugin = object.functions !== undefined ? object.functions : {};
        plugin.__hooks = hooks;

        /* add plugin */
        _globals.plugins[name] = plugin;
      },
      /**
       * Calls a plugin function, all additional arguments will be passed on
       * @param {string} CQHotspot
       * @param {string} pluginFunctionName
       */
      "callPluginFunction": function (CQHotspot, pluginFunctionName) {
        var i;

        /* remove first two arguments */
        var pluginFunctionArgs = [];
        for (i = 2; i < arguments.length; i++) {
          pluginFunctionArgs.push(arguments[i]);
        }

        /* call function */
        _globals.plugins[CQHotspot][pluginFunctionName].apply(null, pluginFunctionArgs);
      },
      /**
       * Checks dependencies based on the _globals.dependencies object
       * @returns {boolean}
       */
      "checkDependencies": function () {
        var dependenciesPresent = true;
        for (var libName in _globals.dependencies) {
          var errorMessage = 'jquery.CQHotspot: Library ' + libName + ' not found! This may give unexpected results or errors.';
          var doesExist = $.isFunction(_globals.dependencies[libName]) ? _globals.dependencies[libName] : _globals.dependencies[libName].doesExist;
          if (doesExist.call() === false) {
            if ($.isFunction(_globals.dependencies[libName]) === false && _globals.dependencies[libName].cdnUrl !== undefined) {
              /* cdn url provided: Load script from external source */
              _helper.loadScript(libName, errorMessage);
            } else {
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

    _globals.dotsIdPrefix = 'cqHotspotDot_' + $(element).attr('id') + '_';

    /* keep track of dots */
    _globals.dotCounter = 0;

    /* handle settings */
    var defaultSettings = {
      "dotSize": 10
    };

    _globals.settings = {};

    if ($.isPlainObject(settings) === true) {
      _globals.settings = $.extend(true, {}, defaultSettings, settings);
    } else {
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
      }
    };
    _helper.checkDependencies();

    //this object holds all plugins
    _globals.plugins = {};


    /* register DOM elements
     * jQuerified elements start with $
     */
    _globals.$element = $(element);

    /**
     * Init function
     **/
    publicObj.init = function () {
      _globals.$element.addClass('cqHotspot');
      _globals.$image = _globals.$element.find('img');


      /* create container and fill it with elements
       */
      _globals.$imageContainer = $('<div class="cqHotspotImageContainer" />');
      _globals.$imageContainer.insertBefore(_globals.$image);

      _globals.$imageContainer.append(_globals.$image);

      //click event
      _globals.$image.on('click', onImageClick);

      //load dots
      publicObj.setDots(_globals.settings.ddDraggableStartPos);
    }

    publicObj.setDots = function (dotsData) {
      $.each(dotsData, function (i) {
        var dotData = dotsData[i];

        addDot(dotData.x, dotData.y);
      });

    };


    /**
     * Registers a plugin
     * @param {string} name Name of plugin, must be unique
     * @param {object} object An object {("functions": {},) (, "hooks: {}) (, "targetCQHotspots": [])}
     */
    publicObj.registerPlugin = function (name, object) {
      _helper.registerPlugin(name, object);
    };

    /**
     * Calls a plugin function, all additional arguments will be passed on
     * @param {string} CQHotspot
     * @param {string} pluginFunctionName
     */
    publicObj.callPluginFunction = function (CQHotspot, pluginFunctionName) {
      /* call function */
      _helper.callPluginFunction.apply(null, arguments);
    };

    function onImageClick(event) {

      var imageElement_offset = _globals.$image.offset();
      var correctedClientX = event.pageX - imageElement_offset.left - 4;
      var correctedClientY = event.pageY - imageElement_offset.top - 4;

      /* add dot */
      addDot(correctedClientX, correctedClientY, true);

      event.preventDefault();
      return false;
    }

    /**
     * Adds a dot
     * @param {integer} left
     * @param {integer} top
     * @param {boolean} byGUI True if GUI created dot (false by default)
     */
    function addDot(left, top, byGUI) {
      byGUI = byGUI === undefined ? false : byGUI;

      if (_globals.settings.maxChoices && _globals.settings.maxChoices !== "" && _globals.dotCounter === _globals.settings.maxChoices) {
        return;
      }
      var id = _globals.dotCounter;
      var $dotElement = $('<div id="' + _globals.dotsIdPrefix + id + '" class="cqHotspotDot" />');

      /* add dot */
      _globals.$imageContainer.append($dotElement);
      if (byGUI === true) {
        $dotElement.addClass('big');
        $dotElement.css({
          "top": top - 30,
          "left": left - 30
        });

        $dotElement.animate({
          "top": top,
          "left": left,
          "width": _globals.settings.dotSize,
          "height": _globals.settings.dotSize
        },
        {
          "duration": "fast",
          "complete": function () {
            $dotElement.removeClass('big');
          }
        });
      } else {
        $dotElement.css({
          "top": top,
          "left": left,
          "width": _globals.settings.dotSize,
          "height": _globals.settings.dotSize
        });
      }

      /* let dot be removeable */
      $dotElement.on('click', function () {
        removeDot(id, true);
      });

      /* bookkeeping */
      _helper.doHook('addDot', id, left, top, byGUI);
      _globals.dotCounter++;
    }

    /**
     * Remove a dot
     * @param {integer} id Dot id
     * @param {boolean} byGUI True if GUI created dot (false by default)
     */
    function removeDot(id, byGUI) {
      byGUI = byGUI === undefined ? false : byGUI;
      var $dotElement = $('#' + _globals.dotsIdPrefix + id);
      $dotElement.addClass("removing")
      var position = $dotElement.position();

      $dotElement.animate({
        "top": position.top - 30,
        "left": position.left - 30,
        "width": 60,
        "height": 60,
        "opacity": 0
      },
      {
        "duration": "fast",
        "complete": function () {
          $dotElement.remove();
        }
      });

      _helper.doHook('removeDot', id, byGUI);
    }

    /* initialize CQHotspot
     */
    if (_cdnFilesToBeLoaded.length === 0) {
      _helper.doInit();
    }
  };

  $.fn.CQHotspot = function (settings) {
    return this.each(function () {
      if (undefined === $(this).data('CQHotspot')) {
        var plugin = new $.CQHotspot(this, settings);
        $(this).data('CQHotspot', plugin);
      }
    });
  };
})(jQuery);