<?php
/**
 * @file
 *  The base class for a pluggable mail plugin
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */
namespace Drupal\PluggableMailers;

class PluggableMailPlugin
  implements PluggableMailInterface {

  /**
   * Parameters
   */
  protected $params = array();

  /**
   * Constructor
   */
  public function __construct(){

    return $this;
  }

  /**
   * Implements PluggableMailInterface::setParams()
   *
   * @param array $params
   */
  public function setParams($params) {
    foreach($params as $key => $value) {
      $this->params[$key] = $value;
    }
  }

  /**
   * Implements PluggableMailInterface::getParams()
   *
   * @return array
   */
  public function getParams() {
    return $this->params;
  }

  /**
   * Implements PluggableMailInterface::param()
   *
   * @param string $key
   * @param mixed $override [optional]
   *
   * @return mixed|bool
   */
  public function param($key, $override = NULL) {
    if (isset($override) && !is_null($override)) {
      return $override;
    }
    elseif (isset($this->params[$key])) {
      return $this->params[$key];
    }

    return FALSE;
  }

  /**
   * Set the mail system to use
   *
   * This simply returns the default configuration for the mail_system module.
   */
  public function mailSystem() {
    return FALSE;
  }

  /**
   * Implements PluggableMailInterface::template()
   *
   * @return bool
   */
  public function template() {
    return FALSE;
  }

  /**
   * Implements PluggableMailInterface::templateContent()
   *
   * @return array
   */
  public function templateContent(){
    return array();
  }
}
