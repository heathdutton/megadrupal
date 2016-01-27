<?php
/**
 * @file
 * File which contains all the page and title callbacks
 * for the content administration pages defined in the hook_menu()
 * function in the module file of this module.
 */

/**
 * Page callback for admin/content.
 */
function content_admin_tree_page() {
  $content = array();
  $menu_list_arr = _content_admin_tree_mla_cache('mla');
  // Loop through each menu item block by node type
  // and create renderable content array.
  $count = 0;
  foreach ($menu_list_arr as $item) {
    // Initialise variables to be used for dynamic markup
    // in the template file.
    $item['active'] = FALSE;
    $item['start'] = FALSE;
    $item['end'] = FALSE;
    // Setting variables to use for dynamic content in template file.
    if ($count === 0) {
      $item['start'] = TRUE;
    }
    // Where the item is the last item set end to
    // true.
    if ($count === count($menu_list_arr) - 1) {
      $item['end'] = TRUE;
    }
    else {
      $item['end'] = FALSE;
    }
    $content['cat_menu'][] = array(
      '#type' => 'markup',
      '#markup' => theme('content_admin_tree_menu_item', $item),
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
        ),
        'js' => array(
          drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
        ),
      ),
    );
    $count++;
  }
  $current_path_arr = drupal_get_destination();
  $current_path = $current_path_arr['destination'];
  $current_path_split = explode('/', $current_path);
  $views_block = views_embed_view('content', 'content_block');
  // The render array for the views block.
  $content['cat_views_block'] = array(
    '#type' => 'markup',
    '#markup' => theme('content_admin_tree_views_block', array('views_block' => $views_block)),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
      ),
      'js' => array(
        drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
      ),
    ),
  );
  return $content;
}

/**
 * Page callback for admin/content/%.
 *
 * % is equal to node type.
 *
 * @param string $node_type
 *   The string passed from the current url wildcard.
 */
function content_admin_tree_page_node_type($node_type) {
  $content = array();
  $menu_list_arr = _content_admin_tree_mla_cache('mla');
  // Loop through each menu item block by node type
  // and create renderable content array.
  $count = 0;
  foreach ($menu_list_arr as $item) {
    $nt_new_name = $node_type->name;
    // Initialise variables to be used for dynamic markup
    // in the template file.
    $item['active'] = FALSE;
    $item['start'] = FALSE;
    $item['end'] = FALSE;
    $nt_new_name = $node_type->name;
    // Setting variables to use for dynamic content in template file.
    if ($nt_new_name === $item['node_type_name']) {
      $item['active'] = TRUE;
      // Where the item is the first in the list
      // set start to true and if the array
      // only contains one item set end to true.
      if ($count === 0) {
        $item['start'] = TRUE;
        if (count($menu_list_arr) === 1) {
          $item['end'] = TRUE;
        }
        else {
          $item['end'] = FALSE;
        }
      }
      else {
        $item['start'] = FALSE;
        if (count($menu_list_arr) === 1) {
          $item['end'] = TRUE;
        }
        else {
          $item['end'] = FALSE;
        }
      }
    }
    else {
      $item['active'] = FALSE;
      // Where the item is the first in the list
      // set start to true and if the array
      // only contains one item set end to true.
      if ($count === 0) {
        $item['start'] = TRUE;
        if (count($menu_list_arr) === 1) {
          $item['end'] = TRUE;
        }
        else {
          $item['end'] = FALSE;
        }
      }
      else {
        $item['start'] = FALSE;
        if (count($menu_list_arr) === 1) {
          $item['end'] = TRUE;
        }
        else {
          $item['end'] = FALSE;
        }
      }
    }
    // Where the item is the last item set end to
    // true.
    if ($count === count($menu_list_arr) - 1) {
      $item['end'] = TRUE;
    }
    else {
      $item['end'] = FALSE;
    }
    $content['cat_action_links'] = array(
      '#type' => 'markup',
      '#markup' => theme('content_admin_tree_action_links', array('node_type' => $node_type)),
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
        ),
        'js' => array(
          drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
        ),
      ),
    );
    $content['cat_menu'][] = array(
      '#type' => 'markup',
      '#markup' => theme('content_admin_tree_menu_item', $item),
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
        ),
        'js' => array(
          drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
        ),
      ),
    );
    $count++;
  }
  // Set node type to an empty string.
  $ntype = '';
  // Set the node type to the node type from the URL args.
  $ntype = $node_type->type;
  if ($ntype !== '') {
    $views_block = views_embed_view('content', 'content_block', $ntype);
  }
  else {
    $views_block = views_embed_view('content', 'content_block');
  }
  // The render array for the views block.
  $content['cat_views_block'] = array(
    '#type' => 'markup',
    '#markup' => theme('content_admin_tree_views_block', array('views_block' => $views_block)),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
      ),
      'js' => array(
        drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
      ),
    ),
  );
  return $content;
}

/**
 * Page callback for admin/content/%/%.
 *
 * %/% is equal to content_type/taxonomy_term.
 *
 * @param string $node_type
 *   The string passed from the current url wildcard
 *   at admin/content/%.
 *
 *   For example where the path is admin/content/news
 *   the node type stirng will be "news".
 *
 * @param string $taxonomy_term
 *   The string passed from the current url wildcard
 *   at admin/content/%/%.
 *
 *   For example where the path is admin/content/news/football
 *   the node type string will be "football".
 */
function content_admin_tree_page_node_type_taxonomy($node_type, $taxonomy_term) {
  $content = array();
  $menu_list_arr = _content_admin_tree_mla_cache('mla');
  // Create html string to output the taxonomy tree.
  // Loop through each menu item block by node type
  // and create renderable content array.
  $count = 0;
  foreach ($menu_list_arr as $item) {
    // Initialise variables to be used for dynamic markup
    // in the template file.
    $item['active'] = FALSE;
    $item['start'] = FALSE;
    $item['end'] = FALSE;
    $node_type_new_name = $node_type->name;
    // Setting variables to use for dynamic content in template file.
    if ($node_type_new_name === drupal_strtolower($item['node_type_name'])) {
      $item['active'] = TRUE;
      // Where the item is the first in the list
      // set start to true and if the array
      // only contains one item set end to true.
      if ($count === 0) {
        $item['start'] = TRUE;
        if (count($menu_list_arr) === 1) {
          $item['end'] = TRUE;
        }
        else {
          $item['end'] = FALSE;
        }
      }
      else {
        $item['start'] = FALSE;
        if (count($menu_list_arr) === 1) {
          $item['end'] = TRUE;
        }
        else {
          $item['end'] = FALSE;
        }
      }
    }
    else {
      $item['active'] = FALSE;
      // Where the item is the first in the list
      // set start to true and if the array
      // only contains one item set end to true.
      if ($count === 0) {
        $item['start'] = TRUE;
        if (count($menu_list_arr) === 1) {
          $item['end'] = TRUE;
        }
        else {
          $item['end'] = FALSE;
        }
      }
      else {
        $item['start'] = FALSE;
        if (count($menu_list_arr) === 1) {
          $item['end'] = TRUE;
        }
        else {
          $item['end'] = FALSE;
        }
      }
    }
    // Where the item is the last item set end to
    // true.
    if ($count === count($menu_list_arr) - 1) {
      $item['end'] = TRUE;
    }
    else {
      $item['end'] = FALSE;
    }
    $content['cat_action_links'] = array(
      '#type' => 'markup',
      '#markup' => theme('content_admin_tree_action_links', array('node_type' => $node_type, 'taxonomy_term' => $taxonomy_term)),
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
        ),
        'js' => array(
          drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
        ),
      ),
    );
    $content['cat_menu'][] = array(
      '#type' => 'markup',
      '#markup' => theme('content_admin_tree_menu_item', $item),
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
        ),
        'js' => array(
          drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
        ),
      ),
    );
    $count++;
  }
  $node_type_new = $node_type->type;
  // Set the taxonomy term tid to 0 by default.
  $taxonomy_term_tid = 0;
  // Get the taxonomy term ID.
  $taxonomy_term_tid = $taxonomy_term->tid;
  if ($node_type_new !== '' && $taxonomy_term_tid !== 0) {
    $views_block = views_embed_view('content', 'content_block', $node_type_new, $taxonomy_term_tid);
  }
  elseif ($node_type_new !== '' && $taxonomy_term_tid === 0) {
    $views_block = views_embed_view('content', 'content_block', $node_type_new);
  }
  else {
    $views_block = views_embed_view('content', 'content_block');
  }
  // The render array for the views block.
  $content['cat_views_block'] = array(
    '#type' => 'markup',
    '#markup' => theme('content_admin_tree_views_block', array('views_block' => $views_block)),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css',
      ),
      'js' => array(
        drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js',
      ),
    ),
  );
  return $content;
}

/**
 * Title callback for admin/content/%.
 *
 * @param string $node_type
 *   See page callback functions.
 */
function content_admin_tree_title_node_type($node_type) {
  $node_type_new = $node_type->name;
  $title = check_plain($node_type_new);
  return $title;
}

/**
 * Title callback for admin/content/%/%.
 *
 * @param string $node_type
 *   See page callback functions.
 *
 * @param string $taxonomy_term
 *   See page callback functions.
 */
function content_admin_tree_title_node_type_taxonomy($node_type, $taxonomy_term) {
  $node_type_new = $node_type->name;
  $title = check_plain($taxonomy_term->name);
  return $title;
}

/**
 * Creates an array of objects to be rendered.
 *
 * @return array
 *   The array containing the menu data.
 */
function _content_admin_tree_menulist() {
  $menu_list_arr = array();
  $taxonomy_html = '';
  // Get the current rebuilt list of node type names
  // keyed by their node type.
  $node_types = _content_admin_tree_node_type_arr();
  // Loop through each node type
  // to find out whether there is a
  // vocabulary attached.
  foreach ($node_types as $key => $value) {
    $vid = NULL;
    $taxonomy_html = '';
    // Create an array of previous Vocabulary IDs to make sure
    // terms aren't duplicated.
    $previous_vids = array();
    $nt_field_bundle = field_info_instances('node', $key);
    // Loop through each node type field bundle.
    foreach ($nt_field_bundle as $field) {
      // Store module and type in local function variables.
      $field_module = $field['display']['default']['module'];
      $field_type = $field['display']['default']['type'];
      // String to test for field module.
      $taxonomy_term_module = 'taxonomy';
      // String to test for field type.
      $taxonomy_term_reference = 'taxonomy_term_reference_link';
      // Check whether the field is a taxonomy term reference.
      if ($field_module === $taxonomy_term_module && $field_type === $taxonomy_term_reference) {
        // Where this is the case get field information.
        $field_info = field_info_field_by_id($field['field_id']);
        // Store vocabulary name in local variable.
        $vocab = $field_info['settings']['allowed_values'][0]['vocabulary'];
        // Get an array of vocabulary's data via the machine name.
        $vocab_array = taxonomy_vocabulary_machine_name_load($vocab);
        // Get the vocabulary id of the field's vocabulary.
        $vid = $vocab_array->vid;
        // Initialize boolean for determining whether the vid
        // has already been used by another field.
        $duplicate = FALSE;
        // Where previous_vids isn't empty loop through
        // each previous vocabulary ID.
        if (!empty($previous_vids)) {
          foreach ($previous_vids as $prev_vid) {
            if ($prev_vid === $vid) {
              $duplicate = TRUE;
              break;
            }
            else {
              $duplicate = FALSE;
            }
          }
        }
        // Where there isn't a duplicate merge arrays as usual
        // otherwise don't do anything and leave the taxonomy_terms array
        // as it is.
        if ($duplicate === FALSE) {
          $tree = _content_admin_tree_get_nested_taxonomy_tree($vid);
          $taxonomy_html .= theme('content_admin_tree_taxonomy_menu_item', _content_admin_tree_output_tax_tree($tree, $value, $variables = array()));
          $previous_vids[] = $vid;
        }
      }
    }
    $node_type_taxonomy_term_arr['node_type_name'] = $value;
    global $base_url;
    $namepath = str_replace('_', '-', $key);
    $node_type_taxonomy_term_arr['node_type_url'] = $base_url . '/admin/content/' . $namepath;
    $node_type_taxonomy_term_arr['node_type'] = $key;
    $menu_list_arr[$key] = $node_type_taxonomy_term_arr;
    // Add taxonomy html string to array.
    if ($vid !== NULL) {
      // Add vids to the array.
      if (!isset($previous_vids[$vid])) {
        $previous_vids[] = $vid;
      }
      $vids = $previous_vids;
      $menu_list_arr[$key]['vids'] = $vids;
      $menu_list_arr[$key]['taxonomy_tree_html'] = $taxonomy_html;
    }
  }
  return $menu_list_arr;
}

/**
 * A function for building an array.
 *
 * This is keyed by a node type's
 * machine name with a value of the node
 * type's human readable name.
 *
 * @return array
 *   An array holding node types keyed by
 *   their machine names.
 */
function _content_admin_tree_node_type_arr() {
  $node_type_rebuild = _node_types_build($rebuild = TRUE);
  $node_type_array = $node_type_rebuild->names;
  foreach ($node_type_array as $node_type => $name) {
    $config_type = variable_get('content_admin_tree_' . $node_type);
    if ($config_type === 0) {
      unset($node_type_array[$node_type]);
      ctools_include('object-cache');
      ctools_object_cache_clear('content_admin_tree', 'mla');
    }
  }
  return $node_type_array;
}
/**
 * Uses ctools object cache to cache the array.
 *
 * This array data will be stored untill
 * caches are next flushed.
 * This is to cache the menu list array.
 *
 * @param string $name
 *   The name of the object in the cache.
 *
 * @return object
 *   The cache object.
 */
function _content_admin_tree_mla_cache($name) {
  $cache = NULL;
  ctools_include('object-cache');
  $cache = ctools_object_cache_get('content_admin_tree', $name);
  if (!$cache || $cache === NULL) {
    $menu_list_arr = _content_admin_tree_menulist();
    $cache = _content_admin_tree_set_cache($name, $menu_list_arr);
  }
  return $cache;
}
/**
 * Function to set to store a cache object.
 *
 * @param string $name
 *   The name of the cache object
 *
 * @param array $cache
 *   The name of the object to be cached.
 */
function _content_admin_tree_set_cache($name, $cache) {
  ctools_include('object-cache');
  ctools_object_cache_set('content_admin_tree', $name, $cache);
  return $cache;
}
/**
 * A function to get the nested taxonomy tree and creates a tree structure.
 *
 * @param int $vid
 *   The vocabulary ID
 *
 * @param array $terms
 *   The terms array.
 *
 * @param int $max_depth
 *   The maximum amount of levels for the taxonomy tree ouput.
 *
 * @param int $parent
 *   The parent of a taxonomy term which is used when using recursion.
 *
 * @param array $parents_index
 *   An array of parents.
 *
 * @param int $depth
 *   The depth of the current term/s.
 *
 * @return array
 *   The tree of objects to be rendered by the output
 *   function.
 */
function _content_admin_tree_get_nested_taxonomy_tree($vid, $terms = array(), $max_depth = NULL, $parent = 0,
$parents_index = array(), $depth = 0) {
  // Initialize the empty tree array.
  $tree = array();
  // Get the taxonomy tree for the current vocabulary ID.
  $terms = taxonomy_get_tree($vid);
  // Loop through each taxonomy term.
  foreach ($terms as $term) {
    // Loop through each of the term's parents.
    foreach ($term->parents as $term_parent) {
      // Where the term parent is equal to the parent
      // parameter passed in then add the term to
      // the tree.
      // Otherwise add it to the parents index.
      if ((int) $term_parent === (int) $parent) {
        $tree[$term->tid] = $term;
      }
      else {
        $parents_index[$term_parent][$term->tid] = $term;
      }
    }
  }
  // Now that the tree is created,
  // Loop through the tree.
  foreach ($tree as &$term) {
    // Where the term is set in the parents index
    // array and the depth is less than the maximum
    // depth set then call the function again with
    // that particular term's children and add the
    // returned tree to the term object's children array.
    if (isset($parents_index[$term->tid]) &&
      (is_null($max_depth) || $depth < $max_depth)) {
      $term->children = _content_admin_tree_get_nested_taxonomy_tree($vid, $parents_index[$term->tid], $max_depth,
      $term->tid, $parents_index, $depth + 1);
    }
  }
  return $tree;
}
/**
 * Theme function for taxonomy tree menu items.
 */
function theme_content_admin_tree_taxonomy_menu_item($variables) {
  // Add the CSS & JS files to the theming process.
  drupal_add_css(drupal_get_path('module', 'content_admin_tree') . '/css/content_admin_tree.css');
  drupal_add_js(drupal_get_path('module', 'content_admin_tree') . '/js/content_admin_tree.js');
  // Initialise the output html string.
  $output = '';
  // Loop through each set of term variables.
  foreach ($variables as $term_variables) {
    // Where the current term is the first to be rendered,
    // add the UL opening tag.
    if ($term_variables['start'] === TRUE) {
      $output .= '<ul class="cat-taxonomy-tree">';
    }
    // Add the markup for the term menu link item.
    $output .= '<li class="cat-taxonomy-term leaf">';
    $output .= '<a class="cat-link" href="' . check_url($term_variables['term_url']) . '">' . check_plain($term_variables['term_name']) . '</a>';
    // Where the current term has children
    // call the theme function again for the children terms
    // and add to the final output string.
    if ($term_variables['has_children'] === TRUE) {
      $output .= theme_content_admin_tree_taxonomy_menu_item($term_variables['children']);
    }
    // Add the closing markup.
    $output .= '</li>';
    // Where the term is the last term in the array
    // Add UL close tag.
    if ($term_variables['end'] === TRUE) {
      $output .= '</ul>';
    }
  }
  return $output;
}
/**
 * Creates the output variables array.
 *
 * @param array $tree
 *   The tree to be output in this recursive function.
 *
 * @param string $node_type
 *   The string containing the node type of the block.
 *
 * @param string $variables
 *   The variables array for the tree.
 *
 * @return array
 *   The variables array.
 */
function _content_admin_tree_output_tax_tree($tree, $node_type, &$variables) {
  // Where the tree has more than 0 elements,
  // create the html string to output.
  if (count($tree) > 0) {
    $termcount = 0;
    foreach ($tree as $term) {
      global $base_url;
      $node_type = drupal_strtolower(str_replace(' ', '-', $node_type));
      // @todo: sort out term paths!!!
      $termpath = $node_type . '/overview/' . $term->tid;
      $termurl = $base_url . '/admin/content/' . $termpath;
      $variables[$term->tid]['term_name'] = $term->name;
      $variables[$term->tid]['term_tid'] = $term->tid;
      $variables[$term->tid]['term_url'] = $termurl;
      // There will be dynamic markup in the template file.
      // Below we are filtering through whether the term is the first
      // term to be rendered, the last term to be rendered or has children
      // and adding these to the term array to use to decide which markup will
      // be used within the theme function.
      if ($termcount === 0) {
        $variables[$term->tid]['start'] = TRUE;
        if (count($tree) > 1) {
          $variables[$term->tid]['end'] = FALSE;
        }
        elseif (count($tree) === 1) {
          $variables[$term->tid]['end'] = TRUE;
        }
      }
      elseif ($termcount === count($tree) - 1) {
        $variables[$term->tid]['start'] = FALSE;
        $variables[$term->tid]['end'] = TRUE;
      }
      else {
        $variables[$term->tid]['start'] = FALSE;
        $variables[$term->tid]['end'] = FALSE;
      }
      if (isset($term->children) || !empty($term->children)) {
        $variables[$term->tid]['has_children'] = TRUE;
      }
      else {
        $variables[$term->tid]['has_children'] = FALSE;
      }
      // Where the current term has children loop through each child
      // and call the function for the child terms.
      if (isset($term->children) || !empty($term->children)) {
        foreach ($term->children as $key => $child) {
          _content_admin_tree_output_tax_tree($term->children, $node_type, $variables[$term->tid]['children']);
        }
      }
      $termcount++;
    }
  }
  return $variables;
}
