<?php

/**
 * @file
 * Utility function for importing things like nodes and taxonomies.
 * This is a temporary measure until we get uuid features as stable as we
 * want it to be, after that this will go away.
 */

/**
 * Import terms from a term structure exported by uuid_features.
 * @param string $module
 *   The module to import from.
 */
function ns_core_import_terms($module) {
  // Flush the cache to get the terms in place.
  drupal_flush_all_caches();
  // Install terms that should be there by default.
  module_load_include('default_terms.inc', $module);
  $terms = module_invoke($module, 'default_terms');
  $vocabularies = taxonomy_vocabulary_get_names();
  foreach ($terms as $term) {
    $term['vid'] = $vocabularies[$term['vocabulary_machine_name']]->vid;
    // Check if we need to save this term in the menu.
    if (isset($term['menu'])) {
      $item = $term['menu'];
      if (!isset($item['link_title'])) {
        $item['link_title'] = $term['name'];
      }
      unset($term['menu']);
    }
    $term = (object)$term;
    taxonomy_term_save($term);
    // Save this term to the menu if we have a menu item and a tid.
    if (isset($item) && isset($term->tid)) {
      $item['link_path'] = 'taxonomy/term/' . $term->tid;
      // Set frontpage if the term indicates it want that.
      if (!empty($item['frontpage'])) {
        variable_set('site_frontpage', $item['link_path']);
      }
      menu_link_save($item);
      unset($item);
    }
  }
}

/**
 * Import files from a file structure exported by uuid_features.
 * @param string $module
 *   The module to import from.
 */
function ns_core_import_files($module) {
  // Flush the cache to get the files in place
  drupal_flush_all_caches();
  // Install files that should be there by default
  module_load_include('default_files.inc', $module);
  $files = module_invoke($module, 'default_files');

  foreach ($files as $file) {
    $file = (object) $file;
    $source_dir = drupal_get_path('module', $file->module) . '/uuid_file';
    // Try to fetch another file with the same fid, and replace that file
    // if it already exists.
    $file->fid = uuid_file_find($file->uuid);
    $source_file = $source_dir . '/' . $file->uuid . '.' . $file->extension;
    // Copy the file from the module directory to our destination path,
    // but only if the file actually has changed.
    if (file_exists($source_file) && (!file_exists($file->uri) || md5_file($file->uri) != $file->hash)) {
      $file->uri = file_unmanaged_copy($source_file, $file->uri, FILE_EXISTS_REPLACE);
    }
    file_save($file);
  }
}

/**
 * Import content from a uuid features export.
 * @param string $module
 *   The module to import from.
 */
function ns_core_import_content($module) {
  drupal_flush_all_caches();
  // Install nodes that should be there by default
  module_load_include('default_content.inc', $module);
  $nodes = module_invoke($module, 'default_content');
  ns_core_save_all_nodes($nodes);
  ns_core_update_dependencies($nodes);
}

/**
 * Save all nodes in the nodes array.
 * This function will strip the node of all reference information the first time,
 * since all nodes need to exist in the database before references can be enforced.
 * @param array $nodes
 *   an array of nodes to import.
 */
function ns_core_save_all_nodes(&$nodes) {
  // A list of disallowed field types at this point.
  $disallowed_field_types = array('node_reference', 'file', 'image', 'media', 'taxonomy_term_reference');
//  $disallowed_node_types = array('ns_ch_web_promo', 'ns_ch_rss_promo');
  // Go through all nodes one by one.
  foreach ($nodes as $uuid => $node) {
    // Get all fields for the given node type.
    $field_instances = field_info_instances('node', $node['type']);
    // Loop through all fields.
    foreach ($field_instances as $name => $instance) {
      // Get more info about all fields.
      $field_info = field_info_field($name);
      // Check if this field typ is one of the disallowed filetype, if so unset it for now.
      if (in_array($field_info['type'], $disallowed_field_types)) {
        unset($node[$name]);
      }
    }
    $node = (object)$node;
    $node->is_new = TRUE;
    node_save($node);
    // Save off the nid into the array for later use.
    $nodes[$uuid]['nid'] = $node->nid;
  }
}

/**
 * Go through all references of a node and make sure they point to the entity
 * id of this installation.
 * @param array $nodes
 *   an array of nodes with dependencies.
 */
function ns_core_update_dependencies(&$nodes) {
  // A list of field types to update.
  $tid_desc = array('type' => 'taxonomy_term_data', 'key' => 'tid');
  $nid_desc = array('type' => 'node', 'key' => 'nid');
  $fid_desc = array('type' => 'file_managed', 'key' => 'fid');

  $allowed_field_types = array('node_reference' => $nid_desc,
                               'file' => $fid_desc,
                               'image' => $fid_desc,
                               'media' => $fid_desc,
                               'taxonomy_term_reference' => $tid_desc);

  foreach($nodes as $uuid => $node) {
    foreach ($allowed_field_types as $field => $desc) {
      $field_instances = field_info_instances('node', $node['type']);
      foreach ($field_instances as $name => $instance) {
        $field_info = field_info_field($name);
        if ($field_info['type'] == $field) {
          foreach ($node[$name] as $lang => &$values) {
            foreach ($values as $key => &$value) {
              // Get the entity id for the field.
              // This assumes that all nodes are already in the database.
              $entity_id = uuid_find($value['uuid'], $desc['type'] , $desc['key']);
              // replace/add nid/tid/vid to the field value
              if ($entity_id) {
                $value[$desc['key']] = $entity_id;
              }
              else {
                unset($node[$name][$lang][$key]);
              }
            }
          }
        }
      }
    }
    $node = (object)$node;
    node_save($node);
    ns_core_create_promo($node);
  }
}

/**
 * Create promos if they haven't been created by rules already. This can happen
 * because features might not have imported the rules yet.
 * @param stdClass $node
 *   The node to process.
 */
function ns_core_create_promo($node) {
  if ($node->type == 'ns_article') {
    $nids = array();
    // Check that we don't already have promos.
    $nodes = db_select('crossclone', 'c')
      ->fields('c', array('entity_id'))
      ->condition('original', $node->nid)
      ->condition('type', 'node')
      ->execute();
    // Load the nodes.
    foreach ($nodes as $node) {
      $nids[] = $node->entity_id;
    }
    // If we did find nodes, we can safely assume that rules have kicked in and
    // created them for us.
    if (count($nids)) {
      return;
    }
    $settings = array(
      'clone' => CROSSCLONE_CLONE_SIMILAR,
      'bundle' => 'ns_ch_web_promo',
      'couple_setting' => CROSSCLONE_COUPLE_NONE,
      'delete_setting' => CROSSCLONE_DELETE_DOWNSTREAM
    );
    // We create rss and web promos.
    crossclone_entity('node', $node, $settings, CROSSCLONE_SAVE);
    $settings['bundle'] = 'ns_ch_rss_promo';
    crossclone_entity('node', $node, $settings, CROSSCLONE_SAVE);
  }
}
