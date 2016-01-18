<?php
/**
 * @file
 * Admin page callbacks for the path level blocks module.
 *
 * @name Path-level Blocks Admin
 */

/**
 * Page callback that provides an overview on all path level block settings.
 */
function path_level_blocks_overview() {
  $results = db_select('path_level_blocks_settings', 'plbs')
    ->fields('plbs', array('plbid', 'title'))
    ->execute();

  $header = array(
    array('data' => t('Path-level block')),
    array('data' => t('Actions'), 'colspan' => 2),
  );
  $rows = array();

  foreach ($results as $plb) {
    $rows[] = array(
      array('data' => l($plb->title, 'admin/structure/path-level-blocks/edit/' . $plb->plbid, array('html' => TRUE))),
      array('data' => l(t('edit'), 'admin/structure/path-level-blocks/edit/' . $plb->plbid, array('html' => TRUE))),
      array('data' => l(t('delete'), 'admin/structure/path-level-blocks/delete/' . $plb->plbid, array('html' => TRUE))),
    );
  }
  return theme('table', array("header" => $header, "rows" => $rows));
}

/**
 * Form definition to add a new path level block setting.
 */
function path_level_blocks_form($form, $form_state, $op, $plbid = NULL) {
  $context = array('plb' => 'plb');
  $blocks = array();

  // Load plb settings and blocks.
  if ($op == 'edit' && $plbid) {
    $plb = db_select('path_level_blocks_settings', 'plbs')
      ->fields('plbs')
      ->condition('plbid', $plbid)
      ->execute()->fetch();

    $form['plbid'] = array(
      '#value' => isset($plb->plbid) ? $plb->plbid : '',
      '#type' => 'value',
    );
    $context = array('plb' => $plb);

    $blocks = _path_level_blocks_blocks($plbid);
  }

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#description' => t('Administrative title for this block setting.'),
    '#default_value' => isset($plb->title) ? $plb->title : '',
    '#required' => TRUE,
  );
  $form['paths'] = array(
    '#title' => t('Paths'),
    '#type' => 'textarea',
    '#description' => t('Paths on which this path level block setting is active. Put each path on a new line. Do not begin and add paths with a slash (/). You can use wildcards (*) and for the frontpage you use <front>.'),
    '#default_value' => isset($plb->paths) ? $plb->paths : '',
    '#required' => TRUE,
  );

  $settings = (object) array(
    'modules' => variable_get('path_level_blocks_modules', array()),
    'regions' => variable_get('path_level_blocks_regions', array()),
  );

  $form['path_level_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Path-level Blocks'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
  );

  node_level_blocks_block_overview($form['path_level_blocks'], $blocks, $settings, $context);

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $op == 'add' ? t('Add setting') : t('Modify setting'),
  );
  return $form;
}

/**
 * Submit handler to add a new path level block setting.
 *
 * @see path_level_blocks_form()
 */
function path_level_blocks_form_submit($form, &$form_state) {
  $plb = array(
    'title' => $form_state['values']['title'],
    'paths' => $form_state['values']['paths'],
  );
  $primary = array();
  if (isset($form_state['values']['plbid'])) {
    $primary = 'plbid';
    $plb['plbid'] = $form_state['values']['plbid'];
  }

  drupal_write_record('path_level_blocks_settings', $plb, $primary);

  // Delete old entries for this node.
  db_delete('path_level_blocks')
    ->condition('plbid', $plb['plbid'])
    ->execute();

  foreach ($form_state['values']['path_level_blocks']['blocks'] as $block) {
    if ($block['region'] != BLOCK_REGION_NONE) {
      db_insert('path_level_blocks')
        ->fields(array(
          'plbid' => $plb['plbid'],
          'bid' => $block['bid'],
          'region' => $block['region'],
          'weight' => $block['weight'],
        ))
        ->execute();
    }
  }

  $form_state['redirect'] = 'admin/structure/path-level-blocks';
  drupal_set_message(t('Your path-level block setting is saved.'));
}

/**
 * Form builder to delete a path level block setting.
 */
function path_level_blocks_delete_form($form, &$form_state, $plbid) {
  $form['plbid'] = array(
    '#type' => 'value',
    '#value' => $plbid,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the path level block setting?'),
    'admin/structure/path-level-blocks'
  );
}

/**
 * Form submit handler; delete an URL redirect after confirmation.
 *
 * @see path_level_blocks_delete_form()
 */
function path_level_blocks_delete_form_submit($form, &$form_state) {
  db_delete('path_level_blocks_settings')
    ->condition('plbid', $form_state['values']['plbid'])
    ->execute();
  drupal_set_message(t('The path level block setting has been deleted.'));
  $form_state['redirect'] = 'admin/structure/path-level-blocks';
}

/**
 * Path-level blocks settings.
 */
function path_level_blocks_settings_form() {
  $form['path_level_blocks_regions'] = array(
    '#type' => 'select',
    '#title' => t('Enabled regions'),
    '#description' => t('Choose which regions are available'),
    '#options' => system_region_list(variable_get('theme_default', 'garland')),
    '#multiple' => TRUE,
    '#default_value' => variable_get('path_level_blocks_regions', array()),
  );

  $form['path_level_blocks_multi'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow duplication of blocks on pages'),
    '#default_value' => variable_get('path_level_blocks_multi', FALSE),
  );

  $module_options = array();
  foreach (module_implements('block_info') as $module) {
    $module_options[$module] = $module;
  }

  $form['path_level_blocks_modules'] = array(
    '#type' => 'select',
    '#title' => t('Modules'),
    '#description' => t('Here you can choose which modules to use for providing block contents.'),
    '#options' => $module_options,
    '#multiple' => TRUE,
    '#default_value' => variable_get('path_level_blocks_modules', array()),
  );
  return system_settings_form($form);
}
