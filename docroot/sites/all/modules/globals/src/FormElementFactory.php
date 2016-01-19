<?php

/**
 * @file
 * Contains a
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\globals;

use Drupal\ghost\Exception\GhostException;
use Drupal\ghost\Traits\InitialiserTrait;

/**
 * Class FormElementFactory
 *
 * @package Drupal\globals
 */
class FormElementFactory {

  use InitialiserTrait;

  /**
   * Get a form element by type.
   *
   * @param string $type
   *   The form API type
   *
   * @param string $title
   *   Title of the form element
   * @param null $default
   *   Default value
   * @param array $args
   *   Any other arguments
   *
   * @return array
   *   A form array element
   * @throws \Drupal\ghost\Exception\GhostException
   */
  public function getFormElement($type, $title, $default = NULL, $args = array()) {

    $element = element_info($type);
    if (empty($element)) {
      throw new GhostException('Invalid form element type');
    }

    if (is_array($args)) {
      $element = array_merge($element, $args);
    }

    $element['#title'] = $title;
    $element['#default_value'] = $default;

    return $element;
  }

}
