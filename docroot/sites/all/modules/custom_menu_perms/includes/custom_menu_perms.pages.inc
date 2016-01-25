<?php

/**
 * Page callback for creating new permissions that can be used on the site
 */
function custom_menu_perms_add_perm()
{
	$breadcrumb = drupal_get_breadcrumb();

	$breadcrumb[] = l(t('Administration'), 'admin');
	$breadcrumb[] = l(t('Configuration'), 'admin/config');
	$breadcrumb[] = l(t('System'), 'admin/config/system');
	$breadcrumb[] = l(t('Custom menu permissions'), 'admin/config/system/custom_menu_perms');
	drupal_set_breadcrumb($breadcrumb);

	$permission = new StdClass;
	$permission->perm_key = FALSE;
	$permission->perm_name = '';
	$permission->description = '';

	return drupal_get_form('custom_menu_perms_perm_form', $permission);
}

function custom_menu_perms_perm_edit($permission)
{
	$breadcrumb = drupal_get_breadcrumb();

	$breadcrumb[] = l(t('Administration'), 'admin');
	$breadcrumb[] = l(t('Configuration'), 'admin/config');
	$breadcrumb[] = l(t('System'), 'admin/config/system');
	$breadcrumb[] = l(t('Custom menu permissions'), 'admin/config/system/custom_menu_perms');
	drupal_set_breadcrumb($breadcrumb);

	return drupal_get_form('custom_menu_perms_perm_form', $permission);
}

/**
 * Form callback allowing users to create/edit custom permissions that can be used on the site
 *
 * @see custom_menu_perms_perm_form_submit()
 */
function custom_menu_perms_perm_form($form, &$form_state, $permission)
{
	$form['permission'] = array
	(
		'#type' => 'value',
		'#value' => $permission,
	);
	$form['perm_name'] = array
	(
		'#type' => 'textfield',
		'#title' => t('Permission name'),
		'#required' => TRUE,
		'#default_value' => $permission->perm_name,
		'#description' => t('Enter the human-readable name of the permission'),
		'#maxlength' => 255,
		'#size' => 40,
	);
	$form['perm_key'] = array
	(
		'#type' => 'machine_name',
		'#title' => t('Machine name'),
		'#default_value' => $permission->perm_key,
		'#maxlength' => 255,
		'#description' => t('The machine name for the permission'),
		'#required' => TRUE,
		'#machine_name' => array
		(
			'exists' => 'custom_menu_perm_perm_exists',
			'source' => array('perm_name'),
			'label' => t('Permission machine name'),
			'replace_pattern' => '[^a-z0-9_]+',
			'replace' => '_',
		),
		'#disabled' => strlen($permission->perm_key),
	);
	$form['description'] = array
	(
		'#type' => 'textarea',
		'#title' => t('Description'),
		'#default_value' => $permission->description,
		'#description' => t('Enter the description of the permission. This will be shown on the Permissions page'),
	);

	$form['save_permission'] = array
	(
		'#type' => 'submit',
		'#value' => t('Save permission'),
	);

	return $form;
}

/**
 * Form submission callback allowing users to create custom permissions that can be used on the site
 *
 * @see custom_menu_perms_perm_form()
 */
function custom_menu_perms_perm_form_submit($form, &$form_state, $permission = FALSE)
{
	$perm = $form_state['values']['permission'];
	$perm->perm_name = $form_state['values']['perm_name'];
	$perm->description = $form_state['values']['description'];
	if($perm->perm_key)
	{
		drupal_write_record('cmp_permissions', $perm, 'perm_key');
	}
	else
	{
		$perm->perm_key = $form_state['values']['perm_key'];
		drupal_write_record('cmp_permissions', $perm);
	}
	$form_state['redirect'] = 'admin/config/system/custom_menu_perms/permissions';
}

function custom_menu_perms_alter_menu_perms_form($form, &$form_state)
{
	$options = array('---' => t('Default'));
	$custom_menu_perms = custom_menu_perms_permission();
	foreach($custom_menu_perms as $key => $value)
	{
		if(!in_array($key, array('administer_custom_menu_perms', 'create_custom_perms')))
		{
			$options[t('Custom Permissions')][$key] = strip_tags($value['title']);
		}
	}

	$module_info = system_get_info('module');
	$modules = array();
	foreach (module_implements('permission') as $module)
	{
		$modules[$module] = $module_info[$module]['name'];
	}
	asort($modules);

	foreach($modules as $module => $display_name)
	{
		if($perms = module_invoke($module, 'permission'))
		{
			foreach($perms as $key => $value)
			{
				if($module != 'custom_menu_perms' || in_array($key, array('administer_custom_menu_perms', 'create_custom_perms')))
				{
					$options[$display_name][$key] = strip_tags($value['title']);
				}
			}
		}
	}

	$menu = module_invoke_all('menu');

	$saved_permissions = array();
	$altered_perms = db_query
	(
		'SELECT menu_path, cmp_permission_key FROM {cmp_menu_perms}'
	);
	foreach($altered_perms as $ap)
	{
		$saved_permissions[$ap->menu_path] = $ap->cmp_permission_key;
	}

	$form['header'] = array
	(
		'#markup' => t('You can change the access callback of any menu items below. Please note that many items do not use user_access() as a menu callback. Changing the access callback of a menu item on this page will cause that page to use user_access() as a menu callback, with the selected permission as the value to be passed to user_access(). This may have unexpected behaviors, so please test those menu items carefully after changing access. To reset an item back to its original access callback, select "Set to default" for that item, and save this form'),
		'#prefix' => '<p>',
		'#suffix' => '</p>',
	);

	$form['perms_table'] = array
	(
		'#type' => 'container',
		'#theme' => 'custom_menu_perms_perm_table',
		'#tree' => TRUE,
	);
	foreach($menu as $path => $item)
	{
		if(isset($item['access arguments']) && (!isset($item['access callback']) || (isset($item['access callback']) && $item['access callback'] == 'user_access')))
		{
			$form['perms_table'][$path]['permission'] = array
			(
				'#type' => 'select',
				'#options' => $options,
				'#default_value' => isset($saved_permissions[$path]) ? $saved_permissions[$path] : '---',
			);
			$form['perms_table'][$path]['original'] = array
			(
				'#markup' =>  $item['access arguments'][0],
			);
		}
		else
		{
			$form['perms_table'][$path]['permission'] = array
			(
				'#type' => 'select',
				'#options' => $options,
				'#default_value' => isset($saved_permissions[$path]) ? $saved_permissions[$path] : '---',
			);

			$form['perms_table'][$path]['original'] = array
			(
				'#markup' => t("Original callback doesn't use user_access()"),
			);
		}
	}

	$form['buttons'] = array
	(
		'#type' => 'buttons',
	);
	$form['buttons']['submit_permissions'] = array
	(
		'#type' => 'submit',
		'#value' => t('Save menu permissions'),
	);

	return $form;
}

function custom_menu_perms_alter_menu_perms_form_submit($form, &$form_state)
{
	db_truncate('cmp_menu_perms')->execute();
	$insert_query = db_insert('cmp_menu_perms')->fields(array('menu_path', 'cmp_permission_key'));
	$insert = FALSE;
	foreach($form_state['values']['perms_table'] as $path => $values)
	{
		if($values['permission'] != '---')
		{
			$insert = TRUE;
			$insert_query->values(array($path, $values['permission']));
		}
	}
	if($insert)
	{
		$insert_query->execute();
		menu_rebuild();
	}
}

function theme_custom_menu_perms_perm_table($variables)
{
	$form = $variables['form'];
	$header = array(t('Menu path'), t('Permission'), t('Original'));
	$rows = array();
	foreach(element_children($form) as $path)
	{
		$row = array();
		$row[] = $path;
		$row[] = drupal_render($form[$path]['permission']);
		$row[] = drupal_render($form[$path]['original']);

		$rows[] = $row;
	}

	return theme('table', array('header' => $header, 'rows' => $rows));
}

function custom_menu_perms_permission_list()
{
	$page = array
	(
		'add_permission' => array
		(
			'#markup' => l(t('Add new permission'), 'admin/config/system/custom_menu_perms/add_permission'),
			'#prefix' => '<p>',
			'#suffix' => '</p>',
		),
		'permission_list_form' => drupal_get_form('custom_menu_perms_permission_list_form'),
	);

	drupal_alter('custom_menu_perms_permission_list_page', $page);

	return $page;
}

function custom_menu_perms_permission_list_form($form, &$form_state)
{
	$permissions = db_query
	(
		'SELECT perm_name, perm_key, description ' .
		'FROM {cmp_permissions} '
	);

	$header = array
	(
		'name' => t('Permission Name'),
		'description' => t('Description'),
		'edit' => t('Edit'),
	);

	$options = array();
	foreach($permissions as $permission)
	{
		$options[$permission->perm_key] = array
		(
			'name' => $permission->perm_name,
			'description' => $permission->description,
			'edit' => l(t('Edit'), 'admin/config/system/custom_menu_perms/edit_permission/' . $permission->perm_key),
		);
	}
	$form['perms_table'] = array
	(
		'#type' => 'tableselect',
		'#header' => $header,
		'#options' => $options,
		'#empty' => t('No permission have been created. !add_new_permission', array('!add_new_permission' => l(t('Add new permission'), 'admin/config/system/custom_menu_perms/add_permission'))),
	);

	if(count($options))
	{
		$form['delete_perms'] = array
		(
			'#type' => 'submit',
			'#value' => t('Delete selected perms'),
		);
	}

	return $form;
}

function custom_menu_perms_permission_list_form_submit($form, &$form_state)
{
	foreach($form_state['values']['perms_table'] as $permission => $value)
	{
		if($permission === $value)
		{
			$perm_title = db_query('SELECT perm_name FROM {cmp_permissions} WHERE perm_key = :perm_key', array(':perm_key' => $value))->fetchField();
			try
			{
				$deleted = db_delete('cmp_permissions')
					->condition('perm_key', $value)
					->execute();
				if($deleted)
				{
					$deleted = db_delete('cmp_menu_perms')
						->condition('', $value)
						->execute();
					if($deleted)
					{
						drupal_set_message(t('The permission %@permission was deleted', array('%@permission' => $perm_title)));
					}
				}
			}
			catch(Exception $e)
			{
				drupal_set_Message(t('There was a problem deleting the permission %@permission', array('%@permission' => $perm_title)));
			}
		}
	}
}
