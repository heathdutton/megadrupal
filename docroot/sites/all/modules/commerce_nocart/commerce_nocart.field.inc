<?php
/**
 * @file
 * Contains the field formatter alter hooks of the commerce nocart module.
 */

/**
 * Implements hook_field_formatter_settings_form_alter().
 */
function commerce_nocart_field_formatter_settings_form_alter(array &$settings_form, array $context) {
  $field = $context['field'];
  $view_mode = $context['view_mode'];
  $display = $context['instance']['display'];
  $settings = $display[$view_mode]['settings'];

  $options = array();
  foreach (commerce_checkout_pages() as $page_id => $page) {
    if (empty($page['status_cart'])) {
      $options[$page_id] = $page['name'];
    }
  }

  if ($display[$view_mode]['type'] === 'commerce_cart_add_to_cart_form') {
    $settings_form['commerce_nocart']['#tree'] = TRUE;
    $settings_form['commerce_nocart']['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Direct checkout'),
      '#default_value' => $settings['commerce_nocart']['enabled'],
    );
    $settings_form['commerce_nocart']['button_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Button text'),
      '#default_value' => $settings['commerce_nocart']['button_text'],
      '#states' => array(
        'visible' => array(
          'input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][commerce_nocart][enabled]"], input[name="options[settings][commerce_nocart][enabled]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $settings_form['commerce_nocart']['checkout_page'] = array(
      '#type' => 'select',
      '#title' => t('Checkout page'),
      '#default_value' => $settings['commerce_nocart']['checkout_page'],
      '#options' => $options,
      '#states' => array(
        'visible' => array(
          'input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][commerce_nocart][enabled]"], input[name="options[settings][commerce_nocart][enabled]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }
}

/**
 * Implements hook_field_formatter_settings_summary_alter().
 */
function commerce_nocart_field_formatter_settings_summary_alter(&$summary, array $context) {
  $view_mode = $context['view_mode'];
  $display = $context['instance']['display'];
  $settings = $display[$view_mode]['settings'];

  if (!empty($settings['commerce_nocart']['enabled'])) {
    $summary .= '<br />' . t('Perform direct checkout, use button name %button_text', array('%button_text' => $settings['commerce_nocart']['button_text']));
  }
}
