<?php
/**
 * @file
 * Giant Bomb data property handler.
 */

class GiantBombHTMLProperty extends GiantBombBaseProperty {
  function value() {
    $value = $this->value;

    if (!empty($this->settings['strip_html'])) {
      $value = strip_tags($value);
    }

    return filter_xss($value);
  }

  function validate() {
    return TRUE;
  }

  static function form(&$form, &$form_state, $settings = array()) {
    $property_form['strip_html'] = array(
      '#title' => t('Strip HTML'),
      '#type' => 'checkbox',
      '#description' => t('Remove HTML formatting, and convert to plain text.'),
    );
    return $property_form;
  }
}