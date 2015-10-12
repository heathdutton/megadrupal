<?php
/**
 * @file
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */
namespace Drupal\PluggableMailers;

/**
 * Defines an interface for pluggable_mailers plugins
 */
interface PluggableMailInterface {

  /**
   * Return basic parameters for this email message.
   *
   * This should return standard email parameter values like 'subject' and 'body'.
   * Where a value does not exist, this should return FALSE
   *
   * @abstract
   *
   * @param $key
   *  The parameter to fetch.
   * @param $default [optional]
   *  A default value
   *
   * @return mixed
   */
  public function param($key, $default = NULL);

  /**
   * Set parameters
   *
   * @abstract
   *
   * @param array $params
   *  An array of parameters to set. Individual parameters will override any set
   *  already, however keys not in the array should be retained.
   */
  public function setParams($params);

  /**
   * Get all parameters as an array
   *
   * @abstract
   *
   * @return array
   *  An array of parameters
   */
  public function getParams();

  /**
   * Chose the mailSystem class to use
   *
   * This defaults to the core mail class, however you can also tell it to use
   * a custom class, by returning a string where the value is the mail system
   * class to use, e.g.
   *
   *  return 'ExampleMailSystem';
   *
   * @return string
   *  The mail system to use
   */
  public function mailSystem();

  /**
   * If a template is to be used, return the name of the template, or FALSE
   *
   * Not all mail handlers support templates. Core does not.
   *
   * @abstract
   * @return string|bool
   */
  public function template();

  /**
   * If a template is to be used, return an array of keys, or FALSE
   *
   * Not all mail handlers support templates. Core does not.
   *
   * @abstract
   * @return array|bool
   */
  public function templateContent();

}
