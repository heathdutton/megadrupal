<?php

/**
 * @file
 * Administrative page callbacks for the Taxonomy Copier module.
 */

/**
 * Build Taxonomy Copier administration form.
 */
function taxonomy_tools_copier_admin_form($form, &$form_state) {
  $form = array();
  $form['taxonomy_tools_copier_term_config'] = array(
    '#type' => 'radios',
    '#title' => t('Default term selection settings'),
    '#options' => array(
      'none' => t('No selections'),
      'parent' => t('Only highest term selected'),
      'all' => t('Whole branch selected'),
    ),
    '#default_value' => variable_get('taxonomy_tools_copier_term_config', 'none'),
  );
  $form['taxonomy_tools_copier_node_config'] = array(
    '#type' => 'radios',
    '#title' => t('Default node action settings'),
    '#options' => array(
      'none' => t('Do nothing'),
      'link' => t('Create associative links'),
      'copy' => t('Create node copies'),
    ),
    '#default_value' => variable_get('taxonomy_tools_copier_node_config', 'none'),
  );
  $form['taxonomy_tools_copier_mark_copies'] = array(
    '#type' => 'fieldset',
    '#title' => t('Marking copies'),
  );
  $form['taxonomy_tools_copier_mark_copies']['taxonomy_tools_copier_mark_term_copies'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mark term copies'),
    '#description' => t('Append text to duplicate term name.'),
    '#default_value' => variable_get('taxonomy_tools_copier_mark_term_copies', FALSE),
  );
  $form['taxonomy_tools_copier_mark_copies']['taxonomy_tools_copier_term_name_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Duplicate term name suffix'),
    '#description' => t('Text that is added to the duplicate term name.'),
    '#states' => array(
      'visible' => array(
        ':input[name="taxonomy_tools_copier_mark_term_copies"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => variable_get('taxonomy_tools_copier_term_name_suffix', t('(duplicate)')),
  );
  $form['taxonomy_tools_copier_mark_copies']['taxonomy_tools_copier_mark_node_copies'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mark node copies'),
    '#description' => t('Append text to duplicate node title.'),
    '#default_value' => variable_get('taxonomy_tools_copier_mark_node_copies', FALSE),
  );
  $form['taxonomy_tools_copier_mark_copies']['taxonomy_tools_copier_node_title_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Duplicate node title suffix'),
    '#description' => t('Text that is added to the duplicate node title.'),
    '#states' => array(
      'visible' => array(
        ':input[name="taxonomy_tools_copier_mark_node_copies"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => variable_get('taxonomy_tools_copier_node_title_suffix', t('(duplicate)')),
  );
  return system_settings_form($form);
}

/**
 * Builds taxonomy term branch copying form.
 *
 * @param stdClass $vocabulary
 *   A vocabulary object.
 * @param mixed $term
 *   Branch root taxonomy term object or identificator.
 */
function taxonomy_tools_copier_copy_form($form, &$form_state, $vocabulary, $term) {
  if (!isset($vocabulary) && is_object($term)) {
    $vocabulary = taxonomy_vocabulary_load($term->vid);
    $root = $term->tid;
  }
  else {
    $root = $term;
  }
  $form = array();
  $form['#attached']['css'][] = drupal_get_path('module', 'taxonomy_tools') . '/style/taxonomy_tools.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'taxonomy_tools_copier') . '/js/taxonomy_tools_copier.js';
  // Destination.
  $form['destination'] = taxonomy_tools_copier_destination($form_state);
  // Term branch copying options.
  $form['content'] = taxonomy_tools_copier_content($root);
  // Form submit button.
  $form['actions'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Copy'),
    ),
  );
  return $form;
}

/**
 * Builds hierarchical destination selector.
 */
function taxonomy_tools_copier_destination(&$form_state) {
  $content['destination-select'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'destination-select',
      'class' => array(
        'destination-select',
      ),
      'name' => 'destination-select',
    ),
  );
  // Hierarchical destination selector.
  // Vocabulary selector.
  $vocabularies = taxonomy_get_vocabularies();
  $options = array('none' => t('Select'));
  foreach ($vocabularies as $vocabulary) {
    $options[$vocabulary->vid] = $vocabulary->name;
  }
  $content['destination-select']['destination-vocabulary'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Vocabulary'),
    '#ajax' => array(
      'callback' => 'taxonomy_tools_copier_destination_ajax',
      'wrapper' => 'destination-select',
    ),
    '#default_value' => isset($form_state['input']['destination-vocabulary']) ? $form_state['input']['destination-vocabulary'] : 'none',
  );
  // Show term selection if vocabulary is selected.
  if (isset($form_state['input']['destination-vocabulary']) && is_numeric($form_state['input']['destination-vocabulary'])) {
    // Determine if vocabulary has been changed.
    $vid_changed = FALSE;
    $vid = $form_state['input']['destination-vocabulary'];
    if (isset($form_state['current_vid']) && $vid != $form_state['current_vid']) {
      $vid_changed = TRUE;
    }
    $form_state['current_vid'] = $vid;
    $tree = taxonomy_get_tree($vid, 0, 1);
    // Show term selectors only if vocabulary has any terms.
    if (!empty($tree)) {
      // Determine how many term selectors need to be generated.
      $count = 1;
      // Continue only when vocabulary is not changed.
      while (!($vid_changed) && isset($form_state['input']['destination-level-' . $count])) {
        if (is_numeric($form_state['input']['destination-level-' . $count])) {
          $children = taxonomy_get_children($form_state['input']['destination-level-' . $count]);
          if (!empty($children)) {
            if ((isset($form_state['input']['destination-level-' . ($count + 1)])) && (!isset($children[$form_state['input']['destination-level-' . ($count + 1)]]))) {
              $count++;
              break;
            }
            else {
              $count++;
            }
          }
          else {
            break;
          }
        }
        else {
          break;
        }
      }
    }
    else {
      $count = 0;
    }
    // Term selection.
    $level = 1;
    while ($level <= $count) {
      if ($level == 1) {
        $tid = 0;
      }
      else {
        $tid = $form_state['input']['destination-level-' . ($level - 1)];
      }
      $options = array('none' => t('Select'));
      $tree = taxonomy_get_tree($vid, $tid, 1);
      foreach ($tree as $term) {
        $options[$term->tid] = check_plain($term->name);
      }
      $content['destination-select']['destination-level-' . $level] = array(
        '#type' => 'select',
        '#title' => t('Level') . '&nbsp;' . $level,
        '#ajax' => array(
          'callback' => 'taxonomy_tools_copier_destination_ajax',
          'wrapper' => 'destination-select',
        ),
        '#options' => $options,
        '#default_value' => isset($form_state['input']['destination-level-' . $level]) ? $form_state['input']['destination-level-' . $level] : 'none',
      );
      $level++;
    }
  }
  return $content;
}

/**
 * Destination selector ajax callback.
 */
function taxonomy_tools_copier_destination_ajax($form, $form_state) {
  return $form['destination'];
}

/**
 * Builds taxonomy term hierarchical tree.
 *
 * @param int $root
 *   The current taxonomy term identificator.
 * @param int $parent
 *   The parent identificator of current taxonomy term.
 */
function taxonomy_tools_copier_content($root, $parent = 0) {
  $content = array();
  $term = array(
    '#type' => 'container',
  );
  // Set proper class for container.
  if ($parent == 0) {
    $term['#attributes']['class'] = array('root-container');
  }
  else {
    $term['#attributes']['class'] = array('child-container');
    $term['#attributes']['name'] = $parent . '-children';
  }
  $term['tid'] = array(
    '#type' => 'hidden',
    '#value' => $root,
  );
  // Parent value.
  $term[$root . '-parent'] = array(
    '#type' => 'hidden',
    '#value' => $parent,
    '#attributes' => array(
      'class' => array(
        $root . '-parent',
      ),
    ),
  );
  // Checkbox representing whether to copy this term.
  $term[$root . '-option'] = array(
    '#type' => 'checkbox',
    '#title' => taxonomy_tools_term_title($root),
  );
  $term_copy_options = variable_get('taxonomy_tools_copier_term_config', 'none');
  if (($parent == 0 && $term_copy_options != 'none') || ($parent > 0 && $term_copy_options == 'all')) {
    $term[$root . '-option']['#attributes'] = array('checked' => 'checked');
  }
  // Set proper checkbox class.
  $term[$root . '-option']['#attributes']['class'] = array($root . '-option');
  // Nodes associated with this term.
  $term += taxonomy_tools_copier_nodes($root);
  // Check for term children.
  $children = taxonomy_get_children($root);
  if (!empty($children)) {
    // Create a similar container for each child.
    $term[$root . '-children'] = array();
    foreach ($children as $child) {
      $child_content = taxonomy_tools_copier_content($child->tid, $root);
      $term[$root . '-children'] = array_merge($term[$root . '-children'], $child_content);
    }
  }
  $content[$root . '-content'] = $term;
  return $content;
}

/**
 * Builds taxonomy term associated node list.
 *
 * @param int $tid
 *   Taxonomy term identificator.
 */
function taxonomy_tools_copier_nodes($tid) {
  $content = array();
  $content[$tid . '-nodes'] = array(
    '#type' => 'container',
    '#theme' => 'taxonomy_tools_copier_nodes_container',
    '#states' => array(
      'visible' => array(
        ':input.' . $tid . '-option' => array(
          'checked' => TRUE,
        ),
      ),
    ),
    '#attributes' => array(
      'name' => $tid . '-nodes',
      'class' => array(
        $tid . '-nodes',
        'nodes',
      ),
    ),
  );
  $count = taxonomy_tools_copier_associated_nodes_count($tid);
  if ($count > 0) {
    // Options for all term associated nodes.
    $content[$tid . '-nodes'][$tid . '-nodes-option'] = array(
      '#type' => 'radios',
      '#title' => t('All nodes'),
      '#options' => array(
        'none' => t('Do nothing'),
        'copy' => t('Copy all'),
        'link' => t('Link all'),
        'custom' => t('Custom'),
      ),
      '#default_value' => variable_get('taxonomy_tools_copier_node_config', 'none'),
      '#title_display' => 'invisible',
    );
    $content[$tid . '-nodes'][$tid . '-nodes-list'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          $tid . '-nodes-list',
          'nodes-list',
        ),
        'name' => $tid . '-nodes-list',
      ),
      '#theme' => 'taxonomy_tools_copier_nodes_list_container',
    );
    // Link to reveal all nodes.
    $content[$tid . '-nodes'][$tid . '-nodes-list-link'] = array(
      '#type' => 'link',
      '#title' => t('Show nodes'),
      '#href' => '',
      '#ajax' => array(
        'path' => 'taxonomy-copier/' . $tid . '/nodes/show',
        'wrapper' => 'edit-' . $tid . '-nodes-list',
        'progress' => array('type' => 'none'),
      ),
      '#attributes' => array(
        'class' => array(
          $tid . '-nodes-list-link',
        ),
      ),
    );
  }
  return $content;
}

/**
 * Returns the HTML for taxonomy term associated nodes list.
 */
function theme_taxonomy_tools_copier_nodes_container($variables) {
  $container = $variables['container'];
  $output = '';
  foreach (element_children($container) as $key) {
    $item = &$container[$key];
    if (is_numeric(strpos($key, 'nodes-option'))) {
      $header = array();
      $rows = array();
      $attributes = array(
        'class' => array(
          'nodes-option',
        ),
      );
      $row[] = array('data' => $item['#title']);
      foreach (element_children($item) as $key2) {
        $row[] = array('data' => drupal_render($item[$key2]));
      }
      $rows[] = $row;
      $table_variables = array(
        'header' => $header,
        'rows' => $rows,
        'attributes' => $attributes,
        'caption' => '',
        'colgroups' => array(),
        'sticky' => FALSE,
        'empty' => '',
      );
      $output .= theme_table($table_variables);
    }
    else {
      $output .= drupal_render($item);
    }
  }
  return $output;
}

/**
 * Returns the HTML for all nodes listing.
 */
function theme_taxonomy_tools_copier_nodes_list_container($variables) {
  $container = $variables['container'];
  $output = '';
  $rows = array();
  foreach (element_children($container) as $key) {
    $item = &$container[$key];
    if (is_numeric(strpos($key, '-node-'))) {
      $row = array();
      foreach (element_children($item) as $key2) {
        $row[] = array(
          'data' => drupal_render($item[$key2]),
        );
      }
      $rows[] = $row;
    }
  }
  $header = array();
  $attributes = array(
    'class' => array(
      'nodes-list',
    ),
  );
  $table_variables = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => $attributes,
    'caption' => '',
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => '',
  );
  $output .= theme_table($table_variables);
  $output .= drupal_render_children($container);
  return $output;
}

/**
 * AJAX callback for all nodes link.
 *
 * @param int $tid
 *   Taxonomy term identificator.
 * @param string $action
 *   Action "show" or "hide".
 */
function taxonomy_tools_copier_nodes_ajax($tid, $action) {
  $commands = array();
  if ($action == 'show') {
    $content = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          $tid . '-nodes-list',
          'nodes-list',
        ),
        'name' => $tid . '-nodes-list',
      ),
      '#theme' => 'taxonomy_tools_copier_nodes_list_container',
    );
    // Nodes list.
    $content += taxonomy_tools_copier_nodes_list($tid);
    // Change the link to opposite action.
    $l = array(
      '#type' => 'link',
      '#title' => t('Hide nodes'),
      '#href' => '',
      '#ajax' => array(
        'path' => 'taxonomy-copier/' . $tid . '/nodes/hide',
        'wrapper' => 'edit-' . $tid . '-all-nodes',
        'progress' => array('type' => 'none'),
      ),
      '#attributes' => array(
        'class' => array(
          $tid . '-nodes-list-link',
        ),
      ),
    );
    // Show nodes list.
    $commands[] = ajax_command_replace('.' . $tid . '-nodes-list:not(.loaded)', drupal_render($content));
    $commands[] = ajax_command_invoke('.' . $tid . '-nodes-list:not(.loaded)', 'addClass', array('loaded'));
    $commands[] = ajax_command_invoke('.' . $tid . '-nodes-list.loaded', 'hide');
    $commands[] = ajax_command_invoke('.' . $tid . '-nodes-list.loaded', 'slideDown', array('slow'));
    // Replace the link.
    $commands[] = ajax_command_replace('.' . $tid . '-nodes-list-link', drupal_render($l));
  }
  elseif ($action == 'hide') {
    // Change the link to opposite action.
    $l = array(
      '#type' => 'link',
      '#title' => t('Show nodes'),
      '#href' => '',
      '#ajax' => array(
        'path' => 'taxonomy-copier/' . $tid . '/nodes/show',
        'wrapper' => 'edit-' . $tid . '-nodes-list',
        'progress' => array('type' => 'none'),
      ),
      '#attributes' => array(
        'class' => array(
          $tid . '-nodes-list-link',
        ),
      ),
    );
    // Hide nodes list.
    $commands[] = ajax_command_invoke('.' . $tid . '-nodes-list.loaded', 'slideUp', array('slow'));
    // Replace the link.
    $commands[] = ajax_command_replace('.' . $tid . '-nodes-list-link', drupal_render($l));
  }
  return ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
}

/**
 * Builds the options for all taxonomy term associated nodes.
 *
 * @param int $tid
 *   Taxonomy term identificator.
 */
function taxonomy_tools_copier_nodes_list($tid) {
  $content = array();
  $nodes = taxonomy_tools_copier_associated_nodes($tid);
  $default = variable_get('taxonomy_tools_copier_node_config', 'none');
  $current_type = '';
  $node_types = node_type_get_names();
  foreach ($nodes as $node) {
    // Options for node type.
    if ($node->type != $current_type) {
      $current_type = $node->type;
      $options = array(
        $node->type . '-option-title' => array(
          '#markup' => 'Node type: ' . $node_types[$node->type],
        ),
        $tid . '-node-type-' . $node->type . '-option-none' => array(
          '#type' => 'radio',
          '#title' => t('Do nothing'),
          '#attributes' => array(
            'name' => $tid . '-node-type-' . $node->type . '-option',
            'value' => 'none',
          ),
        ),
        $tid . '-node-type-' . $node->type . '-option-copy' => array(
          '#type' => 'radio',
          '#title' => t('Copy all'),
          '#attributes' => array(
            'name' => $tid . '-node-type-' . $node->type . '-option',
            'value' => 'copy',
          ),
        ),
        $tid . '-node-type-' . $node->type . '-option-link' => array(
          '#type' => 'radio',
          '#title' => t('Link all'),
          '#attributes' => array(
            'name' => $tid . '-node-type-' . $node->type . '-option',
            'value' => 'link',
          ),
        ),
        $tid . '-node-type-' . $node->type . '-option-custom' => array(
          '#type' => 'radio',
          '#title' => t('Custom'),
          '#attributes' => array(
            'name' => $tid . '-node-type-' . $node->type . '-option',
            'value' => 'custom',
          ),
        ),
      );
      $options[$tid . '-node-type-' . $node->type . '-option-' . $default]['#attributes']['checked'] = 'checked';
      $content[$tid . '-node-type-' . $node->type . '-option'] = $options;
    }
    // Options for separate node.
    $options = array(
      $node->nid . '-node-title' => array(
        '#markup' => check_plain($node->title),
      ),
      $tid . '-node-' . $node->nid . '-option-none' => array(
        '#type' => 'radio',
        '#title' => t('Do nothing'),
        '#attributes' => array(
          'name' => $tid . '-node-' . $node->nid . '-' . $current_type . '-option',
          'value' => 'none',
          'class' => array(
            $current_type,
          ),
        ),
      ),
      $tid . '-node-' . $node->nid . '-option-copy' => array(
        '#type' => 'radio',
        '#title' => t('Copy'),
        '#attributes' => array(
          'name' => $tid . '-node-' . $node->nid . '-' . $current_type . '-option',
          'value' => 'copy',
          'class' => array(
            $current_type,
          ),
        ),
      ),
      $tid . '-node-' . $node->nid . '-option-link' => array(
        '#type' => 'radio',
        '#title' => t('Link'),
        '#attributes' => array(
          'name' => $tid . '-node-' . $node->nid . '-' . $current_type . '-option',
          'value' => 'link',
          'class' => array(
            $current_type,
          ),
        ),
      ),
    );
    $options[$tid . '-node-' . $node->nid . '-option-' . $default]['#attributes']['checked'] = 'checked';
    $content[$tid . '-node-' . $node->nid . '-option'] = $options;
  }
  return $content;
}

/**
 * Copier form validation callback.
 */
function taxonomy_tools_copier_copy_form_validate($form, &$form_state) {
  if ($form_state['triggering_element']['#type'] == 'submit') {
    $input = $form_state['input'];
    // Destination validation.
    if (!is_numeric($input['destination-vocabulary'])) {
      $message = t('At least destination vocabulary must be selected!');
      form_set_error('destination-vocabulary', $message);
    }
    $root = $form_state['values']['tid'];
    // Term validation.
    if ($input[$root . '-option'] == 0) {
      $message = t('At least highest level term must be selected for copying!');
      form_set_error($root . '-option', $message);
    }
  }
}

/**
 * Copier form submit callback.
 */
function taxonomy_tools_copier_copy_form_submit($form, &$form_state) {
  $input = $form_state['input'];
  $values = $form_state['values'];
  $tid = $values['tid'];
  // Redirection after copying if form destination is not set.
  if (!is_numeric(strpos($form['#action'], 'destination'))) {
    $form_state['redirect'] = 'taxonomy/term/' . $tid;
  }
  // Destination vocabulary and parent for first level term.
  $vid = $input['destination-vocabulary'];
  $parent = 0;
  $level = 1;
  while (isset($input['destination-level-' . $level])) {
    if (is_numeric($input['destination-level-' . $level])) {
      $parent = $input['destination-level-' . $level];
    }
    $level++;
  }
  // Call function that handles term copying.
  taxonomy_tools_copier_term_handler($tid, $vid, $parent, $input);
}

/**
 * Handles term copying.
 *
 * @param int $tid
 *   Taxonomy term identificator.
 * @param int $vid
 *   Vocabulary identificator.
 * @param int $parent
 *   Parent value for newly created taxonomy term.
 * @param array $input
 *   Copying form input values.
 */
function taxonomy_tools_copier_term_handler($tid, $vid, $parent, $input) {
  if ($input[$tid . '-option'] == 1) {
    // Copy term.
    $term = taxonomy_term_load($tid);
    // Allow other modules to change the term.
    drupal_alter('taxonomy_tools_copier_term_copy', $term);
    // Set/unset values.
    $term->tid = NULL;
    $term->vocabulary_machine_name = NULL;
    $term->vid = $vid;
    $term->parent = $parent;
    // Append suffix to term name.
    if (variable_get('taxonomy_tools_copier_mark_term_copies', FALSE)) {
      $term->name .= ' ' . variable_get('taxonomy_tools_copier_term_name_suffix', t('(duplicate)'));
    }
    // Save new term.
    taxonomy_term_save($term);
    if (isset($input[$tid . '-nodes-option']) && $input[$tid . '-nodes-option'] != 'none') {
      $option = $input[$tid . '-nodes-option'];
      if ($option == 'custom') {
        $node_types = node_type_get_types();
        foreach ($node_types as $type) {
          if (isset($input[$tid . '-node-type-' . $type->type . '-option'])) {
            $nodes = taxonomy_tools_copier_associated_nodes($tid, $type->type);
            $type_option = $input[$tid . '-node-type-' . $type->type . '-option'];
            if ($type_option == 'custom') {
              // Copy or link each separate node.
              foreach ($nodes as $node) {
                $node_option = $input[$tid . '-node-' . $node->nid . '-' . $type->type . '-option'];
                if (in_array($node_option, array('copy', 'link'))) {
                  taxonomy_tools_copier_node_handler($node->nid, $term, $node_option);
                }
              }
            }
            else {
              // Copy or link all associated nodes of current type.
              foreach ($nodes as $node) {
                taxonomy_tools_copier_node_handler($node->nid, $term, $type_option);
              }
            }
          }
        }
      }
      else {
        // Copy or link all associated nodes.
        $nodes = taxonomy_tools_copier_associated_nodes($tid);
        foreach ($nodes as $node) {
          taxonomy_tools_copier_node_handler($node->nid, $term, $option);
        }
      }
    }
    // Process children term copying.
    $children = taxonomy_get_children($tid);
    if ($children) {
      foreach ($children as $child) {
        taxonomy_tools_copier_term_handler($child->tid, $vid, $term->tid, $input);
      }
    }
  }
}

/**
 * Returns taxonomy term associated node identificators.
 *
 * @param int $tid
 *   Taxonomy term identificator.
 * @param string $bundle
 *   Node type.
 */
function taxonomy_tools_copier_associated_nodes($tid, $bundle = NULL) {
  $result = array();
  $query = db_select('node', 'n');
  $query->addField('n', 'nid');
  $query->addField('n', 'title');
  $query->addField('n', 'type');
  if ($bundle) {
    $query->condition('n.type', $bundle);
  }
  $query->join('taxonomy_index', 'ti', 'n.nid = ti.nid');
  $query->condition('ti.tid', $tid);
  $query->distinct();
  $query->orderBy('n.type');
  $result = $query->execute()->fetchAll();
  return $result;
}

/**
 * Returns the count of taxonomy term associated nodes.
 *
 * @param int $tid
 *   Taxonomy term identificator
 */
function taxonomy_tools_copier_associated_nodes_count($tid) {
  $result = 0;
  $query = db_select('taxonomy_index', 'foo');
  $query->addField('foo', 'nid');
  $query->condition('foo.tid', $tid);
  $result = $query->countQuery()->execute()->fetchField();
  return $result;
}

/**
 * Handles node copying or linking.
 *
 * @param int $nid
 *   Node identificator.
 * @param stdClass $term
 *   Taxonomy term object.
 * @param string $action
 *   Action what to do with the node.
 */
function taxonomy_tools_copier_node_handler($nid, $term, $action) {
  $node = node_load($nid);
  // Term reference fields that need to be filled.
  $fields = taxonomy_tools_copier_reference_fields($term->vocabulary_machine_name, $node->type, $action, $nid);
  // Make a brand new copy of an existing node and link it with the
  // newly created term.
  if ($action == 'copy') {
    global $user;
    // Set/unset values.
    $node->uid = $user->uid;
    $node->vid = NULL;
    $node->nid = NULL;
    $node->created = NULL;
    // Append suffix to node title.
    if (variable_get('taxonomy_tools_copier_mark_node_copies', FALSE)) {
      $node->title .= ' ' . variable_get('taxonomy_tools_copier_node_title_suffix', t('(duplicate)'));
    }
    foreach ($fields as $field) {
      $field_name = $field->field_name;
      $node->$field_name = array(
        LANGUAGE_NONE => array(
          0 => array(
            'tid' => $term->tid,
          ),
        ),
      );
    }
  }
  // Link an existing node with the newly created term.
  elseif ($action == 'link') {
    foreach ($fields as $field) {
      $field_name = $field->field_name;
      $field_values = $node->$field_name;
      array_push($field_values[LANGUAGE_NONE], array('tid' => $term->tid));
      $node->$field_name = $field_values;
    }
  }
  // Save the node.
  node_save($node);
}

/**
 * Returns term reference fields that need to be filled.
 *
 * @param string $machine_name
 *   Vocabulary machine name.
 * @param string $bundle
 *   Node type.
 * @param string $action
 *   Action what to do with the node.
 * @param int $nid
 *   Node identificator.
 */
function taxonomy_tools_copier_reference_fields($machine_name, $bundle, $action, $nid) {
  $result = array();
  // Get term reference fields.
  $query = db_select('field_config', 'fc');
  $query->addField('fc', 'field_name');
  $query->addField('fc', 'data');
  $query->addField('fc', 'cardinality');
  $query->condition('fc.deleted', 0);
  $query->condition('fc.type', 'taxonomy_term_reference');
  if ($action == 'link') {
    $query->condition('fc.cardinality', 1, '<>');
  }
  $query->join('field_config_instance', 'fci', 'fci.field_name = fc.field_name');
  $query->condition('fci.entity_type', 'node');
  $query->condition('fci.bundle', $bundle);
  $result = $query->execute()->fetchAll();
  // Filter out fields by vocabulary and free value slots.
  foreach ($result as $key => $field) {
    $data = unserialize($field->data);
    if ($data['settings']['allowed_values'][0]['vocabulary'] != $machine_name) {
      unset($result[$key]);
    }
    elseif ($action == 'link' && $field->cardinality != -1) {
      $values = taxonomy_tools_copier_field_value_count($field->field_name, $bundle, $nid);
      if ($values == $field->cardinality) {
        unset($result[$key]);
      }
    }
  }
  return $result;
}

/**
 * Returns how many values field already has.
 *
 * @param string $field_name
 *   Field name.
 * @param string $bundle
 *   Node type.
 * @param int $nid
 *   Node identificator.
 */
function taxonomy_tools_copier_field_value_count($field_name, $bundle, $nid) {
  $result = 0;
  $table = 'field_data_' . $field_name;
  $query = db_select($table, 'foo');
  $query->addField('foo', 'entity_id');
  $query->condition('foo.deleted', 0);
  $query->condition('foo.entity_type', 'node');
  $query->condition('foo.bundle', $bundle);
  $query->condition('foo.entity_id', $nid);
  $result = $query->countQuery()->execute()->fetchField();
  return $result;
}
