<?php

/**
 * @file
 * Provides the reply form pane.
 */

$plugin = array(
  'title' => t('Subfolder Outline'),
  'description' => t('Folder outline with current folder nested'),
  'single' => TRUE,
  'category' => array(t('Open Atrium Folders'), -9),
  'edit form' => 'oa_folders_subfolder_outline_settings_form',
  'render callback' => 'oa_folders_subfolder_outline_render',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 */
function oa_folders_subfolder_outline_render($subtype, $conf, $args, $context = NULL) {
  if (!isset($context->data)) {
    return;
  }
  $node = $context->data;
  if ($node->type != 'oa_folder') {
    return;
  }
  $node_wrapper = entity_metadata_wrapper('node', $node);

  // Generate the folder tree
  $trail = array();
  $folders = oa_folders_subfolder_outline_generate_tree($node_wrapper, array(), $trail);
  // $output = oa_folders_subfolder_outline_render_tree($folders, $node->nid);
  $output = oa_folders_render_folder_tree($folders, $node->nid, $trail, array('recurse' => TRUE, 'display_files' => FALSE));
  
  $block = new stdClass();
  // $block->title = t('Folder Outline');
  $block->content = $output;
  return $block;
}

/**
 * Generate the folder tree recursively.
 */
function oa_folders_subfolder_outline_generate_tree(EntityMetadataWrapper $node, $subfolders, &$ancestors) {
  $nid = $node->getIdentifier();
  if (in_array($nid, $ancestors)) {
    // To avoid circular nesting we return the folders if the nid is already in the list.
    return $subfolders;
  }
  $ancestors[] = $nid;

  // Add children if it is not part of the recursion.
  if(empty($subfolders)) {
    $subfolders = oa_folders_subfolder_outline_children($node, 0);
  }

  $folders = array();
  $folders[$nid] = array(
    'title' => $node->label(),
    'subfolders' => $subfolders,
    'depth' => count($ancestors),
  );

  if (!$node->field_oa_folder_parent->value()) {
    // The folder is already the root folder, end of recursion.
    return $folders;
  }

  // Add the siblings
  $folders += oa_folders_subfolder_outline_children($node->field_oa_folder_parent, count($ancestors));

  // Recurs with the parent folder.
  return oa_folders_subfolder_outline_generate_tree($node->field_oa_folder_parent, $folders, $ancestors);
}

/**
 * Get the children without further subfolders.
 */
function oa_folders_subfolder_outline_children(EntityMetadataWrapper $node, $depth) {
  $folders = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'oa_folder')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_oa_folder_parent', 'target_id', $node->getIdentifier(), '=');
  $result = $query->execute();
  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    foreach(node_load_multiple($nids) as $node) {
      $folders[$node->nid] = array(
        'title' => $node->title,
        'subfolders' => array(),
        'depth' => $depth,
      );
    }
  }
  return $folders;
}


/**
 * Empty config form
 */
function oa_folders_subfolder_outline_settings_form($form, &$form_state) {
  return $form;
}

/**
 * Saves changes to the widget.
 */
function oa_folders_subfolder_outline_settings_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['values']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

