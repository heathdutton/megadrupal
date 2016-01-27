<?php

/**
 * @file 
 * Attribute handler for the attribute field.
 */

/**
 * Field handler to present an order's operations links.
 */
class commerce_product_attributes_handler_field_attributes extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['line_item_id'] = 'line_item_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['show_link'] = array('default' => TRUE);
    $options['link_with_line_item'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['show_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link to Product'),
      '#default_value' => $this->options['show_link'],
      '#description' => t('Check this if the product title should be linked with the product display.'),
    );
    $form['link_with_line_item'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link With Line Item'),
      '#default_value' => $this->options['link_with_line_item'],
      '#description' => t('Check this if link should contain the line item. This allow the editing of the line item by the customer.'),
    );


  }
  
  function render($values) {
    $line_item_id = $this->get_value($values, 'line_item_id');
    $line_item = commerce_line_item_load($line_item_id);
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    
    $properties = $line_item_wrapper->getPropertyInfo();
    
    $element = array();
    
    if (isset($properties['commerce_display_path']) && $this->options['link_with_line_item']) {
      $path = $line_item_wrapper->commerce_display_path->value();
      $options = array(
        'query' => array(
          'line_item_id' => $line_item_id,
        )
      );
    }
    else {
      $path = NULL;
      $options = array();
    }
    
    $element['title'] = array(
      '#markup' => theme('commerce_product_attributes_item_title', array('item' => $line_item, 'path' => $path, 'options' => $options, 'create_link' => $this->options['show_link'])),
    );
    
    $hook = 'attribute_field';
    foreach (module_implements($hook) as $module_name) {
      $function = $module_name . '_' . $hook;
      if (function_exists($function)) {
        $function($element, $line_item);
      }
    }
    
    $element['#attached']['css'][] = drupal_get_path('module', 'commerce_product_attributes') . '/theme/commerce_product_attributes_cart.css';

    return drupal_render($element);

  }
}
