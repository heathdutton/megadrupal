/**
 * Closed Question Chemical Reaction question
 * @license: GPL-2    See http://www.gnu.org/licenses/gpl-2.0.html
 * @author Koos van der Kolk / Kryt B.V. The Netherlands (www.kryt.nl)
 */
(function($) {
  "use strict";

  /**
   * Add Drupal behavior: Connect the cqArrowQuestion to the
   * Drupal form element
   */
  Drupal.behaviors.closedQuestionArrow = {
    "attach": function(context) {
      var questionId;
      var settings = Drupal.settings.closedQuestion.cr;

      for (questionId in settings) {
        /* create cqArrowQuestion objects */
        var $answerContainer = $("#" + questionId + "answerContainer");
        var $image_element = $answerContainer.find('img').first();
        var $answerFormElement = $('input[name=' + questionId + 'answer]');

        var cqArrowQuestion = $image_element.cqArrowQuestion(settings[questionId]).data('cqArrowQuestion');

        /* set current answer */
        $image_element.bind('cqArrowQuestion.init', function() {
          cqArrowQuestion.setAnswer($answerFormElement.val());
        });

        var onUpdateAnswer = function() {
          $answerFormElement.val(cqArrowQuestion.getAnswer());
        };

        /* let question tell us when user updates answer */
        cqArrowQuestion.registerPlugin("submitFeedback", {"hooks": {
            "onAddLineToAnswer": onUpdateAnswer,
            "onRemoveLineFromAnswer": onUpdateAnswer
          }});

      }
    }
  };

  /**
   * Turns an image into a arrow question
   * @param {object} element The image
   * @param {object} settings Settings (see defaultSettings declaration in code)
   * @returns {object} The question objects public properties
   */
  $.cqArrowQuestion = function(element, settings) {
    /* define vars
     */

    /* this object will be exposed to other objects */
    var publicObj = this;

    //the version number of the plugin
    publicObj.version = '1.1'

    /* this object holds functions used by the plugin boilerplate */
    var _helper = {
      /**
       * Call hooks, additinal parameters will be passed on to registered plugins
       * @param {string} name
       */
      "doHook": function(name) {
        var i;
        var pluginFunctionArgs = [];

        /* call function */
        if (_globals.plugins !== undefined) {
          /* remove first two arguments */
          for (i = 1; i < arguments.length; i++) {
            pluginFunctionArgs.push(arguments[i]);
          }

          $.each(_globals.plugins, function(cqArrowQuestion, extPlugin) {
            if (extPlugin.__hooks !== undefined && extPlugin.__hooks[name] !== undefined) {
              extPlugin.__hooks[name].apply(publicObj, pluginFunctionArgs);
            }
          });
        }
      },
      /**
       * Registers a plugin
       * @param {string} name Name of plugin, must be unique
       * @param {object} object An object {("functions": {},) (, "hooks: {})}
       */
      "registerPlugin": function(name, object) {
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
       * @param {string} cqArrowQuestion
       * @param {string} pluginFunctionName
       */
      "callPluginFunction": function(cqArrowQuestion, pluginFunctionName) {
        var i;

        /* remove first two arguments */
        var pluginFunctionArgs = [];
        for (i = 2; i < arguments.length; i++) {
          pluginFunctionArgs.push(arguments[i]);
        }

        /* call function */
        _globals.plugins[cqArrowQuestion][pluginFunctionName].apply(null, pluginFunctionArgs);
      },
      /**
       * Checks dependencies based on the _globals.dependencies object
       * @returns {boolean}
       */
      "checkDependencies": function() {
        var dependenciesPresent = true;
        for (var libName in _globals.dependencies) {
          var callback = _globals.dependencies[libName];
          if (callback.call() === false) {
            console.error('jquery.cqArrowQuestion: Library ' + libName + ' not found! This may give unexpected results or errors.')
            dependenciesPresent = false;
          }
        }

        return dependenciesPresent;
      }
    };

    /* this object holds all global variables */
    var _globals = {};

    /* handle settings */
    _globals.settings = {};

    var defaultSettings = {
      "lineColor": "#dd4c00", /* line color */
      "lineSelectedColor": "#DBBA00", /* selection color */
      "showHotspots": true, /* whether to draw hotspots or not */
      "hotspotColor": "#ccc", /* hotspot border color */
      "showHotspotLabels": false, /* draw hotspot labels or not */
      "lineStyle": "straight",
      "endArrow": true,
      "startArrow": true,
      "lineNumbering": true
    };

    if ($.isPlainObject(settings) === true) {
      _globals.settings = $.extend(true, {}, defaultSettings, settings);
    }
    else {
      _globals.settings = defaultSettings;
    }

    /* this object contains a number of functions to test for dependencies,
     * functies should return TRUE if the library/browser/etc is present
     */
    _globals.dependencies = {
      /* check for jQuery 1.6+ to be present */
      "jquery1.5+": function() {
        var jqv, jqv_main, jqv_sub;
        if (window.jQuery) {
          jqv = jQuery().jquery.split('.');
          jqv_main = parseInt(jqv[0], 10);
          jqv_sub = parseInt(jqv[1], 10);
          if (jqv_main > 1 || (jqv_main === 1 && jqv_sub >= 5)) {
            return true;
          }
          else {
            return false;
          }
        }
      },
      "jCanvas (http://calebevans.me/projects/jcanvas)": function() {
        return (typeof $.jCanvas !== "undefined");
      }
    };
    _helper.checkDependencies();

    //this object holds all plugins
    _globals.plugins = {};


    /* register globals
     * jQuerified elements start with $
     */

    /* the main element, originally the image, replaced by canvas during init */
    _globals.$element = $(element);
    _globals.$canvasElement = undefined;

    /* the canvas context */
    _globals.context = undefined;

    /* the image as a Javascript Image object */
    _globals.imageObj = undefined;

    /* the dimensions of the image object */
    _globals.imageWidth = undefined;
    _globals.imageHeight = undefined;

    /* the mouse start position */
    _globals.startPosition = undefined;

    /* inversion of the line */
    _globals.isCurveClockwise = 1;

    /* selected answer */
    _globals.selectedLine = undefined;

    /* answer index */
    _globals.answerIndex = 1;

    /* number of hotspots */
    _globals.hotspotsAsArray = [];

    /**
     * Init function
     **/
    publicObj.init = function() {
      var $canvas;
      console.log(_globals.settings);
      /* wait until image loads */
      $(_globals.$element).one('load', function() {
        /* turn image into jCanvas
         */
        _globals.imageWidth = _globals.$element.width();
        _globals.imageHeight = _globals.$element.height();

        /* replace image with canvas html element */
        $canvas = $('<canvas class="cqCanvas" />');
        $canvas.attr('width', _globals.imageWidth);
        $canvas.attr('height', _globals.imageHeight);
        _globals.$element.after($canvas);

        //add clone of image to canvas
        $canvas.addLayer({
          "name": "image",
          "type": "image",
          "source": _globals.$element.attr('src'),
          "x": 0,
          "y": 0,
          "width": _globals.imageWidth,
          "height": _globals.imageHeight,
          "fromCenter": false
        }).drawLayers();

        //replace image
        _globals.$element.hide();
        _globals.$canvasElement = $canvas;

        /* attach event handlers to canvas */
        _globals.$canvasElement.on('mousedown', onCanvasMouseDown);
        _globals.$canvasElement.on('mouseup', onCanvasMouseUp);
        _globals.$canvasElement.on('mousemove', onCanvasMouseMove);

        /* draw hotspots */
        createHotspots();

        /* capture delete button */
        $('html').keyup(function(e) {
          if (e.keyCode === 46)
            onDeleteKeyUp(e);
        });

        _globals.$element.trigger('cqArrowQuestion.init', publicObj);
      }).each(function() {
        /* http://stackoverflow.com/questions/3877027/jquery-callback-on-image-load-even-when-the-image-is-cached */
        if (this.complete)
          $(this).load();
      });
    };

    /**
     * Returns answer
     * @return string A comma separated list of hotspot ids, e.g. "ab,cd,ef"
     */
    publicObj.getAnswer = function() {
      var answer = [];
      var answerLayers = _globals.$canvasElement.getLayerGroup('answers');
      if (answerLayers !== undefined) {
        $.each(answerLayers, function(i, layer) {
          var isCurveClockwiseAppendix = '';
          switch (layer.type) {
            case 'quadratic':
            case 'line':
              isCurveClockwiseAppendix = layer.isCurveClockwise === 1 ? '*' : ''; //add character to remember isCurveClockwise
              answer[getLayerLineIndex(layer) - 1] = layer.hotspotPair + isCurveClockwiseAppendix;

              break;
          }
        });
      }
      return answer.join(',');
    };

    /**
     * Sets answer
     * @param {string} answerAsString A comma separated list of hotspot ids, e.g. "ab,cd,ef"
     */
    publicObj.setAnswer = function(answerAsString) {
      /* clear current answer */
      clearAnswer();

      if (answerAsString === '')
        return;

      /* add new answer */
      var answerAsArray = answerAsString.split(',');

      $.each(answerAsArray, function(answerIndex, hotspotPair) {
        var startPosition = getHotspotPosition(hotspotPair[0]);
        var endPosition = getHotspotPosition(hotspotPair[1]);
        var isCurveClockwise = typeof hotspotPair[2] === 'undefined' ? -1 : 1;
        drawAnswerLine(startPosition.x, startPosition.y, endPosition.x, endPosition.y, {
          "doRefreshCanvas": false,
          "isCurveClockwise": isCurveClockwise
        });
        addLineToAnswer(hotspotPair.substr(0, 2));
      });

      refreshCanvas();
    };

    /**
     * Registers a plugin
     * @param {string} name Name of plugin, must be unique
     * @param {object} object An object {("functions": {},) (, "hooks: {}) (, "targetcqArrows": [])}
     */
    publicObj.registerPlugin = function(name, object) {
      _helper.registerPlugin(name, object);
    };

    /**
     * Calls a plugin function, all additional arguments will be passed on
     * @param {string} cqArrowQuestion
     * @param {string} pluginFunctionName
     */
    publicObj.callPluginFunction = function(cqArrowQuestion, pluginFunctionName) {
      /* call function */
      _helper.callPluginFunction.apply(null, arguments);
    };

    /**
     * Called when use triggers mousemove event on canvas element
     * @param {object} e The event object
     **/
    function onCanvasMouseMove(e) {
      _helper.doHook('onBeforeCanvasMouseMove', e);
      var startPosition = getStartPosition();
      if (startPosition === undefined) {
        return;
      }

      var isCurveClockwise = e.ctrlKey === false ? -1 : 1;
      var currentPosition = getCurrentPosition(e);

      /* remove previous temp line */
      _globals.$canvasElement.removeLayerGroup('tempLine');

      /* draw temp line */
      drawAnswerLine(startPosition.x, startPosition.y, currentPosition.x, currentPosition.y, {
        "isCurveClockwise": isCurveClockwise
      });

      //call hook
      _helper.doHook('onCanvasMouseMove', e);
    }

    /**
     * Creates (and optionally, draws) hotspots
     */
    function createHotspots() {
      $.each(_globals.settings.hotspots, function(hotspotId, hotspotSettings) {
        var coords = hotspotSettings.coords.split(',');
        $.each(coords, function(i, coord) {
          coords[i] = parseInt(coord, 10);
        });
        var shape = hotspotSettings.shape;

        switch (shape) {
          case 'rect':
            /* remember hotspot specs so we can later find out if cursor is hovering */
            _globals.hotspotsAsArray.push([coords[0], coords[1], coords[2], coords[3], hotspotId]);

            /* draw hotspot */
            _globals.$canvasElement.addLayer({
              "type": "rectangle",
              "name": "hotspot_" + hotspotId,
              "groups": ["hotspots"],
              "x": parseInt(coords[0], 10),
              "y": parseInt(coords[1], 10),
              "fromCenter": false,
              "width": coords[2] - coords[0],
              "height": coords[3] - coords[1],
              "strokeStyle": _globals.settings.hotspotColor,
              "visible": _globals.settings.showHotspots
            });

            /* optionally, draw hotspot id */
            if (_globals.settings.showHotspotLabels === true) {
              _globals.$canvasElement.addLayer({
                "type": "text",
                "groups": ["hotspots"],
                "strokeStyle": _globals.settings.hotspotColor,
                "strokeWidth": 1,
                "x": coords[0],
                "y": coords[1],
                "fontSize": 14,
                "fontFamily": 'Verdana, sans-serif',
                "text": hotspotId
              });
            }

            break;

          default:
            console.warning('The arrow question currently only supports rectangle hotspots');
            break;

        }
      });
    }

    /**
     * Calculates the postion of the line
     * @param {type} x1
     * @param {type} y1
     * @param {type} x2
     * @param {type} y2
     * @param {type} options
     * @returns {object}
     */
    function getAnswerLineConfig(x1, y1, x2, y2, options) {
      var answerLineConfig = {
        "x1": x1,
        "y1": y1,
        "x2": x2,
        "y2": y2,
        "name": options.name === undefined ? "line_" + _globals.answerIndex : options.name,
        "groups": options.groups === undefined ? ["tempLine", "lines"] : options.groups,
        "endArrow": _globals.settings.endArrow,
        "startArrow": _globals.settings.startArrow,
        "arrowRadius": 8,
        "arrowAngle": 90,
        "strokeWidth": 2,
        "rounded": true,
        "strokeStyle": options.lineColor === undefined ? _globals.settings.lineColor : options.lineColor,
        "lineStyle": 1
      };

      var tempDx, tempDy, tempLength, tempHeight, tempAlpha, tempX1, tempY1;

      switch (_globals.settings.lineStyle) {
        case 'curved':
          /* set options */
          answerLineConfig.type = "quadratic";
          answerLineConfig.isCurveClockwise = options.isCurveClockwise === undefined ? -1 : options.isCurveClockwise;

          /* do calculations */
          tempDx = x2 - x1;
          tempDy = y1 - y2;
          tempLength = Math.sqrt(tempDx * tempDx + tempDy * tempDy);
          tempHeight = answerLineConfig.isCurveClockwise * tempLength / 2;
          tempAlpha = Math.atan(tempDy / tempDx);


          tempX1 = x1 + (x2 < x1 ? -1 : 1) * Math.cos(tempAlpha) * tempLength / 2;
          tempY1 = y1 - (x2 < x1 ? -1 : 1) * Math.sin(tempAlpha) * tempLength / 2;

          answerLineConfig.cx1 = tempX1 + Math.sin(tempAlpha) * tempHeight;
          answerLineConfig.cy1 = tempY1 + Math.cos(tempAlpha) * tempHeight;

          answerLineConfig.xText = tempX1 + Math.sin(tempAlpha) * tempHeight / 2;
          answerLineConfig.yText = tempY1 + Math.cos(tempAlpha) * tempHeight / 2;

          answerLineConfig.xDelete = answerLineConfig.xText;
          answerLineConfig.yDelete = answerLineConfig.yText;
          break;

        case 'straight':
          /* set options */
          answerLineConfig.type = "line";

          /* do calculations */
          answerLineConfig.xText = (x2 + x1) / 2;
          answerLineConfig.yText = (y1 + y2) / 2;

          answerLineConfig.xDelete = answerLineConfig.xText;
          answerLineConfig.yDelete = answerLineConfig.yText;
          break;
      }

      return answerLineConfig;
    }



    /**
     * Draws an answer on the canvas
     * @param {integer} x1
     * @param {integer} y1
     * @param {integer} x2
     * @param {integer} y2
     * @param {object} options Additional options {
     *                                              "isCurveClockwise": 1 or -1 (default: -1),
     *                                              "doRefreshCanvas": boolean (default: true),
     *                                              "lineColor": string (default _globals.settings.lineColor)
     *                                            }
     */
    function drawAnswerLine(x1, y1, x2, y2, options) {
      var answerLineConfig = getAnswerLineConfig(x1, y1, x2, y2, options);

      /* draw line */
      _globals.$canvasElement.addLayer(answerLineConfig);

      /* draw text */
      _globals.$canvasElement.addLayer({
        "type": "ellipse",
        "name": "circle_" + _globals.answerIndex,
        "groups": ["tempLine", "texts"],
        fillStyle: answerLineConfig.strokeStyle,
        strokeStyle: answerLineConfig.strokeStyle,
        x: answerLineConfig.xText, y: answerLineConfig.yText,
        "width": 14,
        "height": 14,
        "visible": _globals.settings.lineNumbering
      });

      _globals.$canvasElement.addLayer({
        "type": "text",
        "name": "text_" + _globals.answerIndex,
        "groups": ["tempLine", "texts"],
        fillStyle: "#fff",
        strokeStyle: "#fff",
        strokeWidth: 1,
        x: answerLineConfig.xText, y: answerLineConfig.yText,
        fontSize: 10,
        fontFamily: 'Verdana, sans-serif',
        text: _globals.answerIndex.toString(),
        "visible": _globals.settings.lineNumbering
      });

      /* draw delete button */
      _globals.$canvasElement.addLayer({
        "type": "ellipse",
        "name": "deleteCircle_" + _globals.answerIndex,
        "groups": ["tempLine", "ui"],
        fillStyle: '#f00',
        strokeStyle: '#f00',
        strokeWidth: 2,
        x: answerLineConfig.xDelete, y: answerLineConfig.yDelete,
        "width": 14, "height": 14,
        fontSize: 10,
        fontFamily: 'Verdana, sans-serif',
        text: 'x',
        "visible": false
      });

      //x
      _globals.$canvasElement.addLayer({
        "type": "text",
        "name": "deleteText_" + _globals.answerIndex,
        "groups": ["tempLine", "ui"],
        fillStyle: '#fff',
        strokeStyle: '#fff',
        strokeWidth: 1,
        x: answerLineConfig.xDelete, y: answerLineConfig.yDelete,
        fontSize: 10,
        fontFamily: 'Verdana, sans-serif',
        text: 'x',
        'visible': false
      });

      if (options.refreshCanvas !== false) {
        refreshCanvas();
      }
    }

    /**
     * Adds answer unit to answer
     * @param {string} hotspotPair The ids of the hotspots, e.g. 'ab'
     */
    function addLineToAnswer(hotspotPair) {
      /* add unit to specific groups */
      _globals.$canvasElement.setLayer("line_" + _globals.answerIndex, {
        "groups": ["answers", "lines", _globals.answerIndex],
        "hotspotPair": hotspotPair
      });

      _globals.$canvasElement.setLayer("text_" + _globals.answerIndex, {
        "groups": ["answers", "texts", _globals.answerIndex]
      });

      _globals.$canvasElement.setLayer("circle_" + _globals.answerIndex, {
        "groups": ["answers", "texts", _globals.answerIndex]
      });

      _globals.$canvasElement.setLayer("deleteText_" + _globals.answerIndex, {
        "groups": ["answers", "ui", _globals.answerIndex]
      });

      _globals.$canvasElement.setLayer("deleteCircle_" + _globals.answerIndex, {
        "groups": ["answers", "ui", _globals.answerIndex]
      });

      /* add event handlers */
      _globals.$canvasElement.setLayerGroup(_globals.answerIndex, {
        "mouseover": onLineMouseOver,
        "mouseout": onLineMouseOut,
        "click": onLineMouseClick
      });

      /* set and update selected answer index  */
      setSelectedLine(_globals.answerIndex);
      _globals.answerIndex++;

      /* call hook */
      _helper.doHook('onAddLineToAnswer', _globals.answerIndex, hotspotPair);
    }

    /**
     * Deletes an answer unit
     * @param {index} answerUnitIndex
     */
    function removeLineFromAnswer(answerUnitIndex) {
      if (answerUnitIndex < 1) {
        return;
      }

      var index, layerNames, answerUnitLayers;

      /* remove visual representation */
      _globals.$canvasElement.removeLayerGroup(answerUnitIndex);

      /* renumber layers with higher index */
      for (index = answerUnitIndex + 1; index < _globals.answerIndex; index++) {
        answerUnitLayers = _globals.$canvasElement.getLayerGroup(index);
        layerNames = [];

        /* we want to iterate of names (and not on answerUnitLayers, which will
         * shrink as we remove layers */
        $.each(answerUnitLayers, function(i, layer) {
          layerNames.push(layer.name);
        });

        $.each(layerNames, function(i, layerName) {
          /* remove layer from group with index and add it to group with index-1 */
          _globals.$canvasElement.removeLayerFromGroup(layerName, index);
          _globals.$canvasElement.addLayerToGroup(layerName, index - 1);

          /* change text layer's caption */
          if (layerName.indexOf('text_') !== -1) {
            //change number
            _globals.$canvasElement.setLayer("text_" + index, {
              "text": index - 1
            });
          }

          /* change layer name */
          _globals.$canvasElement.setLayer(layerName, {
            "name": layerName.substring(0, layerName.indexOf('_')) + '_' + (index - 1)
          });
        });
      }

      /* lower layer counter */
      _globals.answerIndex--;
      setSelectedLine(answerUnitIndex - 1);

      /* refresh canvas and trigger event */
      refreshCanvas();
      _helper.doHook('onRemoveLineFromAnswer', _globals.answerIndex, answerUnitIndex);
    }

    /**
     * Called when use triggers mousedown event on canvas element
     * @param {object} e The event object
     **/
    function onCanvasMouseDown(e) {
      var currentPosition = getCurrentPosition(e);

      if (currentPosition.inHotspotKey !== undefined) {
        setStartPosition(currentPosition);
      }
    }

    /**
     * Called when use triggers mouseup event on canvas element
     * @param {object} e The event object
     **/
    function onCanvasMouseUp(e) {
      var currentPosition = getCurrentPosition(e);

      /* decide what to do: turn temp answer into answer or not */
      if (currentPosition.inHotspotKey !== undefined) {
        /* turn temp answer into answer */
        if (getStartPosition().inHotspotKey !== currentPosition.inHotspotKey) {
          addLineToAnswer(getStartPosition().inHotspotKey + currentPosition.inHotspotKey);
        }
      }
      else {
        /* clear temp answer */
        _globals.$canvasElement.removeLayerGroup('tempLine');
      }

      /* reset line start position */
      setStartPosition(undefined);
    }

    /**
     * Called when user hovers line
     * @param {object} layer The line jDraw layer object
     */
    function onLineMouseOver(layer) {
      var answerUnitIndex = getLayerLineIndex(layer);

      _globals.$canvasElement.setLayer('line_' + answerUnitIndex, {
        "strokeStyle": _globals.settings.lineSelectedColor
      });

      _globals.$canvasElement.setLayer('deleteText_' + answerUnitIndex, {
        "visible": true
      });

      _globals.$canvasElement.setLayer('deleteCircle_' + answerUnitIndex, {
        "visible": true
      });
    }

    /**
     * Called when user unhovers line
     * @param {object} layer The line jDraw layer object
     */
    function onLineMouseOut(layer) {
      var answerUnitIndex = getLayerLineIndex(layer);

      _globals.$canvasElement.setLayer('line_' + answerUnitIndex, {
        "strokeStyle": _globals.settings.lineColor
      });

      _globals.$canvasElement.setLayer('deleteText_' + answerUnitIndex, {
        "visible": false
      });

      _globals.$canvasElement.setLayer('deleteCircle_' + answerUnitIndex, {
        "visible": false
      });
    }

    /**
     * Called when user clicks answer unit
     * @param {object} layer The line jDraw layer object
     */
    function onLineMouseClick(layer) {
      /* select answer */
      setSelectedLine(getLayerLineIndex(layer));

      /* look if event on UI layer group should be triggered */
      if (layer.groups.indexOf('ui') >= 0) {
        onLineUIMouseClick(layer);
        return;
      }
    }

    /**
     * Called when user clicks on UI element of answer unit
     * @param {object} layer The line jDraw layer object
     */
    function onLineUIMouseClick(layer) {
      var answerIndex = getLayerLineIndex(layer);

      /* currently only delete button in UI */
      removeLineFromAnswer(answerIndex);
    }

    /**
     * Called when user pressed delete button
     * @param {event} e
     * @todo Implement
     */
    function onDeleteKeyUp(e) {
      /* currently only delete button in UI */
      removeLineFromAnswer(getSelectedLineIndex());
    }

    /**
     * Sets the selected answer
     * @param {integer} index
     */
    function setSelectedLine(index) {
      _globals.selectedLineIndex = index;

      /* change appearance */

      //reset all elements
      _globals.$canvasElement.setLayerGroup('lines', {
        "strokeStyle": _globals.settings.lineColor
      });

//      _globals.$canvasElement.setLayerGroup('texts', {
//        "strokeStyle": _globals.settings.lineColor,
//        "fillStyle": _globals.settings.lineColor
//      });

      _globals.$canvasElement.setLayerGroup('ui', {
        "visible": false
      });

      _globals.$canvasElement.setLayerGroup('ui', {
        "visible": false
      });

      //set selected layer
      _globals.$canvasElement.setLayer('line_' + index, {
        "strokeStyle": _globals.settings.lineSelectedColor
      });

      _globals.$canvasElement.setLayer('text_' + index, {
        fillStyle: "#fff",
        strokeStyle: "#fff"
      });
//
//      _globals.$canvasElement.setLayer('text_' + index, {
//        "strokeStyle": _globals.settings.lineSelectedColor,
//        "fillStyle": _globals.settings.lineSelectedColor
//      });



      refreshCanvas();
    }

    /**
     * Returns the selected answer
     * @return integer
     */
    function getSelectedLineIndex() {
      return _globals.selectedLineIndex;
    }

    /**
     * Returns a layer's answer index
     * @param {object} layer
     * @returns {integer}
     */
    function getLayerLineIndex(layer) {
      return parseInt(layer.name.split('_')[1]);
    }

    /**
     * Returns the center coordinates of a hotspot
     * @param {string} hotspotId
     * @return object {"x": int, "y": int}
     */
    function getHotspotPosition(hotspotId) {
      var hotspot = _globals.$canvasElement.getLayer("hotspot_" + hotspotId);

      if (hotspot) {
        return {"x": hotspot.x + hotspot.width / 2, "y": hotspot.y + hotspot.height / 2}
      }
      return null;
    }

    /**
     * Returns end position of line. This can be the mouse position, but also
     *  a 'snapped' position.
     * @param {object} e Mouse event object
     * @return {object} {"x": int, "y": int, "inHotspot": boolean}
     */
    function getCurrentPosition(e) {
      var position = {"x": undefined, "y": undefined, "inHotspotKey": undefined};
      var mouseX = e.offsetX;
      var mouseY = e.offsetY;
      var i, hotspotCoords;

      /* check if mouse inside hotspot
       * @todo use hotspot mouseover/out for this, currently event not falling
       * through because of line being drawn
       */
      for (i = _globals.hotspotsAsArray.length - 1; i >= 0; i--) {
        hotspotCoords = _globals.hotspotsAsArray[i];

        if (mouseX >= hotspotCoords[0] && mouseX <= hotspotCoords[2]) {
          if (mouseY >= hotspotCoords[1] && mouseY <= hotspotCoords[3]) {
            position.x = hotspotCoords[0] + (hotspotCoords[2] - hotspotCoords[0]) / 2;
            position.y = hotspotCoords[1] + (hotspotCoords[3] - hotspotCoords[1]) / 2;
            position.inHotspotKey = hotspotCoords[4];
            break;
          }
        }
      }

      if (position.inHotspotKey === undefined) {
        position.x = mouseX;
        position.y = mouseY;
      }
      return position;
    }

    /**
     * Saves line start position
     * @param {object} position {"x": int, "y": int, "inHotspotKey": string}
     */
    function setStartPosition(position) {
      _globals.startPosition = position;
    }

    /**
     * Returns line start position
     * @returns {object} See setStartPosition
     */
    function getStartPosition() {
      return _globals.startPosition;
    }

    /**
     * Redraws the canvas
     */
    function refreshCanvas() {
      _globals.$canvasElement.drawLayers();
    }

    /**
     * Clears the canvas
     */
    function clearAnswer() {
      _globals.$canvasElement.removeLayerGroup("answers");
      _globals.answerIndex = 1;
      refreshCanvas();
    }

    /* initialize cqArrowQuestion
     */
    $(element).trigger('cqArrowQuestion.beforeInit', publicObj, element, settings); //trigger event on document
    publicObj.init();
  };

  $.fn.cqArrowQuestion = function(settings) {
    return this.each(function() {
      if (undefined === $(this).data('cqArrowQuestion')) {
        var plugin = new $.cqArrowQuestion(this, settings);
        $(this).data('cqArrowQuestion', plugin);
      }
    });
  };

})(jQuery);