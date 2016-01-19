<?php

/**
 * @file
 * Provides the reply form pane.
 */

$plugin = array(
  'title' => t('Folder Tree'),
  'description' => t('Folder tree for a section'),
  'single' => TRUE,
  'category' => array(t('Open Atrium Folders'), -9),
  'edit form' => 'oa_folders_folders_block_settings_form',
  'render callback' => 'oa_folders_folders_block_render',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);




/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 */
function oa_folders_folders_block_render($subtype, $conf, $args, $context = NULL) {

  $folders = array();
  if ($conf['folders_sections'] == 'folder') {
    $folders = oa_folders_get_folders_tree('view', NULL, TRUE);
    if (isset($folders[$conf['root_folder']])){
      $folders = array($conf['root_folder'] => $folders[$conf['root_folder']]);
    }
  }
  elseif ($conf['folders_sections'] == 'section') {
    $folders = oa_folders_get_folders_tree('view', $conf['root_section']);
    $conf['root_folder'] = -1;
  }
  if (!$folders) {
    return;
  }
  $options = array('recurse' => TRUE, 'collapsible' => $conf['collapsible'], 'display_files' => $conf['files']);
  // if it is collapsible we pass the node id, otherwise all folders are open.
  $nid = ($conf['collapsible'] ? $conf['root_folder'] : OA_FOLDERS_ALL_OPEN);
  $output = oa_folders_render_folder_tree($folders, $nid, array(), $options);

  $block = new stdClass();
  // $block->title = t('Folder Outline');
  $block->content = $output;
  return $block;
}


/**
 * Empty config form
 */
function oa_folders_folders_block_settings_form($form, &$form_state) {

// dpm($form);
// dpm($form_state['conf']);

  $section_options = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'oa_section')
    ->propertyCondition('status', NODE_PUBLISHED);
  $result = $query->execute();
  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    foreach(node_load_multiple($nids) as $node) {
      $section_options[$node->nid] = $node->title . ' (' . $node->nid . ')';
    }
  }

  $folder_options = oa_folders_all_folders_tree_recursive_options(oa_folders_get_folders_tree('view'), '--', 0, TRUE);
  $form['folders_sections'] = array(
    '#type' => 'select',
    '#title' => t('List Folders or Sections'),
    '#options' => array(
      'folder' => t('Folders'),
      'section' => t('Section'),
    ),
    '#default_value' => $form_state['conf']['folders_sections'],
    '#description' => t('Display the files of the folders or all folders in a section?'),
    '#required' => TRUE,
  );
  $form['root_folder'] = array(
    '#type' => 'select',
    '#title' => t('Root Folder'),
    '#options' => $folder_options,
    '#default_value' => $form_state['conf']['root_folder'],
    '#description' => t('Select the root folder of which all sub-folders will be displayed.'),
    '#states' => array(
      'visible' => array(
        ':input[name="folders_sections"]' => array('value' => 'folder'),
      ),
    ),
  );
  $form['root_section'] = array(
    '#type' => 'select',
    '#title' => t('Root Section'),
    '#options' => $section_options,
    '#default_value' => $form_state['conf']['root_section'],
    '#description' => t('Select the section of which all folders will be displayed.'),
    '#states' => array(
      'visible' => array(
        ':input[name="folders_sections"]' => array('value' => 'section'),
      ),
    ),
  );
  $form['files'] = array(
    '#type' => 'select',
    '#title' => t('Files'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#default_value' => $form_state['conf']['files'],
    '#description' => t('Display the files of the folders?'),
    '#required' => TRUE,
  );
  $form['collapsible'] = array(
    '#type' => 'select',
    '#title' => t('Collapsible'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#default_value' => $form_state['conf']['collapsible'],
    '#description' => t('Display the folders in a collapsible way?'),
    '#required' => TRUE,
  );
  
  return $form;
}

/**
 * Saves changes to the widget.
 */
function oa_folders_folders_block_settings_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['values']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

