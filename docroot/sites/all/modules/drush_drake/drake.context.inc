<?php

/**
 * @file
 * Defines the DrakeContext object and helper functions which is used to handle
 * task context variables.
 */

/**
 * Returns a context placeholder.
 *
 * Task definitions use this to signal that a given parameter should be taken
 * from the context. It is then later replaced by the actual value before
 * invoking the action.
 */
function context($name) {
  return new DrakeContextPlaceholder($name);
}

function context_optional($name, $default_value = NULL) {
  return new DrakeContextPlaceholder($name, TRUE, $default_value);
}

function drake_argument($index, $description) {
  return new DrakeContextArgPlaceholder($index, $description);
}


function drake_argument_optional($index, $description, $default_value = NULL) {
  return new DrakeContextArgPlaceholder($index, $description, TRUE, $default_value);
}

interface DrakePlaceholder {
  public function resolve(DrakeContext $context);
}

/**
 * Implements the basic context placeholder object.
 */
class DrakeContextPlaceholder implements DrakePlaceholder {
  protected $name;
  protected $optional;
  protected $default_value;

  /**
   * Constructs the placeholder.
   */
  public function __construct($name, $optional = FALSE, $default_value = NULL) {
    $this->name = $name;
    $this->optional = $optional;
    $this->default_value = $default_value;
  }

  /**
   * Returns the name of this placeholder.
   */
  public function name() {
    return $this->name;
  }

  /**
   * String representation of this context.
   */
  public function __toString() {
    return "[Context " . $this->name . "]";
  }

  /**
   * Resolves the placeholder to an value.
   */
  public function resolve(DrakeContext $context) {
    try {
      if (!preg_match_all('/\\[([^]]*)\\]/', $this->name, $matches, PREG_SET_ORDER)) {
        // Same structure as what preg_match_all returns.
        $matches = array(
          array(
            $this->name,
            $this->name,
          ),
        );
      }

      $values = array();
      foreach ($matches as $match) {
        $pipeline = explode(':', $match[1]);
        $value = $context[array_shift($pipeline)];
        // Special handling for recursive contexts. Try to get a parent value
        // that's not the same as the current context object.
        if ($value instanceof DrakeContextPlaceholder &&
          $value->name == $this->name) {
          $value = $context->get_not($this->name, array($this));
        }
        // In case context contains an context object.
        if ($value instanceof DrakeContextPlaceholder) {
          $value = $value->resolve($context);
        }
        foreach ($pipeline as $op) {
          $method = 'get' . ucfirst($op);
          if (is_object($value) && method_exists($value, $method)) {
            $value = $value->{$method}();
          }
          else {
            $class_name = "DrakeContextPlaceholder" . ucfirst($op) . "Op";
            if (class_exists($class_name)) {
              $value = new $class_name($value);
            }
            else {
              throw new DrakeContextException(dt('Unknown or invalid operation "@op" in context string "@context"', array('@op' => $op, '@context' => $match[1])));
            }
          }
        }
        $values[$match[0]] = (string) $value;
      }
      return strtr($this->name(), $values);
    }
    catch (DrakeContextNotFoundException $e) {
      if ($this->optional) {
        return $this->default_value;
      }
      // Else rethrow.
      throw $e;
    }
  }
}

/**
 * Placeholder for arguments.
 */
class DrakeContextArgPlaceholder extends DrakeContextPlaceholder {
  protected $description;
  /**
   * Constructs the placeholder.
   */
  public function __construct($index, $description, $optional = FALSE, $default_value = NULL) {
    parent::__construct($index, $optional, $default_value);
    $this->description = $description;
  }

  /**
   * String representation of this context.
   */
  public function __toString() {
    return "[Argument " . $this->name . "]";
  }

  /**
   * Resolves the placeholder to an value.
   */
  public function resolve(DrakeContext $context) {
    try {
      return $context['arg' . $this->name];
    }
    catch (DrakeContextNotFoundException $e) {
      if ($this->optional) {
        return $this->default_value;
      }
      throw new DrakeContextNotFoundException("Missing argument: " . $this->description);
    }
  }
}

class DrakeContextPlaceholderSiteOp {
  protected $site;

  public function __construct($value) {
    $this->site = $value;
  }
  public function __toString() {
    return $this->site;
  }

  protected function record() {
    $record = drush_sitealias_get_record($this->site);
    if (empty($record)) {
      throw new DrakeContextException(dt('No "@site" site found', array('@site' => $this->site)));
    }
    return $record;
  }

  public function getRoot() {
    $record = $this->record();
    if (!empty($record['root'])) {
      return $record['root'];
    }
    throw new DrakeContextException(dt('No site root found for "@site"', array('@site' => $this->site)));
  }

  public function getPath() {
    $record = $this->record();
    if (!empty($record['root']) && $path = drush_sitealias_local_site_path($record)) {
      return preg_replace('/^' . preg_quote($record['root'] . '/', '/') . '/', '', $path);
    }
    throw new DrakeContextException(dt('No site path found for "@site"', array('@site' => $this->site)));
  }
}

class DrakeContextPlaceholderStringOp {
  protected $string;

  public function __construct($value) {
    $this->string = $value;
  }
  public function __toString() {
    return $this->string;
  }

  public function getUpcase() {
    return strtoupper($this->string);
  }

  public function getDowncase() {
    return strtolower($this->string);
  }
}

class DrakeContextPlaceholderFileOp {
  protected $string;

  public function __construct($value) {
    $this->string = $value;
  }
  public function __toString() {
    return $this->string;
  }

  public function getDirname() {
    return dirname($this->string);
  }

  public function getBasename() {
    return basename($this->string);
  }
}

/**
 * An context exception class.
 */
class DrakeContextException extends Exception {}
class DrakeContextNotFoundException extends DrakeContextException {}

/**
 * Implements the context object.
 *
 * A context wraps a set of variables for tasks. It may also have a parent
 * context, which is asked if the current context doesn't define a variable.
 */
class DrakeContext implements ArrayAccess {
  protected $parents;
  protected $values = array();
  protected $forced = array();

  /**
   * Constructs a new context.
   *
   * @param DrakeContext $parent
   *   Parent context.
   */
  public function __construct($parents = array()) {
    if (!is_array($parents)) {
      $parents = array($parents);
    }
    $this->parents = $parents;
  }


  /**
   * Enforce a value.
   *
   * This causes the value to be enforced in child contexts.
   */
  function force($key, $value) {
    $this[$key] = $value;
    $this->forced[$key] = TRUE;
  }

  /**
   * Whether a given key is forced.
   */
  function is_forced($key) {
    return isset($this->forced[$key]);
  }

  /**
   * Resolves context placeholders to actual values.
   *
   * Recursively replaces the DrakeContextPlaceholder objects in the provided
   * array, with values from this context.
   *
   * @param array $array
   *   The array to replace placeholders in.
   */
  function resolve(&$array) {
    foreach ($array as $key => &$value) {
      if (is_array($value)) {
        $this->resolve($value);
      }
      else {
        // Using while, as the value might also be a placeholder.
        while ($value instanceof DrakePlaceholder) {
          $value = $value->resolve($this);
          // If placeholder returned null, remove the value.
          if (is_null($value)) {
            unset($array[$key]);
          }
        }
      }
    }
  }

  /**
   * Get offset, but ignore certain values.
   */
  function get_not($offset, $ignore = array()) {
    return $this->offsetGet($offset, $ignore);
  }

  /**
   * Take an MD5 sum of this context.
   *
   * Gives a unique id for this context, depending on content.
   */
  function md5() {
    return md5(serialize($this->toArray()));
  }

  /**
   * Get the context as an array.
   *
   * Gets the context content from this context and its parents as a simple
   * array.
   */
  function toArray() {
    $return = array();
    foreach ($this->parents as $parent) {
      $return = array_merge($return, $parent->toArray());
    }
    return array_merge($return, $this->values);
  }

  /**
   * Implements ArrayAccess::offsetExists().
   */
  public function offsetExists($offset) {
    if (isset($this->values[$offset]) || array_key_exists($offset, $this->values)) {
      return TRUE;
    }
    elseif (!empty($this->parents)) {
      foreach ($this->parents as $parent) {
        if ($parent->offsetExists($offset)) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Implements ArrayAccess::offsetGet().
   */
  public function offsetGet($offset, $ignore = array()) {
    if ((isset($this->values[$offset]) ||
        array_key_exists($offset, $this->values)) &&
      !in_array($this->values[$offset], $ignore)) {
      return $this->values[$offset];
    }
    else {
      if (!empty($this->parents)) {
        // Check backwards as latter values takes priority.
        foreach (array_reverse($this->parents) as $parent) {
          try {
            return $parent->offsetGet($offset, $ignore);
          }
          catch (DrakeContextNotFoundException $e) {
            // Do nothing, try the next parent.
          }
        }
      }
    }
    // Couldn't find a value, throw exception.
    throw new DrakeContextNotFoundException('No such context "' . $offset . '"');
  }

  /**
   * Implements ArrayAccess::offsetSet().
   */
  public function offsetSet($offset, $value) {
    if ($this->parents) {
      foreach ($this->parents as $parent) {
        if ($parent->is_forced($offset)) {
          // Don't set the value if the parent enforces the value.
          return;
        }
      }
    }
    $this->values[$offset] = $value;
  }

  /**
   * Implements ArrayAccess::offsetUnset().
   */
  public function offsetUnset($offset) {
    unset($this->values[$offset]);
  }

}
