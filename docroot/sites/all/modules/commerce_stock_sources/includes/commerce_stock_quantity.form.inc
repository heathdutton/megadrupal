<?php
/**
 * @file
 * File to store the quantity forms.
 */

/**
 * Implements hook_form().
 */
function commerce_stock_sources_quantities_form($form, &$form_state, $commerce_stock_sources) {

  $form['#commerce_stock_sources'] = $commerce_stock_sources;

  $form['title'] = array(
    '#title' => t('Example'),
    '#type' => 'textfield',
    '#default_value' => isset($form_state['values']['title']) ? $form_state['values']['title'] : NULL,
  );

  $conditions = array(
    'source_id' => $commerce_stock_sources->source_id,
  );

  $quantities = entity_load('commerce_stock_quantity', array(), $conditions);

  $form['products'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#prefix' => '<div id="products">',
    '#suffix' => '</div>',
  );

  foreach ($quantities as $quantity) {

    $wrapper = entity_metadata_wrapper('commerce_stock_quantity', $quantity);
    $stq_id = $wrapper->stq_id->value();

    $form['products'][$stq_id] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );

    $form['products'][$stq_id]['stq_id'] = array(
      '#type' => 'value',
      '#value' => $stq_id,
    );

    $form['products'][$stq_id]['product'] = array(
      '#title' => t('Product'),
      '#type' => 'item',
      '#markup' => $wrapper->product->title->value(),
    );

    $form['products'][$stq_id]['quantity'] = array(
      '#title' => t('quantity'),
      '#type' => 'textfield',
      '#default_value' => $wrapper->quantity->value(),
    );

    $form['products'][$stq_id]['actions'] = array(
      '#type' => 'actions',
    );

    $form['products'][$stq_id]['actions']['remove'] = array(
      '#type' => 'button',
      '#value' => t('Remove'),
    );

  }

  $next = count($quantities) + 1;

  $form['products'][$next] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  $form['products'][$next]['product'] = array(
    '#title' => t('Product'),
    '#type' => 'textfield',
  );

  $form['products'][$next]['quantity'] = array(
    '#title' => t('quantity'),
    '#type' => 'textfield',
    '#default_value' => 0,
  );

  $form['products'][$next]['actions'] = array(
    '#type' => 'actions',
  );

  $form['products'][$next]['actions']['save'] = array(
    '#type' => 'button',
    '#value' => t('Save'),
    '#href' => '',
    '#ajax' => array(
      'callback' => 'commerce_stock_sources_quantities_form_ajax_add_quantity',
      'wrapper' => 'products',
    ),
  );

  $form['products'][$next]['actions']['remove'] = array(
    '#type' => 'button',
    '#value' => t('Remove'),
  );

  $form['add_quantity'] = array(
    '#type' => 'button',
    '#value' => t('Add a product quantity.'),
    '#href' => '',
    '#ajax' => array(
      'callback' => 'commerce_stock_sources_quantities_form_ajax_add_quantity',
      'wrapper' => 'products',
    ),
  );

  $form['submit'] = array(
    '#value' => t('Save'),
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Ajax handler.
 */
function commerce_stock_sources_quantities_form_ajax_add_quantity($form, $form_state) {
  return $form['products'];
}
