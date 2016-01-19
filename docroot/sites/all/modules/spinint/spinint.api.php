<?php

/**
 * @file
 * API definitions for module.
 * 
 * Provides hook definitions for spinint module. These are for phpdoc purposes.
 */

/**
 * Allows module to alter scrolling integer widget settings.
 *
 * @param array $element
 *   Widget element.
 * @param array $instance 
 *   Widget instance.
 * @param array $field 
 *   Widget field definition
 */
function hook_spinint_alter(&$element, &$instance, &$field) {
  // Intended to alter $element.
}
