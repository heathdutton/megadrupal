<?php
/**
 * @file
 * The Registry class
 */
class Registry {
  /**
   * Hold an instance of the class
   *
   * @var \Registry
   */
  private static $instance;

  /**
   * Somewhere to store our data
   */
  protected $data;

  /**
   * Context information set via contextSet() is stored in its own variable
   */
  protected $context;

  /**
   * A private constructor; prevents direct creation of object
   */
  private function __construct() {
    // Do nothing
  }

  /**
   * Prevent object cloning (we are a singleton)
   */
  public function __clone() {
    trigger_error('Clone is not allowed.', E_USER_ERROR);
  }

  /**
   * The singleton method
   *
   * @return \Registry
   */
  public static function invoke() {
    if (!isset(self::$instance)) {
      $c = __CLASS__;
      self::$instance = new $c;
    }

    if (!isset(self::$instance->data)){
      self::$instance->data = array();
    }

    return self::$instance;
  }

  /**
   * Set a key/value pair.
   *
   * @visibility public
   *
   * @param $key
   *  The key to store the value with.
   * @param $value
   *  The value to store
   * @param bool $locked
   *  If TRUE, lock the value from further changes.
   *
   * @return bool|\Registry
   */
  public function set($key, $value, $locked = FALSE) {
    if ($this->isLocked($key)) {
      return FALSE;
    }

    $this->data[$key]['#value'] = $value;

    $this->setLocking($key, $locked);

    return $this;
  }

  /**
   * Set a value to be returned via a callback
   *
   * @visibility public
   *
   * @param $key
   *  The key to store the value with.
   * @param $callback
   *  The name of the callback
   * @param null $args
   *  Any arguments to pass to the callback (optional)
   * @param bool $locked (optional)
   *  If TRUE, lock the value from further changes.
   *
   * @return bool|\Registry
   */
  public function setViaCallback($key, $callback, $args = NULL, $locked = FALSE) {
    if ($this->isLocked($key)) {
      return FALSE;
    }

    $this->data[$key]['#callback'] = $callback;
    $this->data[$key]['#args'] = $args;

    $this->setLocking($key, $locked);

    return $this;
  }

  /**
   * Return a value for a key
   *
   * @visibility public
   *
   * @param $key
   *  The key to return a value for
   *
   * @return bool|mixed
   *  The value returned by the key, or FALSE
   */
  public function get($key) {
    if (array_key_exists($key, $this->data)) {
      // Check callback keys
      if ($this->isCallback($key)) {
        $value = $this->loadCallback($key);
      }
      // Return a normal string
      else {
        $value = $this->data[$key]['#value'];
      }
      return $value;
    }
    return FALSE;
  }

  /**
   * Unset a key
   *
   * @param $key
   * @param bool $force
   *
   * @return bool|\Registry
   */
  public function delete($key, $force = FALSE) {
    if (!empty($this->data[$key])) {
      if ($this->isLocked($key) && $force == FALSE) {
        return FALSE;
      }
      unset($this->data[$key]);
    }

    return $this;
  }

  /**
   * Return all information held in the registry
   */
  public function all() {
    $clean = array();
    foreach ($this->data as $key => $datum) {
      $clean[$key] = $this->get($key);
    }
    return $clean;
  }

  /**
   * Set a context for this request.
   *
   * This is a slightly more complicated version of a standard set, as it tracks
   * a context 'stack' of primary (visible) and secondary (related) contexts.
   *
   * @param $key
   *  The key to store the value with.
   * @param $item
   *  The item to set
   * @param string $context_type
   *  The type of context being set. This can be anything, but for returning default
   *  context should be one of the following: (optional)
   *    - REGISTRY_CONTEXT_PRIMARY: The primary (viewed) object(s) (default)
   *    - REGISTRY_CONTEXT_SECONDARY: Any related contexts
   * @param bool $locked
   *  If TRUE, lock the value from further changes.
   *
   * @return \Registry
   */
  public function setContext($key, $item, $context_type = REGISTRY_CONTEXT_PRIMARY, $locked = FALSE) {
    $result = $this->set($key, $item, $locked);
    if ($result != FALSE) {
      $this->context[$context_type][] = $key;
    }

    return $this;
  }

  /**
   * Set a context for this request.
   *
   * This is a slightly more complicated version of a standard set, as it tracks
   * a context 'stack' of primary (visible) and secondary (related) contexts.
   *
   * @param $key
   *  The key to store the value with.
   * @param $callback
   *  The callback used to retrieve the object
   * @param string $context_type
   *  The type of context being set. This can be anything, but for returning default
   *  context should be one of the following: (optional)
   *    - REGISTRY_CONTEXT_PRIMARY: The primary (viewed) object(s) (default)
   *    - REGISTRY_CONTEXT_SECONDARY: Any related contexts
   * @param $args
   *  Any arguments to pass to the callback (optional)
   * @param bool $locked
   *  If TRUE, lock the value from further changes (optional)
   *
   * @return \Registry
   */
  public function setContextCallback($key, $callback, $context_type = REGISTRY_CONTEXT_PRIMARY, $args = NULL, $locked = FALSE) {
    $result = $this->setViaCallback($key, $callback, $args, $locked);
    if ($result != FALSE) {
      $this->context[$context_type][] = $key;
    }

    return $this;
  }

  /**
   * Retrieve the currently viewed objects for this Registry
   *
   * @param string $context_type
   *  The type of context being set. This can be anything, but for returning
   *  default context should be one of the following, or another context key set
   *  manually (optional):
   *    - REGISTRY_CONTEXT_ALL: Retrieve all contexts (default)
   *    - REGISTRY_CONTEXT_PRIMARY: The primary (viewed) object(s)
   *    - REGISTRY_CONTEXT_SECONDARY: Any related contexts
   *
   * @return array|bool
   *  An array of context items, or FALSE.
   */
  public function getContext($context_type = REGISTRY_CONTEXT_ALL) {
    $stack = array();

    if (isset($this->context)) {
      if ($context_type == REGISTRY_CONTEXT_ALL) {
        foreach ($this->context as $context_key => $context) {
          foreach ($context as $key) {
            $stack[$context_key][] = $this->get($key);
          }
        }
      }
      elseif (array_key_exists($context_type, $this->context)) {
        foreach ($this->context[$context_type] as $key) {
          $stack[$context_type][] = $this->get($key);
        }
      }
      return $stack;
    }
    return FALSE;
  }

  /**
   * Check if a property is locked or not
   *
   * @visibility protected
   *
   * @param $key
   *  The key to check.
   *
   * @return bool
   *  TRUE, if the key is locked, or FALSE.
   */
  protected function isLocked($key) {
    if (!empty($this->data[$key]) && isset($this->data[$key]['#locked']) && $this->data[$key]['#locked'] == TRUE) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Set locking for a key
   *
   * @param $key
   * @param bool $locking
   */
  protected function setLocking($key, $locking = TRUE) {
    if ($locking == TRUE) {
      $this->data[$key]['#locked'] = TRUE;
    }
    elseif ($locking == FALSE) {
      $this->data[$key]['#locked'] = FALSE;
    }
  }

  /**
   * Determine of a value is a callback or not
   *
   * @param $key
   *  The key to check.
   *
   * @return bool
   *  TRUE, if the value is a callback
   */
  protected function isCallback($key) {
    if (isset($this->data[$key]) && isset($this->data[$key]['#callback']) && $this->data[$key]['#callback'] == TRUE) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Load a parameter from a callback
   *
   * @param $key
   *  The key to load information from
   *
   * @return mixed
   *  A value is only returned if the callback produces a result
   */
  protected function loadCallback($key) {
    if (function_exists($this->data[$key]['#callback'])) {
      return call_user_func($this->data[$key]['#callback'], $this->data[$key]['#args']);
    }
  }
}
