<?php

namespace Drupal\krumong;


/**
 * Render the tree as nested javascript value/object.
 * This is not a simple json_encode(), and it is not valid json, so only works
 * if run as full javascript.
 *
 * Will use "pseudo types" that show up nicely in console.log.
 * E.g. console.log will say "PHP.SomeNamespace\SubNamespace\ClassName" for the
 * type of the object.
 */
class TreeTheme_JsDataPseudoTyped {

  protected $classes = array();
  protected $recursions = array();

  function renderArray($array, $position, $children) {
    $rendered = $this->renderKeyValueData($children, '[', ']');
    return "a($rendered)";
  }

  function renderObject($object, $position, $children, $class) {
    $class = json_encode($class);
    $this->classes[$class] = TRUE;
    $rendered = $this->renderKeyValueData($children, '->', '');
    return "o($class, $rendered)";
  }

  protected function renderKeyValueData($data, $prefix, $suffix) {
    $parts = array();
    foreach ($data as $k => $v) {
      $parts[] = '  ' . json_encode($prefix . $k . $suffix) . ': ' . $v;
    }
    return "{\n" . implode(",\n", $parts) . "\n}";
  }

  function renderValue($value, $position) {
    return json_encode($value);
  }

  function renderRecursion($value, $position, $original_position) {
    $json = json_encode($original_position);
    $json_json = json_encode($json);
    $this->recursions[$json_json] = TRUE;
    return "new Recursion[$json_json]";
  }

  function wrap($string, $called_from, $name) {
    $parts = array();
    foreach ($this->classes as $class => $true) {
      $parts[] = "PHP[$class] = function(){};";
    }
    foreach ($this->recursions as $recursion => $true) {
      $parts[] = "Recursion[$recursion] = function(){};";
    }
    $parts = implode("\n", $parts);
    return <<<EOT
(function(){
  function a(data) {
    return o('[]', data);
  }
  function o(classname, data) {
    var obj = new PHP[classname];
    for (var k in data) {
      obj[k] = data[k];
    }
    return obj;
  }
  var PHP = {};
  var Recursion = {};
  PHP['[]'] = function(){};
  $parts
  return $string;
})()
EOT;
  }
}
