<?php
/**
 * @file
 * Contains functions for Domain VBO User action.
 */

/**
 * domain_vbo_modify_user_domains: Configuration form.
 */
function domain_vbo_modify_user_domains_form($settings, &$form_state) {
  $form = array();

  $form['action'] = array(
    '#type' => 'radios',
    '#title' => t('Action'),
    '#options' => array(
      'replace' => t("Replace with"),
      'add' => t("Add"),
      'remove' => t("Remove"),
    ),
    '#description' => t("Choose the action to perform on each user"),
    '#required' => TRUE,
    '#default_value' => isset($settings['settings']['action']) ? $settings['settings']['action'] : 'replace',
  );
  $form['domains'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Modify domains for user'),
    '#options' => domain_vbo_domains(),
    '#description' => t("Choose domains to process"),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * domain_vbo_modify_user_domains: Configuration form submit.
 */
function domain_vbo_modify_user_domains_submit($form, $form_state) {
  $return = array();
  foreach ($form_state['values']['domains'] as $k => $v) {
    if ($v) {
      $return['domains'][$k] = $v;
    }
  }

  $return['action'] = $form_state['values']['action'];
  return $return;
}

/**
 * domain_vbo_modify_user_domains: Action callback.
 */
function domain_vbo_modify_user_domains(&$user, $context) {
  $action = $context['action'];
  $submitted_domains = $context['domains'];

  // This function is called for each entity (user).
  switch ($action) {
    case 'replace':
      $edit['domain_user'] = $submitted_domains;
      break;

    case 'add':
      // Get current user's domains and merge with submitted.
      $current_domains = domain_get_user_domains($user);

      // Just add the arrays with domains together.
      $combined_domains = $current_domains + $submitted_domains;

      $edit['domain_user'] = array_unique($combined_domains);
      break;

    case 'remove':
      // Get current user's domains and substract submitted.
      $current_domains = domain_get_user_domains($user);

      // Remove submitted domains from the current domains array.
      $subtracted_domains = array_diff_assoc($current_domains, $submitted_domains);
      $edit['domain_user'] = array_unique($subtracted_domains);
      break;
  }

  // Save new settings.
  domain_user_save($edit, $user);
}
