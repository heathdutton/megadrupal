<?php

/**
* @file
* Views field handler. Adds the add to cart radio field to the view.
* Implements the Views Form API.
*/

class commerce_add_to_cart_radio_handler_field extends views_handler_field {
  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['show_quantity'] = array('default' => FALSE);
    $options['quantity_widget'] = array('default' => 'input');
    $options['quantity_select_range'] = array('default' => '1-5');
    $options['quantity_select_suffix'] = array('default' => '');
    $options['quantity_select_prefix'] = array('default' => '');
    $options['quantity_select_show_price'] = array('default' => TRUE);
    $options['quantity_label'] = array('default' => '');
    $options['combine'] = array('default' => TRUE);
    $options['display_path'] = array('default' => FALSE);
    $options['line_item_type'] = array('product' => t('Product'));

    return $options;
  }

  /**
   * Provide the add to cart display options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['combine'] = array(
      '#type' => 'checkbox',
      '#title' => t('Attempt to combine same products on the same line item in the cart.'),
      '#description' => t('The line item type, referenced product, and data from fields exposed on the Add to Cart form must all match to combine.'),
      '#default_value' => $this->options['combine'],
    );

    $form['show_quantity'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display a quantity widget on the add to cart form.'),
      '#default_value' => $this->options['show_quantity'],
    );

    $form['quantity_widget'] = array(
      '#type' => 'select',
      '#title' => t('Widget type for the quantity element.'),
      '#options' => array('input' => 'Input textfield', 'select' => 'Input Select'),
      '#default_value' => $this->options['quantity_widget'],
      '#states' => array(
        'visible' => array(
          ':input[name="options[show_quantity]"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['quantity_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label for the quantity element.'),
      '#default_value' => $this->options['quantity_label'],
      '#size' => '60',
      '#states' => array(
        ':input[name="options[show_quantity]"]' => array('checked' => TRUE),
      ),
    );

    $form['quantity_select_range'] = array(
      '#type' => 'textfield',
      '#title' => t('Range of quantity options to display in the quantity select widget.'),
      '#description' => t('Define a range in the format 1-5 or 1,3,5,7 depending if you want a continuous serie or custom quantities.'),
      '#default_value' => $this->options['quantity_select_range'],
      '#states' => array(
        'visible' => array(':input[name="options[quantity_widget]"]' => array('value' => 'select')),
      ),
    );

    $form['quantity_select_suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix to add after the quantity number options.'),
      '#default_value' => $this->options['quantity_select_suffix'],
      '#size' => '20',
      '#states' => array(
        'visible' => array(':input[name="options[quantity_widget]"]' => array('value' => 'select')),
      ),
    );

    $form['quantity_select_prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix to add after the quantity number options.'),
      '#default_value' => $this->options['quantity_select_prefix'],
      '#size' => '20',
      '#states' => array(
        'visible' => array(':input[name="options[quantity_widget]"]' => array('value' => 'select')),
      ),
    );

    $form['quantity_select_show_price'] = array(
      '#type' => 'checkbox',
      '#title' => t('Quantity select total price.'),
      '#description' => t('Display the total price (product price X quantity) for each quantity option.'),
      '#default_value' => $this->options['combine'],
      '#states' => array(
        'visible' => array(':input[name="options[quantity_widget]"]' => array('value' => 'select')),
      ),
    );

    // Add a conditionally visible line item type element.
    $types = commerce_product_line_item_types();

    if (count($types) > 1) {
      $form['line_item_type'] = array(
        '#type' => 'select',
        '#title' => t('Add to Cart line item type'),
        '#options' => array_intersect_key(commerce_line_item_type_get_name(), drupal_map_assoc($types)),
        '#default_value' => !empty($this->options['line_item_type']) ? $this->options['line_item_type'] : 'product',
      );
    }
    else {
      $form['line_item_type'] = array(
        '#type' => 'hidden',
        '#value' => key($types),
      );
    }
  }

  /**
   * The form which replaces the placeholder from render().
   */
  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }

    foreach ($this->view->result as $row_index => $row) {
      $product_id = $this->get_value($row);

      // store the product_id in the form for mapping row_ids to products
      $form[$this->options['id']][$row_index]['#product_id'] = $product_id;

      $form[$this->options['id']][$row_index]['radio'] = array(
        '#name' => $this->options['id'],
        '#type' => 'radio',
        '#return_value' => $product_id,
      );

      // load current product
      $product = commerce_product_load($product_id);

      // Extract a default quantity for the Add to Cart form line item.
      $default_quantity = 1;
      $product_ids = array($product->product_id);

      if ($this->options['show_quantity'] && $this->options['quantity_widget'] == 'input') {
        $form[$this->options['id']][$row_index]['quantity' . '_' . $product_id] = array(
          '#type' => 'textfield',
          '#default_value' => $default_quantity,
          '#datatype' => 'integer',
          '#size' => 5,
          '#weight' => 45,
        );
        
        $this->element_quantity_label($form[$this->options['id']][$row_index]['quantity' . '_' . $product_id]);
      }
      else if ($this->options['show_quantity'] && $this->options['quantity_widget'] == 'select') {
        // determine which range is configured and if not found any valid format define a
        // default range
        if (strpos($this->options['quantity_select_range'], '-') !== FALSE) {
          list($init, $end) = explode('-', $this->options['quantity_select_range']);
          $range = range((int) $init, (int) $end);
        }
        else if (strpos($this->options['quantity_select_range'], ',') !== FALSE) {
          $range = explode(',', $this->options['quantity_select_range']);
          $range = array_map('abs', $range);
        }
        else {
          $range = range(1, 5);
        }

        // build the options asoc array
        $options = array('_none' => t('-- Select --'));
        foreach ($range as $quantity) {
          $options[$quantity] = $this->options['quantity_select_prefix'] . $quantity . $this->options['quantity_select_suffix'];
          if ($this->options['quantity_select_show_price']) {
            $price = commerce_add_to_cart_radio_calculate_sell_price($product, $quantity);
            $options[$quantity] .= ' - ' . $price;
          }
        }
        $form[$this->options['id']][$row_index]['quantity' . '_' . $product_id] = array(
          '#type' => 'select',
          '#options' => $options,
          '#weight' => 45,
          '#prefix' => '<div class="add-to-cart-radio-select-quantity">',
          '#suffix' => '</div>',
        );

        $this->element_quantity_label($form[$this->options['id']][$row_index]['quantity' . '_' . $product_id]);
      }

      // Build the line item we'll pass to the Add to Cart form.
      $line_item = commerce_product_line_item_new($product, $default_quantity, 0, array(), $this->options['line_item_type']);
      $line_item->data['context']['product_ids'] = $product_ids;
      $line_item->data['context']['add_to_cart_combine'] = $this->options['combine'];

      $form[$this->options['id']][$row_index]['line_item_fields'] = array(
        '#type' => 'container',
        '#parents' => array('line_item_fields', $row_index),
        '#weight' => 10,
        '#tree' => TRUE,
      );

      $form_state[$row_index]['line_item'] = $line_item;
      field_attach_form('commerce_line_item', $form_state[$row_index]['line_item'], $form[$this->options['id']][$row_index]['line_item_fields'], $form_state);

      $line_item_fields =& $form[$this->options['id']][$row_index]['line_item_fields'];

      // Loop over the fields we just added and remove any that haven't been
      // marked for inclusion on this form. We do this instead of just setting
      // '#access' => FALSE to avoid specifying empty values for hidden fields.
      foreach (element_children($line_item_fields) as $field_name) {
        $info = field_info_instance('commerce_line_item', $field_name, $form_state[$row_index]['line_item']->type);
        $line_item_fields[$field_name]['#commerce_cart_settings'] = commerce_cart_field_instance_access_settings($info);

        // As we display a multi form we need to disable the required option
        // and verify at form_validate only for the marked radio element
        $line_item_fields[$field_name][LANGUAGE_NONE]['#required'] = FALSE;

        if (empty($line_item_fields[$field_name]['#commerce_cart_settings']['field_access'])) {
          $line_item_fields[$field_name]['#access'] = FALSE;
        }
      }
    }
  }

  /**  
   * Set the element quantity label if configured
   */
  private function element_quantity_label(&$form) {
    // set the form element label if configured
    if (is_array($form) && $this->options['quantity_label']) {
      $form['#title'] = $this->options['quantity_label'];
    }
  }

  /**
   * Form validate function
   */
  function views_form_validate($form, &$form_state) {
    $field_name = $this->options['id'];

    // if the show_quantity setting is disabled omit quantity validation
    if (!$this->options['show_quantity']) {
      return;
    }

    // if no selected product then omit validation
    if (!isset($form_state['input'][$field_name])) {
      return;
    }

    // selected product
    $selected_product_id = $form_state['input'][$field_name];
    if (!isset($form_state['values']['quantity_' . $selected_product_id]) || !is_numeric($form_state['values']['quantity_' . $selected_product_id])) {
      form_set_error('quantity_' . $selected_product_id, t("Please select a quanitity."));
    }
  }

  /**
   * Form submit function
   */
  function views_form_submit($form, &$form_state) {
    global $user;
    $field_name = $this->options['id'];

    // if no selected product then omit add to cart logic
    if (!isset($form_state['input'][$field_name])) {
      return;
    }

    // selected product
    $selected_product_id = $form_state['input'][$field_name];

    foreach (element_children($form[$field_name]) as $row_id) {
      // Find the row_id for the selected product
      $product_id = $form[$field_name][$row_id]['#product_id'];
      if ($selected_product_id == $product_id) {
        $selected_product_row_id = $row_id;
        break;
      }
    }

    // load the selected product
    $product = commerce_product_load($product_id);

    if (isset($form_state['values']['quantity_' . $selected_product_id])) {
      $quantity = $form_state['values']['quantity_' . $selected_product_id];
    }
    else {
      $quantity = 1;
    }

    // If the line item passed to the function is new...
    if (empty($form_state[$selected_product_row_id]['line_item']->line_item_id)) {
      // Create the new product line item of the same type.
      $line_item = commerce_product_line_item_new($product, $quantity, 0, $form_state[$selected_product_row_id]['line_item']->data, $form_state[$selected_product_row_id]['line_item']->type);

      // Unset the line item field values array if it is now empty.
      if (empty($form_state['values']['line_item_fields'])) {
        unset($form_state['values']['line_item_fields']);
      }

      // Add field data to the line item.
      field_attach_submit('commerce_line_item', $line_item, $form[$field_name][$selected_product_row_id]['line_item_fields'], $form_state);

      // Only attempt an Add to Cart if the line item has a valid unit price.
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

      if (!is_null($line_item_wrapper->commerce_unit_price->value())) {
        // Add the product to the specified shopping cart.
        $form_state[$selected_product_row_id]['line_item'] = commerce_cart_product_add(
          $user->uid,
          $line_item,
          isset($line_item->data['context']['add_to_cart_combine']) ? $line_item->data['context']['add_to_cart_combine'] : TRUE
        );
      }
    }
  }
}
