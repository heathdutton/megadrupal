<?php
/**
 * @file
 * Views Elastic Grid style plugin.
 */

/**
 * Style plugin to render each item in an ordered or unordered list.
 *
 * @ingroup views_style_plugins
 */
class views_elastic_grid_plugin_style_elastic_grid extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['views_elastic_grid']['thumbnail_fields'] = array('default' => array());
    $options['views_elastic_grid']['expanded_fields'] = array('default' => array());
    $options['type'] = array('default' => 'ul');
    $options['class'] = array('default' => '');
    $options['wrapper_class'] = array('default' => 'item-list');

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $field_names = $this->display->handler->get_field_labels();
    $form['views_elastic_grid'] = array(
      '#title' => t('Elastic Grid Settings'),
      '#type' => 'fieldset',
    );
    $form['views_elastic_grid']['thumbnail_fields'] = array(
      '#title' => t('Thumbnail Fields'),
      '#description' => t('The fields that should be displayed as part of the thumbnail.'),
      '#type' => 'checkboxes',
      '#default_value' => $this->options['views_elastic_grid']['thumbnail_fields'],
      '#options' => $field_names,
    );
    $form['views_elastic_grid']['expanded_fields'] = array(
      '#title' => t('Expanded Area Fields'),
      '#description' => t('The fields that should be displayed as part of the expanded area.'),
      '#type' => 'checkboxes',
      '#default_value' => $this->options['views_elastic_grid']['expanded_fields'],
      '#options' => $field_names,
    );
    $form['views_elastic_grid']['id_field'] = array(
      '#title' => t('ID Field for URL Fragments'),
      '#description' => t('A unique ID field (Node ID, etc.) so that links to specific items open those items on page load.'),
      '#type' => 'select',
      '#default_value' => $this->options['views_elastic_grid']['id_field'],
      '#options' => array_merge(array('' => 'View Row Number'), $field_names),
    );
    $form['wrapper_class'] = array(
      '#title' => t('Wrapper class'),
      '#description' => t('The class to provide on the wrapper, outside the list.'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['wrapper_class'],
    );
    $form['class'] = array(
      '#title' => t('List class'),
      '#description' => t('The class to provide on the list element itself.'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['class'],
    );
  }
}
