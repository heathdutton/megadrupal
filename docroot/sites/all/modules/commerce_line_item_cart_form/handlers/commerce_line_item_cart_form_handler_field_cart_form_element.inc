<?php

/**
 * @file
 * Field handler to present a form field to change a line item field. It's
 * a dummy handler based on the edit quantity handler, most part of the
 * implementation is done via post render hook.
 */

/**
 * Field handler to display the form element of a line item field.
 */
class commerce_line_item_cart_form_handler_field_cart_form_element extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['line_item_id'] = 'line_item_id';

    // Set real_field in order to make it generate a field_alias.
    $this->real_field = 'line_item_id';
  }

  /**
   * Creates placeholder for form element
   */
  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['cart_form_hide_label'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['cart_form_hide_label'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide field label for cart form element'),
      '#default_value' => $this->options['cart_form_hide_label'],
    );
  }

  /**
   * Returns the form element which replaces the placeholder from render().
   */
  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }
 
    // This is necessary for field_default_form() not to yell at us.
    if (!isset($form['#parents'])) {
      $form['#parents'] = array();
    }

    $form[$this->options['id']] = array(
      '#tree' => TRUE,
    );
    foreach ($this->view->result as $row_id => $row) {
      $line_item_id = $this->get_value($row, 'line_item_id');
      $line_item = commerce_line_item_load($line_item_id);

      // Only add the form when the commerce_line_item type has this field assigned.
      if (property_exists($line_item, $this->definition['field name'])) {
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

        // This code for field form building is adapted from:
        // http://www.alexweber.com.br/en/articles/how-manually-add-entity-field-forms-custom-drupal-form
        $bundle_name = $line_item_wrapper->getBundle();
        $langcode =  $line_item_wrapper->getPropertyLanguage();
        if (empty($langcode)) {
          $langcode = LANGUAGE_NONE;
        }
        $field_name = $this->definition['field name'];
        $items = field_get_items('commerce_line_item', $line_item, $field_name);
        $field = field_info_field($field_name);
        $instance = field_info_instance('commerce_line_item', $field_name, $bundle_name);

        if ($this->options['cart_form_hide_label']) {
          $instance['label'] = '';
        }

        $form[$this->options['id']][$row_id] = field_default_form('commerce_line_item', $line_item, $field, $instance, $langcode, $items, $form, $form_state);
        $form[$this->options['id']][$row_id]['#line_item_id'] = $line_item_id;
      }
    }
  }

  function views_form_submit($form, &$form_state) {
    $field_name = $this->options['id'];
    foreach (element_children($form[$field_name]) as $row_id) {
      $line_item_id = $form[$field_name][$row_id]['#line_item_id'];
      // If the line item hasn't been deleted...
      if ($line_item = commerce_line_item_load($line_item_id)) {
        // And the field exists...
        if (property_exists($line_item, $this->definition['field name'])) {
          $line_item->{$this->definition['field name']} = $form_state['values'][$field_name][$row_id][$this->definition['field name']];
          commerce_line_item_save($line_item);
        }
      }
    }
  }
}
