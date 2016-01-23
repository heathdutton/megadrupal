<?php

/**
 * @file
 * Definition of lightshop_handler_field_lightshop_order_items.
 */

/**
 * Field handler to show items for the order.
 *
 * @ingroup views_field_handlers
 */
class lightshop_handler_field_lightshop_order_items extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => 'default');
    $options['show_as_link'] = array('default' => TRUE);
    $options['delimiter'] = array('default' => ' | ');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['format'] = array(
      '#type' => 'select',
      '#title' => t('The display format'),
      '#description' => t('Select the output format for order items display.'),
      '#options' => array(
        'default' => t('Default view'),
        'table' => t('Table view'),
        'list' => t('List view'),
        'only_total_sum' => t('Only the total sum'),
      ),
      '#default_value' => $this->options['format'],
    );
    $form['show_as_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link the title of the product to the original piece of content.'),
      '#default_value' => $this->options['show_as_link'],
      '#dependency' => array('edit-options-format' => array('list')),
    );
    $form['delimiter'] = array(
      '#type' => 'textfield',
      '#title' => t('Delimiter for values inside one element of order.'),
      '#default_value' => $this->options['delimiter'],
      '#dependency' => array('edit-options-format' => array('list')),
    );
  }

  function render($values) {
    $value = (array) unserialize($values->{$this->field_alias});

    switch ($this->options['format']) {
      case 'default':
        return theme('lightshop_items_table', array('basket' => $value));

      case 'table':
        return theme('table', array('rows' => $value['items'], 'header' => _lightshop_basket_items_header()));

      case 'list':
        $delimiter = !empty($this->options['delimiter']) ? $this->options['delimiter'] : ' | ';
        $item_list = array();

        foreach ($value['items'] as $nid => $item) {
          foreach ($item as $key => $val) {
            if ($key == 'title') {
              if ($this->options['show_as_link']) {
                $item[$key] = l($val, 'node/' . $nid);
              }
            }
            else {
              if ($key == 'price') {
                $val = _lightshop_get_price_numberformat($val);
              }
              $item[$key] = $key . ': ' . $val;
            }
          }
          $item_list[] = implode($delimiter, $item);
        }
        return theme('item_list', array('items' => $item_list));

      case 'only_total_sum':
        return isset($value['total_sum']) ? _lightshop_get_price_numberformat($value['total_sum']) : '';
    }
  }
}
