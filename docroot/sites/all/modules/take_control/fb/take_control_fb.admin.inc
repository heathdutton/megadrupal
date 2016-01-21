<?php


include_once (drupal_get_path('module', 'take_control') . '/includes/security.inc');
include_once (drupal_get_path('module', 'take_control') . '/includes/filesystem.inc');

/**
 * take control admin configuration form.
 */
function take_control_fb_settings_form($form, $form_state) {
  $extvers = take_control_extjs_dirs();

  $form['take_control_fb_extjs_ver'] = array(
      '#type' => 'select',
      '#options' => drupal_map_assoc($extvers),
      '#default_value' => variable_get('take_control_fb_extjs_ver', ''),
      '#required' => TRUE,
      '#title' => t('ExtJs version directory'),
      '#description' => t('ExtJs version directory (in /sites/all/modules) that should be used by the File Browser module.'));

  $form['take_control_fb_num_browser_blocks'] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get('take_control_fb_num_browser_blocks', '1'),
      '#required' => TRUE,
      '#title' => t('Number of File Browser blocks'),
      '#description' => t('Number of File Browser blocks to make available on Block Administration page.'));

  $form['take_control_fb_enable_demo_mode'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('take_control_fb_enable_demo_mode', FALSE),
      '#title' => t('Enable demo mode'),
      '#description' => t('When demo mode is enabled, all edit/delete functionality for the browser is disabled on the server.'));

  $form['take_control_fb_roles_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Accessible paths per role'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE);

  $form['take_control_fb_roles_fieldset']['label1'] = array(
      '#type' => 'markup',
      '#markup' => t('<div>If a user has multiple roles, the sort order of the roles at <i>!link</i> will be used to determine the base accessible path for the user.</div>', array('!link' => l('admin/people/permissions/roles', 'admin/people/permissions/roles'))));
  $form['take_control_fb_roles_fieldset']['label2'] = array(
      '#type' => 'markup',
      '#markup' => t('<div>Roles with higher weight get higher precedence when deciding a user\'s root directory for file browser access.<div>'));
  $form['take_control_fb_roles_fieldset']['label3'] = array(
      '#type' => 'markup',
      '#markup' => t('<div>If accessible path for a role is empty below, the users having that role can access the entire Drupal file-system.<div>'));
  $form['take_control_fb_roles_fieldset']['label4'] = array(
      '#type' => 'markup',
      '#markup' => t('<div>The path should be relative to Drupal\'s root and should <b>not</b> contain leading or trailing slashes.<div>'));
	  
  foreach(_take_control_fb_get_allowed_roles() as $rid => $name) {
	$form['take_control_fb_roles_fieldset']['take_control_fb_role_path' . $rid] = array(
	  '#type' => 'textfield',
	  '#default_value' => variable_get('take_control_fb_role_path' . $rid, ''),
	  '#title' => $name);
  }

  $form['take_control_fb_roles_fieldset']['token_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => theme('token_tree', array('token_types' => array('current-user'))),
    '#weight' => 10,
  );

  $form['#validate'][] = 'take_control_fb_settings_validate_handler';
  $form['#submit'][] = 'take_control_fb_settings_submit_handler';

  return system_settings_form($form);
}

function take_control_fb_settings_validate_handler($form, &$form_state) {
	$numBlocks = $form_state['values']['take_control_fb_num_browser_blocks'];
	
	if(!is_numeric($numBlocks)) {
		form_set_error('take_control_fb_num_browser_blocks', t('Number of blocks should be an integral value.'));
	}
	else if (intval($numBlocks) < 1) {
		form_set_error('take_control_fb_num_browser_blocks', t('Minimum number of blocks should be 1.'));
	}

  foreach(_take_control_fb_get_allowed_roles() as $rid => $name) {
	$id = 'take_control_fb_role_path' . $rid;
	if (strpos($form_state['values'][$id], '..') !== false) {
	  form_set_error($id, t('Path cannot contain ".."'));
    }
  }
}

function take_control_fb_settings_submit_handler($form, &$form_state) {
  variable_set('take_control_fb_extjs_ver', $form_state['values']['take_control_fb_extjs_ver']);
  variable_set('take_control_fb_num_browser_blocks', $form_state['values']['take_control_fb_num_browser_blocks']);
  variable_set('take_control_fb_enable_demo_mode', $form_state['values']['take_control_fb_enable_demo_mode']);

  foreach(_take_control_fb_get_allowed_roles() as $rid => $name) {
	$id = 'take_control_fb_role_path' . $rid;
    variable_set($id, $form_state['values'][$id]);
  }

  //Need to clear caches so changes in number of blocks is recognized.
  cache_clear_all();
}
