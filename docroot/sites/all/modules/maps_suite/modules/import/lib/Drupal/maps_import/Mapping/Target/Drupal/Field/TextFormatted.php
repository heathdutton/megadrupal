<?php

/**
* @file
* Field handler for Text Formatted fields.
*/

namespace Drupal\maps_import\Mapping\Target\Drupal\Field;

/**
 * Drupal Field class for field which type is "text_formatted".
 */
class TextFormatted extends DefaultField {

  /**
   * @inheritdoc
   */
  public function sanitize($values) {
    $options = $this->getOptions();

    foreach ($values as &$value) {
      $value = array(
        'value' => $value,
        'format' => !empty($options['format']) ? $options['format'] : filter_default_format(),
      );      
    }

    return parent::sanitize($values);
  }

  /**
   * @inheritdoc
   */
  public function hasOptions() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function optionsFormValidate($form, &$form_state) {
    if (empty($form_state['values']['format'])) {
      form_set_error('format', t('!format field is required.', array('!format' => $form_state['values']['format'])));
    }

    $values = maps_suite_reduce_array(filter_formats(), 'name', TRUE);
    if (!in_array($form_state['values']['format'], $values)) {
      form_set_error('format', t('Incorrect value !value for format.', array('!value' => $form_state['values']['format'])));
    }
  }

  /**
   * @inheritdoc
   */
  public function optionsForm($form, &$form_state) {
    $options = $form_state['item']->getOptions();

    return array(
      'format' => array(
        '#type' => 'select',
        '#title' => t('Text format'),
        '#options' => maps_suite_reduce_array(filter_formats(), 'name', TRUE),
        '#default_value' => !empty($options['target']['format']) ? $options['target']['format'] : '',
        '#description' => theme('filter_tips_more_info'),
      )
    );
  }

  /**
   * @inheritdoc
   */
  public function getOptionsDefault() {
    return array('format' => '');
  }

}
