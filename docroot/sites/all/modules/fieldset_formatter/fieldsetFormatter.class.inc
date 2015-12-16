<?php

/**
 * @file
 * Internal functions for fieldset formatter.
 */

class fieldsetFormatter {

  private static $defaults;

  /**
   * Provides initial settings for a new filter configuration.
   *
   * @return string
   *   Content usable in a textarea.
   */
  public static function getTypeDefaults() {
    if (!isset(self::$defaults)) {
      self::$defaults = array(
        format_string('example:collapsible,collapsed:!label', array(
          '!label' => t('Example'),
        )),
        format_string('example2::!label', array(
          '!label' => t('Example without collapsing', array(), array('context' => 'Fieldsets')),
        )),
        format_string('note:collapsible:!label', array(
          '!label' => t('Note', array(), array('context' => 'Something worth noticing')),
        )),
        format_string('hint:collapsible,collapsed:!label', array(
          '!label' => t('Hint'),
        )),
        format_string('danger::!label', array(
          '!label' => t('Danger'),
        )),
        format_string('disclaimer::!label', array(
          '!label' => t('Disclaimer'),
        )),
      );
    }
    return implode("\n", self::$defaults);
  }

}

