<?php


class dqx_adminmenu_HtmlTagAttributes {
  protected $_attributes = array();
  protected $_classes = array();

  function setAttribute($key, $value, $raw = TRUE) {
    $this->_attributes[$key] = $raw ? $value : check_plain($value);
  }

  function unsetAttribute($key) {
    unset($this->_attributes[$key]);
  }

  function setAttributes(array $values, $raw = TRUE) {
    foreach ($values as $key => $value) {
      if ($key === 'class') {
        $this->_classes = array();
        $this->addClasses($value);
      }
      else {
        $this->_attributes[$key] = $raw ? $value : check_plain($value);
      }
    }
  }

  function mergeAttributes(array $values, $raw = TRUE) {
    foreach ($values as $key => $value) {
      if ($key === 'class') {
        $this->addClasses($value);
      }
      elseif (!isset($this->_attributes[$key])) {
        $this->_attributes[$key] = $raw ? $value : check_plain($value);
      }
    }
  }

  function addClass($class) {
    $this->_classes[$class] = $class;
  }

  function addClassIf($cond, $class) {
    if ($cond) {
      $this->_classes[$class] = $class;
    }
  }

  function addClasses($classes) {
    if (!is_array($classes)) {
      $classes = explode(' ', $classes);
    }
    foreach ($classes as $class) {
      if ($class) {
        $this->_classes[$class] = $class;
      }
    }
  }

  function addClassesIf(array $classes_if) {
    foreach ($classes_if as $class => $cond) {
      if ($cond) {
        $this->_classes[$class] = $class;
      }
    }
  }

  function hasClass($class) {
    return isset($this->_classes[$class]);
  }

  function removeClass($class) {
    unset($this->_classes[$class]);
  }

  function getAttributes() {
    if (!empty($this->_classes)) {
      $this->_attributes['class'] = implode(' ', $this->_classes);
    }
    else {
      unset($this->_attributes['class']);
    }
    return $this->_attributes;
  }

  function __toString() {
    $t = '';
    foreach ($this->getAttributes() as $key => $value) {
      $t .= ' '. $key .'="'. $value .'"';
    }
    return $t;
  }

  function renderTag($tagname, $content = NULL, $raw = TRUE) {
    if (isset($content)) {
      if (!$raw) {
        $content = check_plain($content);
      }
      return '<'. $tagname . $this->__toString() .'>'. $content .'</'. $tagname .">\n";
    }
    else {
      return '<'. $tagname . $this->__toString() .' />';
    }
  }

  function __call($tagname, $args) {
    if (preg_match('/^[A-Z]+$/', $tagname)) {
      list($content, $raw, $empty_text) = $args + array(NULL, TRUE, NULL);
      $tagname = strtolower($tagname);
      if (!strlen($content) && is_string($empty_text)) {
        return $empty_text;
      }
      else {
        return $this->renderTag($tagname, $content, $raw);
      }
    }
  }
}
