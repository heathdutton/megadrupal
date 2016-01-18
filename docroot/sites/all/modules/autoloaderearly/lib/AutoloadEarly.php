<?php

/**
 * Early dynamic autoloader: this class is a singleton on which you can register
 * as many raw file map, or namespaces base path you wish.
 * 
 * The namespace based autoloading respects the PSR-0 standard.
 */
class AutoloadEarly {
  /**
   * @var AutoloadEarly
   */
  private static $__instance;

  /**
   * Get instance.
   * 
   * @return AutoloadEarly
   */
  public static function getInstance() {
    if (!isset(self::$__instance)) {
      global $conf;
      $debug = isset($conf['autoloadearly_debug']) && $conf['autoloadearly_debug'];
      self::$__instance = new self;
      self::$__instance->setDebug($debug);
      // First access means we are registering stuff: we do here a lazy
      // autoloader registration.
      spl_autoload_register(array(self::$__instance, ($debug ? 'autoloadDebug' : 'autoload')));
    }
    return self::$__instance;
  }

  /**
   * @var bool
   */
  protected $_debug = FALSE;

  /**
   * Change debug status.
   * 
   * @param bool $toggle
   */
  public function setDebug($toggle) {
    $this->_debug = $toggle;
  }

  /**
   * Namespace map for autoloading.
   * 
   * @var array
   */
  protected $_namespaceMap = array();

  /**
   * Register one namespace.
   * 
   * You can use it to register one single namespace:
   * 
   * <code>
   * AutoloadEarly::getInstance()
   *   ->registerNamespace('MyModule',
   *                       DRUPAL_ROOT . '/sites/all/modules/mymodule/lib');
   * </code>
   * 
   * Or to register multiple namespaces at once:
   * 
   * <code>
   * AutoloadEarly::getInstance()
   *   ->registerNamespace(array(
   *     'MyModule'     => DRUPAL_ROOT . '/sites/all/modules/mymodule/lib'
   *     'SomeOtherLib' => DRUPAL_ROOT . '/sites/all/libraries/SomeOtherLib',
   *   ));
   * </code>
   * 
   * @param string|array $namespaceOrList
   * @param string $basepath
   * 
   * @throws Exception
   */
  public function registerNamespace($namespaceOrList, $basepath = NULL) {
    if (isset($basepath) && is_string($basepath) && is_string($namespaceOrList)) {
      $this->_namespaceMap[$namespaceOrList] = $basepath;
    }
    else if (is_array($namespaceOrList)) {
      foreach ($namespaceOrList as $name => $basepath) {
        $this->_namespaceMap[$name] = $basepath;
      }
    }
    else {
      throw new Exception("Wrong AutoloadEarly::registerNamespace() usage.");
    }
  }

  /**
   * File map for those who'd prefer to use a fixed file list.
   * 
   * @var array
   */
  protected $_fileMap = array();

  /**
   * Register specific class names within file list.
   * 
   * @param array $map
   *   Map of class with associated files. Key value pairs where keys are class
   *   names, and values are files where to find the associated class.
   */
  public function registerFiles(array $map) {
    $this->_fileMap += $map;
  }

  /**
   * Autoload callback, suitable for spl_register_autoloader as a callable.
   */
  public function autoload($className) {

    // First check with file map which is the fastest.
    if (isset($this->_fileMap[$className])) {
      require_once $this->_fileMap[$className];
      return TRUE;
    }

    if (FALSE !== strpos($className, '\\')) {
      // Support PHP 5.3 namespaces.
      $parts = explode("\\", $className);
    }
    else {
      // Legacy PHP 5.2 namespacing (as of Zend namespacing, legacy PSR-0).
      $parts = explode('_', $className);
    }

    // Test the namespace map, this will be slower because due to the file
    // input/ouput operations induced.
    if (isset($this->_namespaceMap[$parts[0]])) {
      $filePath = $this->_namespaceMap[$parts[0]] . '/' . implode('/', $parts) . '.php';

      if (file_exists($filePath)) {
        require_once $filePath;
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Same autoloader implementation, except it will do file_exists() calls and
   * check for class existence, and throws nice exceptions instead of going
   * WSOD in case of errors.
   * 
   * This implementation will be a lot slower, don't use on production sites.
   */
  public function autoloadDebug($className) {

    // First check with file map which is the fastest.
    if (isset($this->_fileMap[$className])) {

      if (!file_exists($this->_fileMap[$className])) {
        throw new Exception("File " . $this->_fileMap[$className] . " has been registered for class " . $className . " but does not exists");
      }

      require_once $this->_fileMap[$className];

      if (!class_exists($className) && !interface_exists($className)) {
        throw new Exception("File " . $this->_fileMap[$className] . " exists but class " . $className . " still does not exists");
      }

      return TRUE;
    }

    if (FALSE !== strpos($className, '\\')) {
      // Support PHP 5.3 namespaces.
      $parts = explode('\\', $className);
    }
    else {
      // Legacy PHP 5.2 namespacing (as of Zend namespacing, legacy PSR-0).
      $parts = explode('_', $className);
    }

    // Test the namespace map, this will be slower because due to the file
    // input/ouput operations induced.
    if (isset($this->_namespaceMap[$parts[0]])) {
      $filePath = $this->_namespaceMap[$parts[0]] . '/' . implode('/', $parts) . '.php';

      if (file_exists($filePath)) {
        require_once $filePath;

        if (!class_exists($className) && !interface_exists($className)) {
          throw new Exception("Namespace " . $parts[0] . " contains the file " . $filePath . " but class " . $className . " still does not exists");
        }

        return TRUE;
      }
      else {
        throw new Exception("Namespace " . $parts[0] . " found for class " . $className . " but file " . $filePath . " does not exists");
      }
    }

    // Do not throw exception when we didn't found any direct file reference or
    // namespace, this means another autoloader does manage this class.
    return FALSE;
  }
}
