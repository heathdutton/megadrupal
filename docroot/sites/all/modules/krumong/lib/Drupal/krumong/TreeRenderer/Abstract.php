<?php

namespace Drupal\krumong;


abstract class TreeRenderer_Abstract {

  protected $theme;

  protected $hiveObjects = array();
  protected $hiveArrays = array();
  protected $trailOfKeys = array();
  protected $hiveMarkerKey;

  /**
   * @param TreeTheme_Interface $theme
   *   "Theme" to use for the rendering.
   */
  function __construct($theme) {
    $this->theme = $theme;
  }

  /**
   * Render a full tree of data.
   *
   * @param mixed $data
   *   Any value, e.g. nested array etc.
   *
   * @return string
   *   Rendered and "wrapped" output.
   */
  function render(&$data, $call, $name = NULL) {
    $this->hiveInit($data);
    $result = $this->renderData($data);
    $result = $this->theme->wrap($result, $call, $name);
    $this->hiveClean();
    return $result;
  }

  /**
   * Render a value and everything that's nested below.
   *
   * @param mixed $data
   *   Any value, e.g. nested array etc.
   *
   * @return string
   *   Rendered output.
   */
  protected function renderData(&$data) {

    $info = $this->hiveGetElementInfo($data);

    if (!isset($info)) {
      return $this->theme->renderValue($data, $this->trailOfKeys);
    }
    elseif ($this->hiveDetectRecursion($data, $info)) {
      if (!is_array($info) || !is_array($info['trail'])) {
        throw new \Exception('$info["trail"] must be array.');
      }
      // Stop recursion.
      return $this->theme->renderRecursion($data, $this->trailOfKeys, $info['trail']);
    }
    else {
      // Process children.
      $children = $this->renderChildren($data);
      if (is_object($data)) {
        $class = get_class($data);
        return $this->theme->renderObject($data, $this->trailOfKeys, $children, $class);
      }
      else {
        return $this->theme->renderArray($data, $this->trailOfKeys, $children);
      }
    }
  }

  /**
   * Render all array values or object attribute values, and return them as an
   * associative array.
   *
   * @param array|object $data
   *   An array or object, the values or attributes of which we are interested
   *   in.
   *
   * @return array
   *   Rendered output, one string for each array value / object attribute.
   */
  protected function renderChildren(&$data) {
    $children = array();
    foreach ($data as $k => &$v) {
      if ($k === $this->hiveMarkerKey) {
        continue;
      }
      $this->trailOfKeys[] = $k;
      $children[$k] = $this->renderData($v);
      array_pop($this->trailOfKeys);
    }
    return $children;
  }

  // Hive management
  // ---------------------------------------------------------------------------

  abstract protected function hiveDetectRecursion($data, $info);

  protected function hiveInit(&$data) {
    $this->hiveMarkerKey = '_' . Util::randomstring(40);
  }

  protected function hiveClean() {
    foreach ($this->hiveArrays as &$arr) {
      unset($arr[$this->hiveMarkerKey]);
    }
    $this->hiveArrays = array();
    $this->hiveObjects = array();
    $this->trailOfKeys = array();
  }

  protected function hiveGetElementInfo(&$data) {

    if (is_object($data)) {
      return $this->hiveGetObjectInfo($data);
    }
    elseif (is_array($data)) {
      return $this->hiveGetArrayInfo($data);
    }

    return NULL;
  }

  protected function hiveAddElement(&$data) {

    if (is_object($data)) {
      $this->hiveAddObject($data);
    }
    elseif (is_array($data)) {
      $this->hiveAddArray($data);
    }
  }

  // Object hive management
  // ---------------------------------------------------------------------------

  protected function hiveGetObjectInfo($obj) {

    $class = get_class($obj);
    if (isset($this->hiveObjects[$class])) {
      foreach ($this->hiveObjects[$class] as &$existing) {
        if ($obj === $existing[0]) {
          return array('trail' => &$existing[1]);
        }
      }
    }

    return FALSE;
  }

  protected function hiveAddObject($obj) {

    $class = get_class($obj);
    $this->hiveObjects[$class][] = array($obj, $this->trailOfKeys);
  }

  // Array hive management
  // ---------------------------------------------------------------------------

  protected function hiveGetArrayInfo(&$arr) {

    if (isset($arr[$this->hiveMarkerKey])) {
      // The array is already visited.
      return array('trail' => &$arr[$this->hiveMarkerKey]);
    }

    return FALSE;
  }

  protected function hiveAddArray(&$arr) {
    $arr[$this->hiveMarkerKey] = $this->trailOfKeys;
    $this->hiveArrays[] =& $arr;
  }
}
