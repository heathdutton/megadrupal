<?php

/**
 * @file
 * This file implements the functionality of Domain finder configuration page.
 */

/**
 * This function generate admin form.
 */
function domain_finder_generate_admin_form($form, &$form_state) {
  $form = array();
  require_once drupal_get_path('module', 'domain_finder') . '/includes/domain_finder.domains.inc';

  $form['help'] = array(
    '#markup' => t('This configuration page provide the domain extension list.So please choose the domain extensions you want to find.'),
  );

  $form['domain_finder_domains_in_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show checkboxes on domain finder form'),
    '#default_value' => variable_get('domain_finder_domains_in_form', 0),
  );

  $available_domains = domain_finder_get_domains();
  $form['domain_finder_domains'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available domain extensions'),
    '#options' => drupal_map_assoc($available_domains['basic']['domains']),
    '#default_value' => variable_get('domain_finder_domains', array()),
  );

  return system_settings_form($form);
}
