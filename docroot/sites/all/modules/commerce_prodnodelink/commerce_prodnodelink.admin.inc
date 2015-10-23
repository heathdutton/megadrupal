<?php

/**
 * @file
 * Admin related form(s) etc. for this module.
 */


/**
 * Creates a module settings form.
 *
 * As well as any module settings, this lists what the module knows about node
 * type/product reference fields.
 *
 * @return array
 *   Drupal Form API array.
 */
function commerce_prodnodelink_config_form($form, &$form_state) {
  $form['conf'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
  );

  $yes_no_options = array(
    0 => t('No'),
    1 => t('Yes'),
  );

  $form['conf']['commerce_prodnodelink_conf_links_on_ui_product_form'] = array(
    '#type' => 'radios',
    '#title' => t('Display links on the product edit form?'),
    '#default_value' => variable_get('commerce_prodnodelink_conf_links_on_ui_product_form', 1),
    '#options' => $yes_no_options,
    '#description' => t('Choose whether or not the list of display nodes are
      shown on the product edit form.'),
  );

  $form['conf']['commerce_prodnodelink_conf_line_item_view_prod_title_links'] = array(
    '#type' => 'radios',
    '#title' => t("Make 'Line Items' View product titles links to product?"),
    '#default_value' => variable_get('commerce_prodnodelink_conf_line_item_view_prod_title_links', 1),
    '#options' => $yes_no_options,
    '#description' => t("Choose whether or not product line items 'Title' links
      to Commerce product in the 'Line item' Views view output (line items shown
      when viewing an order at admin/commerce/orders/&lt;order_id&gt;)."),
  );

  $form['conf']['commerce_prodnodelink_conf_li_manager_prod_title_links'] = array(
    '#type' => 'radios',
    '#title' => t("Make line item manager product titles links to product?"),
    '#default_value' => variable_get('commerce_prodnodelink_conf_li_manager_prod_title_links', 1),
    '#options' => $yes_no_options,
    '#description' => t("Choose whether or not product line items 'Title' links
      to Commerce product in the line item manager (line items shown
      when editing an order at admin/commerce/orders/&lt;order_id&gt;/edit)."),
  );

  return system_settings_form($form);
}


/**
 * List of content types and product reference fields the module knows about.
 *
 * @return array
 *   Drupal render array.
 */
function commerce_prodnodelink_module_variables_page() {
  $page = array(
    'content' => array(
      'description' => array(
        '#prefix' => '<p>',
        '#suffix' => '</p>',
        '#markup' => t('Summary of content types and their product reference fields:'),
      ),
      'table' => array(
        '#theme' => 'table',
        '#header' => array(
          t('Content type (machine name)'),
          t('Product reference field'),
        ),
        '#rows' => array(),
        '#sticky' => TRUE,
        '#empty' => t('No data to summarise.'),
      ),
    ),
  );

  $node_types = commerce_prodnodelink_product_reference_field_by_node_type();

  foreach ($node_types as $type => $field_data) {
    for ($i = 0, $length = count($field_data); $i < $length; $i++) {
      $suffix = $type . '_' . $field_data[$i]['instance_id'];
      $field_name = variable_get('commerce_prodnodelink_data_' . $suffix, '');

      $page['content']['table']['#rows'][] = array(
        'data' => array(
          check_plain($type),
          $field_name,
        ),
      );
    }
  }

  return $page;
}
