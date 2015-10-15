<?php

/**
 * The main administration form for DocCheck.
 */
function doccheck_admin_form($form, &$form_state) {
  $form['#prefix'] = '<div id="doccheck-admin-form">';
  $form['#suffix'] = '</div>';

  module_load_include('data.inc', 'doccheck');
  $doccheck_languages = doccheck_get_languages();

  $form['doccheck_language'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Language'),
    '#options' => $doccheck_languages,
    '#default_value' => variable_get('doccheck_language', array()),
  );

  $form['domains'] = array(
    '#type' => 'container',
    '#theme_wrappers' => array(
      'doccheck_form_table',
    ),
  );

  $form['domains']['header'] = array(
    '#type' => 'container',
    '#theme_wrappers' => array(
      'doccheck_form_tr',
    ),
  );

  $form['domains']['header'][] = array(
    '#type' => 'markup',
    '#theme_wrappers' => array(
      'doccheck_form_th',
    ),
    '#markup' => t('Domains'),
  );

  $form['domains']['header'][] = array(
    '#type' => 'markup',
    '#theme_wrappers' => array(
      'doccheck_form_th',
    ),
    '#markup' => '&nbsp;',
  );

  if (!isset($form_state['domains'])) {
    $form_state['domains'] = variable_get('doccheck_domains', array());
  }

  foreach ($form_state['domains'] as $domain) {
    $form['domains'][] = array(
      '#type' => 'container',
      '#theme_wrappers' => array(
        'doccheck_form_tr',
      ),
      'domain' => array(
        '#type' => 'markup',
        '#markup' => check_plain($domain),
        '#theme_wrappers' => array(
          'doccheck_form_td',
        ),
      ),
      'remove' => array(
        '#type' => 'submit',
        '#value' => t('Remove'),
        '#domain' => $domain,
        '#theme_wrappers' => array(
          'button',
          'doccheck_form_td',
        ),
        '#submit' => array(
          '_doccheck_admin_form_domain_remove_submit',
        ),
        '#ajax' => array(
          'callback' => '_doccheck_mapping_ajax',
          'wrapper' => 'doccheck-admin-form',
        ),
      ),
    );
  }

  $form['adddomain'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add domain'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['adddomain']['domainname'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
  );

  $form['adddomain']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array(
      '_doccheck_admin_form_domain_add_submit',
    ),
    '#ajax' => array(
      'callback' => '_doccheck_mapping_ajax',
      'wrapper' => 'doccheck-admin-form',
    ),
  );

  $form['block_template'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block template'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['block_template'] +=
    _doccheck_admin_form_get_template_settings('block');

  $form['page_template'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page template'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['page_template'] +=
    _doccheck_admin_form_get_template_settings('page');

  $form['doccheck_authentication_method'] = array(
    '#type' => 'radios',
    '#title' => t('Authentication method'),
    '#options' => array(
      'uniquekey' => t('Unique key'),
      'profession' => t('Profession and activity'),
    ),
    '#default_value' => variable_get('doccheck_authentication_method', 'uniquekey'),
  );

  $form['doccheck_shared_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#default_value' => variable_get('doccheck_shared_secret', ''),
  );

  $form['doccheck_hide_dc_profile_fields'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide DocCheck profile fields on the profile edit form.'),
    '#default_value' => variable_get('doccheck_hide_dc_profile_fields', FALSE),
  );

  $form['doccheck_force_language'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force language settings'),
    '#default_value' => variable_get('doccheck_force_language', FALSE),
  );

  $form['doccheck_disable_normal_registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable normal registration'),
    '#default_value' => variable_get('doccheck_disable_normal_registration', FALSE),
  );

  $form['doccheck_log_get_array'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debug logging for DocCheck authentication.'),
    '#default_value' => variable_get('doccheck_log_get_array', FALSE),
  );

  $form['doccheck_force_mapping_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force mapping roles on user load'),
    '#default_value' => variable_get('doccheck_force_mapping_roles'),
  );

  $form = system_settings_form($form);

  array_unshift($form['#submit'], 'doccheck_admin_form_domains_submit');
  $form['#submit'][] = 'doccheck_admin_form_submit';

  return $form;
}

function _doccheck_admin_form_domain_remove_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $key = array_search($form_state['triggering_element']['#domain'], $form_state['domains'], TRUE);
  unset($form_state['domains'][$key]);
}

function _doccheck_admin_form_domain_add_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['domains'][] = $form_state['values']['domainname'];
}

function doccheck_admin_form_domains_submit($form, &$form_state) {
  unset($form_state['values']['domainname']);
  $form_state['values']['doccheck_domains'] = $form_state['domains'];
}

function doccheck_admin_form_submit($form, &$form_state) {
  menu_rebuild();
}

/**
 * Generates form controls for the template settings.
 *
 * @param string $type
 *   Template type
 *
 * @return array
 *   Template form items.
 */
function _doccheck_admin_form_get_template_settings($type) {
  return array(
    "doccheck_{$type}_template_size" => array(
      '#type' => 'select',
      '#title' => t('Size'),
      '#options' => drupal_map_assoc(array('S', 'M', 'L', 'XL')),
      '#default_value' => variable_get("doccheck_{$type}_template_size", NULL),
    ),
    "doccheck_{$type}_template_name" => array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#default_value' => variable_get("doccheck_{$type}_template_name", NULL),
    ),
  );
}
