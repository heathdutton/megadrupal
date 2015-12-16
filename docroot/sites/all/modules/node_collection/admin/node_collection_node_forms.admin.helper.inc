<?php
/**
 * @file
 * The file that contains the functions that needed to alter the form
 * create edit screens.
 */

/**
 * Add a menu link item
 * @throws NodeCollectionRuntimeException
 *
 * @param object $parent_node_object
 *   Parent node object
 *
 * @param object $child_node_object
 *   Child node object
 *
 * @return bool
 *   Return TRUE or FALSE
 */
function node_collection_add_menu_link_item_to_parent($parent_node_object, $child_node_object) {
  // Check if the $parent_node_object is a proper node object.
  if (empty($parent_node_object) || isset($parent_node_object->nid) == FALSE || isset($parent_node_object->title) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid parent object specified in _node_collection_add_menu_link_item_to_parent');
  }
  // Check if the $child_node_object is a proper node object.
  if (empty($child_node_object) || isset($child_node_object->nid) == FALSE || isset($child_node_object->title) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid child object specified in _node_collection_add_menu_link_item_to_parent');
  }

  // Path to the node collection home.
  if ($parent_node_object->nid == $child_node_object->nid) {
    $link_title = t('Overview');
  }
  else {
    $link_title = $child_node_object->title;
  }

  $link_item = array(
    'menu_name' => "nc-$parent_node_object->nid",
    'weight' => (int) $child_node_object->nid + 10,
    'link_title' => $link_title,
    // Save this as an external link so that we can have menu items for
    // un-published content.
    'link_path' => "node/$child_node_object->nid",
    'module' => 'node_collection',
  );

  // Save the item to database.
  $is_successful = menu_link_save($link_item);

  if ($is_successful) {
    // Clear all the menu cache.
    menu_cache_clear_all();
    return TRUE;
  }
  else {
    throw new NodeCollectionRuntimeException('Creating the menu link item failed in _node_collection_add_menu_link_item_to_parent');
  }
}

/**
 * Get the links that has edit links to the children
 * Exclude nid will exclude any nodes being inserted as edit links
 *
 * @param object $parent_node
 *   parent node object
 *
 * @param object $child_node
 *   child node object
 *
 * @param array $exclude_nids
 *   any node ids to exclude
 *
 * @return array|null|string
 *   Return an array, null or a string
 */
function _node_collection_get_children_edit_links($parent_node, $child_node, $exclude_nids = array()) {
  $edit_list = array();

  if (
    isset($parent_node) && isset($parent_node->nid) && isset($parent_node->type) &&
    isset($child_node) && isset($child_node->nid)
  ) {
    try {
      $exclude_nids[] = $child_node->nid;

      // Set the title and the parent node edit links.
      if ($parent_node->nid == $child_node->nid) {
        $output = '<h1>' . t('Edit child nodes') . '</h1>';
        $output .= t('Use the links below to edit the child nodes associated with @node_title', array('@node_title' => $parent_node->title));
      }
      else {
        $output = '<h1>' . t('Edit parent node') . '</h1>';
        $output .= l(t("Edit parent : @node_type", array('@node_type' => $parent_node->title)), "node/$parent_node->nid/edit");
        $output .= '<h1>' . t('Edit sibling nodes') . '</h1>';
        $output .= t('Use the links below to edit the sibling nodes associated with @node_title', array('@node_title' => $parent_node->title));
      }
      // Get the node ids associates with  the parent.
      $node_ids = node_collection_api_get_child_node_instances($parent_node->nid);

      if (empty($node_ids) == FALSE) {
        foreach ($node_ids as $nid) {
          $child_node = node_load($nid);
          // Add the edit link.
          if ($child_node) {
            // Check if any nids has been passed so that we have to exclude them
            // from editing.
            if (empty($exclude_nids) == FALSE && is_array($exclude_nids)) {
              if (in_array($child_node->nid, $exclude_nids) == FALSE) {
                $edit_list[] = l(
                  t('Edit : @child_node_title', array('@child_node_title' => $child_node->title)),
                  "node/$child_node->nid/edit",
                  array('query' => array(PARENT_ID_QUERY_STRING => $parent_node->nid))
                );
              }
            }
            else {
              $edit_list[] = l(
                t('Edit : @child_node_title', array('child_node_title' => $child_node->title)),
                "node/$child_node->nid/edit",
                array('query' => array(PARENT_ID_QUERY_STRING => $parent_node->nid))
              );
            }
          }
        }
      }
    }
    catch (Exception $exp) {
      watchdog_exception('node_collection', $exp);
    }
  }

  if (empty($edit_list) && isset($parent_node->nid) && is_numeric($parent_node->nid)) {
    return t('There were no child pages associated with this page yet.');
  }
  else {
    // Return the set of links for edit the children.
    return $output . theme(
      'item_list',
      array(
        'items' => $edit_list,
        'type' => 'ul',
      )
    );
  }
}

/**
 * List child creation links
 *
 * @param object $parent_node
 *   The parent node object
 *
 * @return void
 *   Dont return anything
 */
function _node_collection_create_children($parent_node) {
  if ($parent_node && isset($parent_node->type) && isset($parent_node->nid)) {
    $output = '<h1>' . t('Create child nodes using the following link.') . '</h1>';
    $create_list = array();
    $relationships = node_collection_api_get_parent_child_relationship_object($parent_node->type);

    foreach ($relationships as $relationship) {
      foreach ($relationship['children'] as $key => $child) {
        $create_link_type = str_replace('_', '-', $key);
        $create_list[] = l(
          t(
            'Create @node_type', array('@node_type' => $child)
          ),
          "node/add/$create_link_type",
          array('query' => array(PARENT_ID_QUERY_STRING => $parent_node->nid))
        );
      }
    }

    // Add the create links as a list.
    $output .= theme(
      'item_list',
      array(
        'items' => array_values($create_list),
        'type' => 'ul',
      )
    );
  }
  else {
    $output = t('No child relationships defined.');
  }

  return $output;
}

/**
 * Check if the path is prepended and if not prepend it and return it
 *
 * @param int $parent_nid
 *   Parent id
 *
 * @param string $current_path
 *   Current Path
 *
 * @return string
 *   Return the path alias as a string
 */
function _node_collection_get_child_path_alias($parent_nid, $current_path) {
  try {
    // Update the path aliases.
    if ($parent_nid && is_numeric($parent_nid)) {
      // Get the parent path alias.
      $parent_path = drupal_lookup_path('alias', "node/$parent_nid");

      // Append the $ to the end.
      $preg_match_string = '/' . preg_quote(($parent_path . '/'), '/') . '/';

      if (strstr($parent_path, $parent_nid) == FALSE && empty($parent_path) == FALSE) {
        if (isset($current_path) && empty($current_path) == FALSE) {
          // See if we already prepended the path.
          if (preg_match($preg_match_string, $current_path) == FALSE) {
            // Set the path alias as a part of the parent.
            return $parent_path . '/' . $current_path;
          }
        }
      }
    }
  }
  catch (Exception $alias_update_exception) {
    watchdog_exception('node_collection', $alias_update_exception);
  }

  // Return the path.
  return $current_path;
}

/**
 * Render a Menu Local Action to add a link that will navigate the user to the
 * associated menu edit screen.
 *
 * @param int $parent_node_id
 *   Node ID of the parent for the menu link
 */
function _node_collection_administer_associated_menu_links($parent_node_id) {
  drupal_goto("admin/structure/menu/manage/nc-$parent_node_id");
}
