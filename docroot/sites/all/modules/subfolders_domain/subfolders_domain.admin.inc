<?php

/**
 * @file
 * Content administration and module settings UI.
 */

/**
 * Subfolder domain form admin.
 */
function subfolders_domain_admin($form, &$form_state, $domain = array()) {
  drupal_set_message(t('With Subfolder Domain module it is strongly recommended to use one and only one language for each domain.'), 'warning');
  $domain_conf = array();
  if (isset($domain['domain_id'])) {
    $domain_conf = subfolders_domain_get_domain_conf($domain['domain_id']);
  }
  $form['#tree'] = TRUE;
  $form['subfolders_domain']['domain_id'] = array(
    '#title' => t('Domain id'),
    '#type' => 'textfield',
    '#default_value' => isset($domain['domain_id']) ? intval($domain['domain_id']) : 0,
    '#required' => TRUE,
    '#disabled' => TRUE,
    '#size' => 20,
  );

  $match_domain = explode('_', $domain['subdomain']);
  $form['subfolders_domain']['site'] = array(
    '#title' => t('Domain name'),
    '#type' => 'textfield',
    '#default_value' => end($match_domain),
    '#required' => TRUE,
    '#disabled' => TRUE,
  );

  $subfolders_domain_infos = subfolders_domain_infos(isset($domain['domain_id']) ? intval($domain['domain_id']) : NULL);
  $form['subfolders_domain']['other_domain'] = array(
    '#title' => t('Other Domain'),
    '#type' => 'textfield',
    '#default_value' => isset($subfolders_domain_infos->other_domain) ? $subfolders_domain_infos->other_domain : '',
    '#required' => FALSE,
    '#description' => t('Another domain name if this one is different of primary domain.'),
  );
  
  $languages = language_list();
  $options_languages = array();
  foreach ($languages as $lang => $value) {
    $options_languages[$lang] = $value->name;
  }
  $languages += array('domain-conf-ignore' => t('Use primary domain settings'));
  $language_default = isset($domain_conf['language_default']) ? $domain_conf['language_default'] : NULL;
  
  if (!empty($language_default) && isset($languages[$language_default])) {
    $options_languages = array('domain-conf-ignore' => t('Use primary domain settings')) + $options_languages;
    $disabled = TRUE;
  }
  else {
    $options_languages = array();
    $disabled = FALSE;
  }
  
  $domain_default = domain_default();
  $path_to_language_settings = $domain_default['scheme'] . '://' . $domain_default['subdomain'] . '/admin/structure/domain/view/'.$domain['domain_id'].'/config';
  $form['subfolders_domain']['language'] = array(
    '#title' => t('Language'),
    '#access' => module_exists('locale'),
    '#type' => 'select',
    '#options' => $options_languages,
    '#default_value' => !empty($language_default) && isset($languages[$language_default]) ? $language_default : FALSE,
    '#required' => TRUE,
    '#disabled' => $disabled,
    '#empty_option' => t('Choose the default language on the domain settings page'),  
    '#description' => t('<a href="@url">Set the default language for this domain.</a>', array('@url' => $path_to_language_settings)),
  );
  
  $parents = subfolders_domain_tree();
  unset($parents[$domain['domain_id']]);
  $form['subfolders_domain']['parent_id'] = array(
    '#title' => t('Parent site'),
    '#type' => 'select',
    '#default_value' => isset($subfolders_domain_infos->parent_id) ? $subfolders_domain_infos->parent_id : FALSE,
    '#options' => $parents,
    '#empty_option' => t('Choose a subdomain'),
    '#required' => FALSE,
  );

  $form['subfolders_domain']['inherit_parent_roles'] = array(
    '#title' => t('Users can inherit the roles of parent domain(s)?'),
    '#type' => 'radios',
    '#default_value' => isset($subfolders_domain_infos->inherit_parent_roles) ? (int) $subfolders_domain_infos->inherit_parent_roles : 0,
    '#options' => array((int) FALSE => t('No'), (int) TRUE => t('Yes')),
    '#attributes' => array('class' => array('')),
    '#required' => TRUE,
  );

  $form['domain_id'] = array(
    '#type' => 'hidden',
    '#value' => DOMAIN_SETTINGS_ALL_DOMAINS,
  );

  $form['#validate'][] = 'subfolders_domain_admin_validate';
  $form['#submit'][] = 'subfolders_domain_admin_submit';
  $form = system_settings_form($form);
  return $form;
}

/**
 * Process of validate domain setting form.
 */
function subfolders_domain_admin_validate($form, &$form_state) {
  if (isset($form_state['values']['subfolders_domain']['parent_id']) && (trim($form_state['values']['subfolders_domain']['parent_id']) != '' && (int) $form_state['values']['subfolders_domain']['parent_id'] <= 0)
          || (intval($form_state['values']['subfolders_domain']['parent_id']) === intval($form_state['values']['subfolders_domain']['domain_id']))) {
    form_set_error('subfolders_domain][parent_id', t('Invalid parent domain'));
  }
  if (isset($form_state['values']['subfolders_domain']['other_domain']) && trim($form_state['values']['subfolders_domain']['other_domain']) != '') {
    $check_domain = trim($form_state['values']['subfolders_domain']['other_domain']);
    $other_domain = db_query("SELECT * FROM {subfolders_domain} WHERE other_domain = :other_domain", array(':other_domain' => $check_domain))->fetchObject();
    if (isset($other_domain->domain_id) && (int) $other_domain->domain_id > 1) {
      form_set_error('t_domains][other_domain', t('@other_domain has already been taken.', array('@other_domain' => $check_domain)));
    }
  }
}

/**
 * Process result from domain setting form.
 */
function subfolders_domain_admin_submit($form, &$form_state) {
  if (!empty($form_state['values']) && isset($form_state['values']['subfolders_domain'])) {
    if (!empty($form_state['values']['subfolders_domain'])) {
      $form_state['values']['subfolders_domain']['changed'] = REQUEST_TIME;
      if (isset($form_state['values']['subfolders_domain']['parent_id'])) {
        $form_state['values']['subfolders_domain']['parent_id'] = (int) $form_state['values']['subfolders_domain']['parent_id'];
      }
      if (isset($form_state['values']['subfolders_domain']['inherit_parent_roles'])) {
        $form_state['values']['subfolders_domain']['inherit_parent_roles'] = (int) $form_state['values']['subfolders_domain']['inherit_parent_roles'];
      }

      db_delete('subfolders_domain')->condition('domain_id', intval($form_state['values']['subfolders_domain']['domain_id']))->execute();
      db_insert('subfolders_domain')->fields($form_state['values']['subfolders_domain'])->execute();
    }
  }
}

/**
 * Page callback for admin settings form.
 */
function subfolders_domain_roles_form($form, &$form_state, $account) {
  $form = array();
  $user_admin_role = (int) variable_get('user_admin_role', 0);
  $roles = user_roles();
  unset($roles[1]);
  $domains = array();
  $roles[0] = t('Users');
  ksort($roles);
  $defaults = variable_get('subfolders_domain_roles', array());
  $form['subfolders_domain_role'] = array(
    '#tree' => TRUE,
  );

  $form['domain_id'] = array(
    '#type' => 'hidden',
    '#value' => DOMAIN_SETTINGS_ALL_DOMAINS,
  );

  $domain_get_user_domains = domain_get_user_domains($account, FALSE);
  foreach (domain_domains() as $key => $value) {
    if ((int) $account->uid !== 1 && !isset($domain_get_user_domains[$key])) {
      continue;
    }
    $domains[$key] = $value['sitename'];
  }
  foreach ($roles as $rid => $role) {
    $form['subfolders_domain_role'][$rid]['#tree'] = TRUE;
    if ($rid == 0) {
      $form['subfolders_domain_role'][$rid]['all'] = array(
        '#title' => t('All domains'),
        '#type' => 'hidden',
        '#disabled' => TRUE,
        '#default_value' => $account->uid,
      );
    }
    else {
      $form['subfolders_domain_role'][$rid]['all'] = array(
        '#title' => t('All domains'),
        '#type' => 'checkbox',
        '#default_value' => isset($defaults[$account->uid]['all'][$rid]),
      );
      if (1 == (int) $account->uid || $rid == DRUPAL_AUTHENTICATED_RID || ($rid != 3 && isset($defaults[$account->uid]['all'][3]))) {
        $form['subfolders_domain_role'][$rid]['all']['#disabled'] = TRUE;
        $form['subfolders_domain_role'][$rid]['all']['#default_value'] = TRUE;
      }
    }
    foreach ($domains as $domain_id => $domain) {
      if ($rid == 0) {
        $form['subfolders_domain_role'][$rid][$domain_id] = array(
          '#title' => filter_xss_admin($domain),
          '#type' => 'hidden',
          '#disabled' => TRUE,
          '#default_value' => $account->uid,
        );
      }
      elseif (1 == (int) $account->uid  || (1 != (int) $account->uid && isset($account->domain_user[$domain_id]))) {
        $form['subfolders_domain_role'][$rid][$domain_id] = array(
          '#title' => filter_xss_admin($domain),
          '#type' => 'checkbox',
          '#default_value' => isset($defaults[$account->uid][$domain_id][$rid]),
        );
        $subfolders_domain_infos = subfolders_domain_infos($domain_id);
        if (isset($subfolders_domain_infos->inherit_parent_roles) && (int) $subfolders_domain_infos->inherit_parent_roles) {
          // Parent domains.
          $parent_domains = subfolders_domain_parent($domain_id);
          if (!empty($parent_domains)) {
            foreach ($parent_domains as $id_domain => $value) {
              if (isset($defaults[$account->uid][$id_domain]) && !empty($defaults[$account->uid][$id_domain]) &&
                      array_key_exists($rid, $defaults[$account->uid][$id_domain])) {
                $form['subfolders_domain_role'][$rid][$domain_id]['#disabled'] = TRUE;
                $form['subfolders_domain_role'][$rid][$domain_id]['#default_value'] = TRUE;
              }
            }
          }
          // END parent domains.
        }
        if (1 == (int) $account->uid || $rid == DRUPAL_AUTHENTICATED_RID || isset($defaults[$account->uid]['all'][$rid]) || isset($defaults[$account->uid]['all'][3])) {
          $form['subfolders_domain_role'][$rid][$domain_id]['#disabled'] = TRUE;
          $form['subfolders_domain_role'][$rid][$domain_id]['#default_value'] = TRUE;
        }
      }
      elseif (1 == (int) $account->uid  || (1 != (int) $account->uid && !isset($account->domain_user[$domain_id]))) {
        $form['subfolders_domain_role'][DRUPAL_AUTHENTICATED_RID][$domain_id] = array(
          '#title' => filter_xss_admin($domain),
          '#type' => 'checkbox',
          '#disabled' => TRUE,
          '#default_value' => TRUE,
        );
      }
      
      $user_admin_role = intval($user_admin_role);
      if ($user_admin_role == $rid && !user_access('affect admin role on a domain')) {
        $form['subfolders_domain_role'][$user_admin_role]['all']['#disabled'] = TRUE;
        $form['subfolders_domain_role'][$user_admin_role][$domain_id]['#disabled'] = TRUE;
      }
    }
    $form['subfolders_domain_role'][$rid]['all']['#access'] = user_access('affect role on all domains');
  }
  $form = system_settings_form($form);
  $form['#submit'][] = 'subfolders_domain_roles_form_submit';
  unset($form['#theme']);
  $form['#theme'] = 'subfolders_domain_roles_form';
  return $form;
}

/**
 * Custom Admin Setting Submit Callback.
 */
function subfolders_domain_roles_form_submit($form, &$form_state) {
  $uid = $form_state['values']['subfolders_domain_role'][0]['all'];
  $roles = user_roles();
  $custom_user_role = variable_get('subfolders_domain_roles', array());
  $custom_user_role[$uid] = array();

  $custom_user_role[$uid]['all'] = array();
  foreach ($roles as $rid => $role) {
    if (isset($form_state['values']['subfolders_domain_role'][$rid]['all']) && $form_state['values']['subfolders_domain_role'][$rid]['all'] === 1) {
      $custom_user_role[$uid]['all'] += array($rid => $role);
    }
  }
  foreach (domain_domains() as $domain_id => $value) {
    $custom_user_role[$uid][$domain_id] = array();
    foreach ($roles as $rid => $role) {
      if (isset($form_state['values']['subfolders_domain_role'][$rid][$domain_id]) && $form_state['values']['subfolders_domain_role'][$rid][$domain_id] === 1) {
        $custom_user_role[$uid][$domain_id] += array($rid => $role);
      }
    }
  }
  ksort($custom_user_role[$uid]);
  variable_set('subfolders_domain_roles', $custom_user_role);
}
