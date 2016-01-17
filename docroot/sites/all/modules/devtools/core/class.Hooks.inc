<?php
namespace devtools;

/**
 * Allows multiple components to extend a hook call.
 */
class Hooks extends \devtools\Base {
  protected $_path;
  protected $_components_path;
  protected $_module_path;

  /**
   * Sets path.
   * @param [type] $path [description]
   */
  function setPath($path) {
    $this->_path = $path;
  }

  /**
   * Sets components path.
   * @param [type] $path [description]
   */
  function setComponentsPath($path) {
    $this->_components_path = $path;
  }

  /**
   * Sets module path.
   * @param [type] $path [description]
   */
  function setModulePath($path) {
    $this->_module_path = $path;
  }

  /**
   * Placeholder for post-initialization operations.
   * @return [type] [description]
   */
  function postInit() {
    
  }
}