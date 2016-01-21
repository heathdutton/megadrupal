<?php

/**
 * @file
 * Classes used for drawing a HTML5 canvases.
 */

/**
 * A stub class used for creating a HTML canvas.
 *
 * This class isn't really used at the moment, save for extending for other
 * classes.
 */
class TutorCanvas {
  /**
   * The HTML element ID for the canvas.
   *
   * @var string
   */
  var $id;

  /**
   * Set this to FALSE if you don't want the JS for this canvas to load.
   *
   * @var boolean
   */
  var $active = TRUE;
  // The width and height of the canvas, in pixels. Set on construct.
  var $width;
  var $height;

  /**
   * Coordinate settings for the canvas.
   *
   * This array has two keys, 'x' and 'y'. Each of these has three values:
   * - min: The starting value on this axis, i.e. top/left. Default: -10
   * - max: The ending value on this axis, i.e. bottom/right. Default: 10
   * - step: How far spaced any grid should be on this axis. Default: 1
   *
   * @var array
   */
  var $window = array(
    'x' => array(
      'min' => -10,
      'max' => 10,
      'step' => 1,
    ),
    'y' => array(
      'min' => -10,
      'max' => 10,
      'step' => 1,
    ),
  );

  /**
   * Settings to pass to the JavaScript.
   *
   * @var array
   */
  private $settings = array();

  public function __construct($id, $width = 300, $height = 150) {
    $this->id = $id;
    $this->width = $width;
    $this->height = $height;
  }

  function getMarkup() {
    $sorry_message = t("Your browser doesn't seem to support canvas. There should be a drawing here. Sorry.");
    return '<canvas id="' . $this->id . '" height="' . $this->height . '" width="' . $this->width . '" >' . $sorry_message . '</canvas>';
  }

  /**
   * Processes and sends off data to the JavaScript.
   *
   * This processing is delayed until the destruct phase of the object, to allow
   * window settings to be changed without having to recalculate coordinates.
   */
  public function __destruct() {
    if (!$this->active) {
      return;
    }
    // JavaScript inclusion should be done here, which means it is only called
    // if the canvas is active.
    $this->settings['id'] = $this->id;
  }
}

/**
 * A class used for drawing coordinate systems and graphs on a HTML5 canvas.
 */
class TutorCanvasCoordinatePlane extends TutorCanvas {
  /**
   * Settings to pass to the JavaScript.
   *
   * @var array
   */
  private $settings = array(
    'id' => NULL, // Set on construct.
    'drawGrid' => TRUE,
    'drawScale' => TRUE,
    'dataSets' => array(), // Set when data sets are added, processed on destruct.
    'grid' => array(), // Set on destruct.
  );

  /**
   * Adds a data set that should be drawn in the canvas graph.
   *
   * @param $type
   *   The type of plot to use. Valid options are 'curve' and 'points', which
   *   results in a curve of connected dots and stand-alone dots, respectively.
   * @param $x
   *   An array with the x values for the points in the graph, in the coordinate
   *   system defined by the window settings.
   * @param array $y
   *   An array with the y values for the points in the graph.
   * @param $options
   *   An array with any extra options to use in the graph. Valied keys are:
   *   - color: The value of this entry will be used as color for the data set.
   *     Defaults to 'blue'.
   *   - draw coordinates: Set to TRUE if coordinates should be plotted next to
   *     individual dots. Only valid for the 'points' type.
   * @return
   *   The TutorCanvas object, to allow chaining, or FALSE if the number of x
   *   and y coordinates don't match.
   */
  public function addDataSet($type, array $x, array $y, $options = array()) {
    if (count($x) != count($y)) {
      return FALSE;
    }
    $settings = array(
      'x' => $x,
      'y' => $y,
      'type' => $type,
      'color' => 'blue',
    );

    // Take care of extra options, if any.
    if (isset($options['color'])) {
      $settings['color'] = $options['color'];
    }
    if ($type == 'points' && isset($options['draw coordinates']) && $options['draw coordinates']) {
      $settings['drawCoordinates'] = TRUE;
    }

    $this->settings['dataSets'][] = $settings;

    // Return the object, so it can be chained with new methods.
    return $this;
  }

  /**
   * Turn on or off the drawing of background grid. Defaults to TRUE.
   *
   * @param $value
   *   TRUE if the grid should be drawn, otherwise FALSE.
   */
  function drawGrid($value) {
    $this->settings[__FUNCTION__] = $value;
    return $this;
  }

  /**
   * Turn on or off the drawing of scale markers. Defaults to TRUE.
   *
   * The scale markers are only drawn if a grid is drawn.
   *
   * @param $value
   *   TRUE if scale markers should be drawn, otherwise FALSE.
   */
  function drawScale($value) {
    $this->settings[__FUNCTION__] = $value;
    return $this;
  }

  /**
   * Processes and sends off data to the JavaScript.
   *
   * This processing is delayed until the destruct phase of the object, to allow
   * window settings to be changed without having to recalculate coordinates.
   */
  public function __destruct() {
    if (!$this->active) {
      return;
    }
    $this->settings['id'] = $this->id;

    // Create two aliases to make code readable.
    $window = &$this->window;
    $settings = &$this->settings;

    // Make sure that noone has messed up the window settings.
    foreach (array('x', 'y') as $axis) {
      if (!isset($window[$axis])) {
        return;
      }
      if (!isset($window[$axis]['min']) || !isset($window[$axis]['max']) || $window[$axis]['min'] >= $window[$axis]['max']) {
        return;
      }
      if (!isset($window[$axis]['step']) || $window[$axis]['step'] <= 0) {
        return;
      }
    }

    // All seems fine – process the settings and send to the JavaScript.
    $settings['origin'] = array('x' => $this->convertX(0), 'y' => $this->convertY(0));
    $settings['grid']['x'] = $window['x']['step'] / ($window['x']['max'] - $window['x']['min']) * $this->width;
    $settings['grid']['xOriginal'] = $window['x']['step'];
    $settings['grid']['y'] = $window['y']['step'] / ($window['y']['max'] - $window['y']['min']) * $this->height;
    $settings['grid']['yOriginal'] = $window['y']['step'];

    foreach ($settings['dataSets'] as &$data_set) {
      // Store away the original coordinates if they are to be printed out.
      if (isset($data_set['drawCoordinates'])) {
        $data_set['xOriginal'] = $data_set['x'];
        $data_set['yOriginal'] = $data_set['y'];
      }
      // Convert all coordinates to pixels coordinates in the canvas.
      foreach ($data_set['x'] as $key => $value) {
        $data_set['x'][$key] = $this->convertX($data_set['x'][$key]);
        $data_set['y'][$key] = $this->convertY($data_set['y'][$key]);

        // Make sure we don't have any really large numbers. JS don't like them.
        if ($data_set['x'][$key] > 3 * $this->width) {
          $data_set['x'][$key] = 3 * $this->width;
        }
        if ($data_set['y'][$key] > 3 * $this->height) {
          $data_set['y'][$key] = 3 * $this->width;
        }
        if ($data_set['x'][$key] < -100) {
          $data_set['x'][$key] = -100;
        }
        if ($data_set['y'][$key] < -100) {
          $data_set['y'][$key] = -100;
        }
      }
    }

    // Send the settings to the JavaScript handler.
    drupal_add_js(array('tutorCanvas' => array('graphData' => array($settings))), 'setting');

    // Add the Tutor canvas JavaScript file, to take care of the settings.
    $path = drupal_get_path('module', 'tutor_canvas');
    drupal_add_js($path . '/tutor_canvas_coordinate_plane.js', array('defer' => TRUE));
  }

  /**
   * Converts a window coordinate to canvas pixel coordinate, for x values.
   *
   * @param $x
   *   The x coordinate, in the window values.
   * @return
   *   The x coordinate, in canvas pixel values.
   */
  private function convertX($x) {
    return ($x - $this->window['x']['min']) / ($this->window['x']['max'] - $this->window['x']['min']) * $this->width;
  }

  /**
   * Converts a window coordinate to canvas pixel coordinate, for y values.
   *
   * (This is a separate function from convertY since the y coordinates are
   * flipped between the two reference frames. It just didn't make sense to
   * abstract this out for these two cases only.)
   *
   * @param $y
   *   The y coordinate, in the window values.
   * @return
   *   The y coordinate, in canvas pixel values.
   */
  private function convertY($y) {
    return ($y - $this->window['y']['min']) / ($this->window['y']['max'] - $this->window['y']['min']) * $this->height * -1 + $this->height;
  }
}
