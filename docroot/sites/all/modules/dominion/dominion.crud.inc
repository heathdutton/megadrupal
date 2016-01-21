<?php

/**
 * @file
 * CRUD file for Dominion.
 */

/**
 * Form to deploy a new subsite or edit an existing subsite.
 */
function dominion_form($form, &$form_state, $domain_id = NULL, $blueprint = NULL) {
  $form = array();

  // This is an edit form, store data in form
  if (!empty($domain_id)) {
    $form['#domain_id'] = $domain_id;
    if (!$subsite = dominion_get_info($domain_id)) {
      drupal_not_found();
    }
    $blueprint = dominion_blueprint_load($subsite->blueprint);
  }
  else {
    $blueprint = dominion_blueprint_load($blueprint);
  }
  if (!$blueprint) {
    // If the blueprint was not found, load the default blueprint.
    $blueprint = dominion_blueprint_load();
  }

  $form['#blueprint'] = $blueprint->name;

  if (!dominion_subsite_edit_access($domain_id)) {
    drupal_set_message(t('You do not have permission to edit all options. Some fields may be locked.'), 'warning');
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#default_value' => !is_null($domain_id) ? $subsite->sitename : '',
    '#description' => t('Name to use for this subsite.'),
    '#disabled' => !dominion_subsite_edit_access($domain_id),
  );

  if (!empty($domain_id) && ($domain = domain_load($domain_id)) && $domain['is_default']) {
    // Do not allow changing of domain name for default domain.
    $form['domain_type'] = array(
      '#type' => 'hidden',
      '#value' => 'domain',
    );
    $form['domain'] = array(
      '#type' => 'hidden',
      '#value' => $subsite->subdomain,
    );
  }
  else {
    $domain_suffix = variable_get(DOMINION_VAR_DOMAIN_SUFFIX, DOMINION_VAR_DOMAIN_SUFFIX_DEFAULT);
    $allow_tld = $blueprint->allow_tld;

    $options = array();
    $options['subdomain'] = t('Use a subdomain of %domain', array('%domain' => $domain_suffix));
    if ($allow_tld) {
      $options['domain'] = t('Use a custom domainname');
    }
    $options['directory'] = t('Use a directory');

    $default_value = variable_get(DOMINION_VAR_DEFAULT_DOMAIN_TYPE, 'subdomain');
    if (!isset($options[$default_value])) {
      // Fall back to subdomain when the configured default value is not available.
      $default_value = 'subdomain';
    }

    $form['domain_type'] = array(
      '#type' => 'radios',
      '#title' => t('Domain type'),
      '#options' => $options,
      '#default_value' => !is_null($domain_id) ? $subsite->domain_type : $default_value,
      '#ajax' => array(
        'callback' => 'dominion_form_domain_type_callback',
        'wrapper' => 'domain-type-fieldset-div',
        'method' => 'html',
        'effect' => 'fade',
        'progress' => array(
          'type' => 'throbber',
          'message' => '',
        )
      ),
      '#disabled' => !dominion_subsite_edit_access($domain_id),
    );

    $form['domain_type_fieldset'] = array(
      '#prefix' => '<div id="domain-type-fieldset-div">',
      '#suffix' => '</div>',
    );
    $type = isset($form_state['values']['domain_type']) ? $form_state['values']['domain_type'] : $form['domain_type']['#default_value'];
    switch ($type) {
      case 'subdomain':
        $domain_suffix = variable_get(DOMINION_VAR_DOMAIN_SUFFIX, DOMINION_VAR_DOMAIN_SUFFIX_DEFAULT);
        $regex = '/' . preg_quote($domain_suffix) . '$/';
        $subdomain = isset($subsite) ? preg_replace($regex, '', $subsite->subdomain) : '';

        $form['domain_type_fieldset']['subdomain'] = array(
          '#type' => 'textfield',
          '#title' => t('Subdomain'),
          '#required' => $allow_tld || (!is_null($domain_id) && $subsite->domain_type == 'domain') ? FALSE : TRUE,
          '#size' => 40,
          '#default_value' => !is_null($domain_id) ? $subsite->domain_type == 'subdomain' ? $subdomain : '' : '',
          '#description' => t('Part before %domain, i.e. "@example_subdomain" for %example_domain.', array(
            '%domain' => $domain_suffix,
            '@example_subdomain' => t('blog'),
            '%example_domain' => t('blog') . $domain_suffix,
          )),
          '#required' => TRUE,
          '#disabled' => !dominion_subsite_edit_access($domain_id),
        );
        break;
      case 'domain':
        $form['domain_type_fieldset']['domain'] = array(
          '#type' => 'textfield',
          '#title' => t('Domainname'),
          '#size' => 40,
          '#default_value' => !is_null($domain_id) ? $subsite->domain_type == 'domain' ? $subsite->subdomain : '' : '',
          '#description' => t('The full domainname, i.e.: "example.com"'),
          '#required' => TRUE,
          '#disabled' => !dominion_subsite_edit_access($domain_id),
        );
        break;
      case 'directory':
        $options = db_select('domain', 'd')
          ->fields('d', array('subdomain'))
          ->isNull('d.dominion_directory')
          ->orderBy('d.is_default', 'desc')
          ->orderBy('d.subdomain', 'asc')
          ->execute()
          ->fetchCol();
        $form['domain_type_fieldset']['base_domain'] = array(
          '#type' => 'select',
          '#title' => t('Domain'),
          '#options' => drupal_map_assoc($options),
          '#default_value' => !is_null($domain_id) ? $subsite->subdomain : '',
          '#required' => TRUE,
          '#disabled' => !dominion_subsite_edit_access($domain_id),
        );
        $form['domain_type_fieldset']['directory'] = array(
          '#type' => 'textfield',
          '#title' => t('Directory'),
          '#size' => 40,
          '#default_value' => !is_null($domain_id) ? $subsite->domain_type == 'directory' ? $subsite->directory : '' : '',
          '#description' => t('Provide directory without leading or trailing slashes, i.e. "press".'),
          '#required' => TRUE,
          '#disabled' => !dominion_subsite_edit_access($domain_id),
        );
        break;
    }
  }

  if ($blueprint->allow_custom_frontpage) {
    $form['frontpage'] = array(
      '#type' => 'textfield',
      '#title' => t('Frontpage'),
      '#default_value' => !is_null($domain_id) ? $subsite->frontpage : $blueprint->default_frontpage,
      '#size' => 40,
      '#description' => t('System path of the frontpage to use for this subsite. Leave empty to use site default.'),
      '#disabled' => !dominion_subsite_edit_access($domain_id),
    );
  }

  if ($blueprint->allow_functions_override && ($options = dominion_get_all_functions())) {
    $form['functions'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Available functions'),
      '#default_value' => !is_null($domain_id) ? drupal_map_assoc($subsite->available_functions) : drupal_map_assoc($blueprint->default_functions),
      '#options' => $options,
      '#disabled' => !dominion_subsite_edit_access($domain_id),
    );
  }

  if (user_access('administer site configuration')) {
    $form['locked'] = array(
      '#type' => 'checkbox',
      '#title' => t('Lock subsite'),
      '#default_value' => is_null($domain_id) ? FALSE : $subsite->locked,
      '#description' => t('Disallow editors to edit or delete this subsite.'),
    );
  }
  else {
    $form['locked'] = array(
      '#type' => 'hidden',
      '#value' => is_null($domain_id) ? FALSE : $subsite->locked,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 50,
  );

  return $form;
}

/**
 * Dominion form callback for type selection.
 */
function dominion_form_domain_type_callback($form, &$form_state) {
  return $form['domain_type_fieldset'];
}


/**
 * Form validate for dominion_form.
 */
function dominion_form_validate($form, &$form_state) {
  $domain_id = isset($form['#domain_id']) ? $form['#domain_id'] : -1;

  // Get hostname for the subdomain, we call this "subdomain" to follow the naming of the domain module.
  $domain_type = !empty($form_state['values']['domain_type']) ? $form_state['values']['domain_type'] : 'subdomain';
  $domain_suffix = variable_get(DOMINION_VAR_DOMAIN_SUFFIX, DOMINION_VAR_DOMAIN_SUFFIX_DEFAULT);
  switch ($domain_type) {
    case 'subdomain':
      $subdomain = $form_state['values']['subdomain'] . $domain_suffix;
      break;
    case 'directory':
      $subdomain = $form_state['values']['base_domain'];
      break;
    case 'domain':
      $subdomain = $form_state['values']['domain'];
      break;
  }

  if ($domain_type == 'directory') {
    if (!preg_match('/^[a-z0-9\\-\\_\\.\\/]+$/si', $form_state['values']['directory'])) {
      form_set_error($domain_type, t('The specified directory is invalid.'));
    }
  }
  else {
    if (!preg_match('/^[a-z0-9\\-\\_\\.]+(\\:[0-9]+)?$/', $subdomain)) {
      form_set_error($domain_type, t('The specified domain is invalid.'));
    }
    elseif (_dominion_domain_exists($subdomain, $domain_id)) {
      form_set_error($domain_type, t('The specified domain is already in use.'));
    }
  }
}


/**
 * Form submit for dominion_form.
 */
function dominion_form_submit($form, &$form_state) {
  if (!empty($form_state['clicked_button']['#ajax'])) {
    $form_state['rebuild'] = TRUE;
    return;
  }

  $name = $form_state['values']['name'];
  $blueprint = dominion_blueprint_load($form['#blueprint']);

  // Get hostname for the subdomain, we call this "subdomain" to follow the naming of the domain module.
  $domain_type = isset($form_state['values']['domain_type']) ? $form_state['values']['domain_type'] : 'subdomain';
  $domain_suffix = variable_get(DOMINION_VAR_DOMAIN_SUFFIX, DOMINION_VAR_DOMAIN_SUFFIX_DEFAULT);
  $directory = NULL;
  switch ($domain_type) {
    case 'subdomain':
      $subdomain = $form_state['values']['subdomain'] . $domain_suffix;
      break;
    case 'directory':
      $subdomain = $form_state['values']['base_domain'];
      $directory = $form_state['values']['directory'];
      break;
    case 'domain':
      $subdomain = $form_state['values']['domain'];
      break;
  }

  $frontpage = empty($form_state['values']['frontpage']) ? $blueprint->default_frontpage : $form_state['values']['frontpage'];
  $locked = !empty($form_state['values']['locked']);

  if (!isset($form['#domain_id'])) {
    // Call functions which actually creates the subsite.
    if ($domain_id = _dominion_create_domain($subdomain, $name, $directory, $blueprint->blueprint, $locked)) {
      domain_conf_variable_set($domain_id, 'site_frontpage', $frontpage);
    }
    else {
      drupal_set_message(t('Error while creating new domain. The new subsite was not created.'), 'error');
      return;
    }
  }
  else {
    $domain_id = $form['#domain_id'];
    $subsite = dominion_get_info($domain_id);
    if ($subsite->directory != $directory) {
      _dominion_change_directory($subsite->directory, $directory);
    }
    _dominion_update_domain($domain_id, $subdomain, $name, $directory, $locked);
    if (!empty($form_state['values']['frontpage'])) {
      // Only overwrite the alias if it is not set automatically.
      // Otherwise we are overwriting the node path when a node is set as the frontpage.
      domain_conf_variable_set($domain_id, 'site_frontpage', $frontpage);
    }
  }

  // If option is on for creating a new node.
  if (!isset($form['#domain_id'])) {
    // Only create frontpage node when creating a new domain.
    if ($frontpage_nodetype = $blueprint->frontpage_nodetype) {
      if ($frontpage == $blueprint->default_frontpage) {
        $frontpage_nid = dominion_save_node($domain_id, $frontpage_nodetype, $name);
        $frontpage = "node/$frontpage_nid";
        domain_conf_variable_set($domain_id, 'site_frontpage', $frontpage);
      }
    }
  }

  // Update dominion functions.
  db_delete('dominion_function')
    ->condition('domain_id', $domain_id)
    ->execute();
  if (!empty($form_state['values']['functions'])) {
    foreach (array_filter($form_state['values']['functions']) as $function) {
      db_insert('dominion_function')->fields(array(
          'domain_id' => $domain_id,
          'name' => $function,
        ))->execute();
    }
  }

  dominion_form_set_redirect('admin/structure/dominion');

  if (!empty($form['#domain_id'])) {
    module_invoke_all('dominion_update', $domain_id, $form_state['values']);
  }
  else {
    module_invoke_all('dominion_insert', $domain_id, $form_state['values']);
  }

  cache_clear_all('domain_id:' . $domain_id, 'cache_dominion');

  if (isset($form['#domain_id'])) {
    drupal_set_message(t('The changes to %name were saved succesfully.', array('%name' => $name)));
  }
  else {
    drupal_set_message(t('The subdomain %name was created succesfully.', array('%name' => $name)));
  }

  if ($custom_redirect = dominion_form_set_redirect()) {
    $form_state['redirect'] = $custom_redirect;
  }
}

/**
 * Update dominion directory in body texts.
 *
 * @param string $old
 * @param string $new
 */
function _dominion_change_directory($old, $new) {
  $replace = array(
    'href="/' . $old => 'href="/' . $new,
    'href="//' . check_plain($_SERVER['HTTP_HOST']) . '/' . $old => 'href="//' . check_plain($_SERVER['HTTP_HOST']) . '/' . $new,
    'href="http://' . check_plain($_SERVER['HTTP_HOST']) . '/' . $old => 'href="http://' . check_plain($_SERVER['HTTP_HOST']) . '/' . $new,
    'href="https://' . check_plain($_SERVER['HTTP_HOST']) . '/' . $old => 'href="https://' . check_plain($_SERVER['HTTP_HOST']) . '/' . $new,
  );
  foreach ($replace as $search => $replacement) {
    $sql = 'UPDATE field_data_body SET body_value = REPLACE(body_value, :search, :replacement)';
    db_query($sql, array(':search' => $search, ':replacement' => $replacement));
    $sql = 'UPDATE field_revision_body SET body_value = REPLACE(body_value, :search, :replacement)';
    db_query($sql, array(':search' => $search, ':replacement' => $replacement));
  }
  cache_clear_all('*', 'cache_filter', TRUE);
}

/**
 * Set redirect after form submit.
 */
function dominion_form_set_redirect($dst = NULL) {
  static $redir = NULL;
  !$dst or $redir = $dst;
  return $redir;
}

/**
 * Form to delete a subsite or edit an existing subsite.
 */
function dominion_delete_form($form, &$form_state, $domain_id = NULL) {
  global $_domain;

  $form['#domain_id'] = $domain_id;
  if (!$subsite = dominion_get_info($domain_id)) {
    drupal_not_found();
    exit;
  }

  if ($_domain['domain_id'] == $domain_id) {
    $form['info'] = array(
      '#markup' => '<p>' . t('You cannot delete a subsite from the same domain. Please delete the subsite from the main domain.') . '</p>',
    );
  }
  else {
    $form['info'] = array(
      '#markup' => '<p>' . t('Are you sure you want to delete the subsite %name and all of its contents?', array(
        '%name' => $subsite->sitename
    )) . '</p>',
    );

    $form['buttons'] = array(
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );

    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Yes'),
    );

    $form['buttons']['cancel'] = array(
      '#markup' => l(t('cancel'), 'admin/structure/dominion'),
    );
  }

  return $form;
}


/**
 * Submit handler for dominion_delete_form().
 */
function dominion_delete_form_submit($form, &$form_state) {
  $domain_id = $form['#domain_id'];

  if (!$subsite = dominion_get_info($domain_id)) {
    drupal_not_found();
    return;
  }

  // Delete the domain.
  module_load_include('inc', 'dominion', 'dominion.delete');
  _dominion_delete_subsite($domain_id);

  // Hide the message from the Dominion User module.
  drupal_set_message(t('The subsite %name has been deleted.', array('%name' => $subsite->sitename)));
  $form_state['redirect'] = 'admin/structure/dominion';
}
