<?php

/**
 * Building settings form.
 */
function reasons_bounce_admin_settings_form($form, &$form_state, $node) {
  //Name settings
  $visibility = 'reasons_bounce_template_' . $node->nid . '_visibility';
  $access_pages = 'reasons_bounce_template_' . $node->nid . '_access_pages';
  $node_types = 'reasons_bounce_template_' . $node->nid . '_node_types';
  $roles = 'reasons_bounce_template_' . $node->nid . '_roles';
  $message = 'reasons_bounce_template_' . $node->nid . '_message';
  
  // Message settings.
  $form[$message] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => variable_get($message),
    '#rows' => 3,
    '#wysiwyg' => FALSE,
  );
  
  // Visibility settings.
  $form['visibility_title'] = array(
    '#type' => 'item',
    '#title' => t('Visibility settings'),
  );
  $form['visibility'] = array(
    '#type' => 'vertical_tabs',
  );
  
  // Path group
  $form['visibility']['path'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Pages'),
  );
  $form['visibility']['path'][$visibility] = array(
    '#type' => 'radios',
    '#title' => t('Show form on specific pages'),
    '#default_value' => variable_get($visibility, 0),
    '#options' => array(
      t('Show on every page except the listed pages.'), 
      t('Show on only the listed pages.')
    ),
  );

  $form['visibility']['path'][$access_pages] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get($access_pages),
    '#rows' => 3,
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are  %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#wysiwyg' => FALSE,
  );
  // if php_filter is enable
  $access = user_access('use PHP for settings');
  if ($form['visibility']['path'][$visibility]['#default_value'] == BOUNCE_FORM_PHP_FILTER && !$access) {
    $form['visibility']['path'][$visibility] = array(
      '#type' => 'value', 
      '#value' => BOUNCE_FORM_PHP_FILTER
    );
    $form['visibility']['path'][$access_pages] = array(
      '#type' => 'value', 
      '#value' => $form['visibility']['path'][$access_pages]['#default_value'],
    );
  }
  elseif ($access && module_exists('php')) {
    $form['visibility']['path'][$visibility]['#options'][] = 
      t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
    $form['visibility']['path'][$access_pages]['#description'] .= 
      ' ' . t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  
  //Node type group
  $form['visibility']['node_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 5,
  );
  $form['visibility']['node_type'][$node_types] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show form for specific content types'),
    '#options' => node_type_get_names(),
    '#description' => t('Show this form only on pages that display content of the given type(s). If you select no types, there will be no type-specific limitation.'),
    '#default_value' => variable_get($node_types, array()),  
  );
  
  // Role group
  $role_options = array_map('check_plain', user_roles());
  $form['visibility']['role'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 10,
  );
  $form['visibility']['role'][$roles] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show block for specific roles'),
    '#options' => $role_options,
    '#description' => t('Show this form only for the selected role(s). If you select no roles, the block will be visible to all users.'),
    '#default_value' => variable_get($roles, array()),
  );
  
  return system_settings_form($form);
}

/**
 * Submit callback.
 */
function reasons_bounce_node_form_submit($form, $form_state) {
  // menu_cache_clear() is not need, because node_form_submit use cache_clear_all().
  // @see reasonse_bounce_menu().
  $node = $form_state['node'];
  $is_enabled = $form_state['values']['bounce']['responce_bounce_enabled'];
  if ($is_enabled) {
    reasons_bounce_add_bounce_form($node->nid);
  }
  else {
    reasons_bounce_delete_bounce_form($node->nid);
  }
}

/**
 * Submit callback.
 */
function reasons_bounce_webform_submit($form, $form_state) {
  $webform_nid = $form['#node']->nid;
  $_SESSION['webform_client_form_'.$webform_nid] = 'submitted';
}