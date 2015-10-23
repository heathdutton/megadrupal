<?php

/**
 * @file
 * Contains table_chart_plugin_display_extender_attributes.
 */

class table_chart_plugin_display_extender_attributes extends views_plugin_display_extender {

  function options_definition_alter(&$options) {
    $options['attributes'] = array('default' => array());
  }

  function options_summary(&$categories, &$options) {
    $attributes = $this->display->get_option('attributes');
    $options['attributes'] = array(
      'category' => 'other',
      'title' => t('Data Attributes'),
      'description' => t('Add data attributes to your display wrapper and fields.'),
      'value' => !empty($attributes) ? t('Attributes added'): t('None'),
    );
  }

  function options_form(&$form, &$form_state) {
    if ($form_state['section'] == 'attributes') {
      $form['#title'] .= t('Data attributes added to the display wrapper.');
      $attributes = $this->display->get_option('attributes');
      $form['attributes'] = array(
        '#type' => 'textarea',
        '#title' => t('Data attributes to add to this display\'s wrapper and fields.'),
        '#description' => t('Add one data attribute per line. The data-* will automatically be prepended to your attribute definition. Leave out the wrapping quotes around the value since Drupal will add those automatically. (e.g. mydata=custom data turns into data-mydata="custom data")'),
        '#default_value' => $attributes,
      );
    }
  }

  function options_submit(&$form, &$form_state) {
    if ($form_state['section'] == 'attributes') {
      $attributes = $form_state['values']['attributes'];
      $this->display->set_option('attributes', $attributes);
    }
  }
}
