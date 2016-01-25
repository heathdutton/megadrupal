<?php

/**
 * @file
 * Administrative page callbacks for the amocrm_widget_pages module.
 */

/**
 * Page callback for /admin/amocrm_widget/settings-link.
 */
function amocrm_widget_pages_link_settings_form($form, $form_state) {
  $form = array();
  $paths = variable_get('amocrm_widget_pages_link_paths', '');

  $form['amocrm_widget_links_path'] = array(
    '#type' => 'textarea',
    '#title' => t('Link widget paths'),
    '#description' => t('Enter one links info per line, in the form "Name|link/path|amocrm page (contacts|leads)".'),
    '#default_value' => $paths,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  $form['#submit'][] = 'amocrm_widget_pages_link_settings_form_submit';

  return $form;
}

/**
 * Submit handler for 'amocrm_widget_pages_link_settings_form' form.
 */
function amocrm_widget_pages_link_settings_form_submit($form, $form_state) {
  $paths = !empty($form_state['values']['amocrm_widget_links_path']) ? $form_state['values']['amocrm_widget_links_path'] : '';

  variable_set('amocrm_widget_pages_link_paths', $paths);
  drupal_set_message(t('Configuration saved.'));
}
