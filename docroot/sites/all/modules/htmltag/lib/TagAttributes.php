<?php


class htmltag_TagAttributes {

  protected $attributes = array();
  protected $classes = array();

  function setAttribute($key, $value, $raw = TRUE) {
    $this->attributes[$key] = $raw ? $value : check_plain($value);
  }

  function unsetAttribute($key) {
    unset($this->attributes[$key]);
  }

  function setAttributes(array $values, $raw = TRUE) {
    foreach ($values as $key => $value) {
      if ($key === 'class') {
        $this->classes = array();
        $this->addClasses($value);
      }
      else {
        $this->attributes[$key] = $raw ? $value : check_plain($value);
      }
    }
  }

  function mergeAttributes(array $values, $raw = TRUE) {
    foreach ($values as $key => $value) {
      if ($key === 'class') {
        $this->addClasses($value);
      }
      elseif (!isset($this->attributes[$key])) {
        $this->attributes[$key] = $raw ? $value : check_plain($value);
      }
    }
  }

  function addClass($class) {
    $this->classes[$class] = $class;
  }

  function addClassIf($cond, $class) {
    if ($cond) {
      $this->classes[$class] = $class;
    }
  }

  function addClasses($classes) {
    if (!is_array($classes)) {
      $classes = explode(' ', $classes);
    }
    foreach ($classes as $class) {
      if ($class) {
        $this->classes[$class] = $class;
      }
    }
  }

  function addClassesIf(array $classes_if) {
    foreach ($classes_if as $class => $cond) {
      if ($cond) {
        $this->classes[$class] = $class;
      }
    }
  }

  function hasClass($class) {
    return isset($this->classes[$class]);
  }

  function removeClass($class) {
    unset($this->classes[$class]);
  }

  function getAttributes() {
    if (!empty($this->classes)) {
      $this->attributes['class'] = implode(' ', $this->classes);
    }
    else {
      unset($this->attributes['class']);
    }
    return $this->attributes;
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
