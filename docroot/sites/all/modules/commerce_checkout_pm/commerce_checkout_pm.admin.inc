<?php

/**
 * @file
 * Provides the Commerce Checkout Page Manager administrative interface.
 */

/**
 * Form builder for the admin display defaults page.
 */
function commerce_checkout_pm_form_settings() {
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  foreach (commerce_checkout_pages() as $page) {
    $form[$page['page_id']] = array(
      '#type' => 'fieldset',
      '#title' => $page['title'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'settings',
    );
    $form[$page['page_id']]['commerce_checkout_pm_' . $page['page_id'] . '_weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Weight'),
      '#default_value' => variable_get('commerce_checkout_pm_' . $page['page_id'] . '_weight', $page['weight']),
      '#element_validate' => array('element_validate_number'),
      '#required' => TRUE,
    );
    $form[$page['page_id']]['commerce_checkout_pm_' . $page['page_id'] . '_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => variable_get('commerce_checkout_pm_' . $page['page_id'] . '_title', $page['title']),
      '#required' => TRUE,
    );
    $form[$page['page_id']]['commerce_checkout_pm_' . $page['page_id'] . '_help'] = array(
      '#type' => 'textarea',
      '#title' => t('Help'),
      '#default_value' => variable_get('commerce_checkout_pm_' . $page['page_id'] . '_help', $page['help']),
    );
    $form[$page['page_id']]['commerce_checkout_pm_' . $page['page_id'] . '_submit_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Submit value'),
      '#default_value' => variable_get('commerce_checkout_pm_' . $page['page_id'] . '_submit_value', $page['submit_value']),
      '#required' => TRUE,
    );
    $form[$page['page_id']]['commerce_checkout_pm_' . $page['page_id'] . '_back'] = array(
      '#type' => 'checkbox',
      '#title' => t('Output back button.'),
      '#default_value' => variable_get('commerce_checkout_pm_' . $page['page_id'] . '_back', TRUE),
    );
  }

  return system_settings_form($form);
}
