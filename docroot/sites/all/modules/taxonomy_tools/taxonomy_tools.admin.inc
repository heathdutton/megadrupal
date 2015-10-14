<?php

/**
 * @file
 * Administrative page callbacks for the Taxonomy Tools module.
 */

/**
 * Build taxonomy overview form.
 *
 * @param array $form
 *   An associative array representing form element.
 * @param array $form_state
 *   An associative array representing form_state element.
 * @param object $vocabulary
 *   A vocabulary object.
 * @param string $root
 *   Parent taxonomy term ID of the branch that is to be shown.
 *   NULL by default.
 *
 * @ingroup forms
 * @see theme_taxonomy_tools_overview()
 */
function taxonomy_tools_overview($form, &$form_state, $vocabulary, $root = NULL) {
  $in_page = FALSE;
  if (!$root) {
    drupal_set_title($vocabulary->name);
  }
  elseif (is_object($root)) {
    drupal_set_title($root->name);
  }
  elseif (is_numeric($root)) {
    $query = db_select('taxonomy_term_data', 'foo');
    $query->addField('foo', 'name');
    $query->condition('foo.tid', $root);
    $name = $query->execute()->fetchField();
    drupal_set_title($name);
  }
  // @todo Duplicate if statement
  if (is_object($root)) {
    $root = $root->tid;
  }
  if (!isset($vocabulary)) {
    // It means we are not in administrative pages.
    $in_page = TRUE;
    $query = db_select('taxonomy_term_data', 'foo');
    $query->addField('foo', 'vid');
    $query->condition('foo.tid', $root);
    $vid = $query->execute()->fetchField();
    $vocabulary = taxonomy_vocabulary_load($vid);
  }
  // Make sure Drupal Ajax framework javascript is around.
  drupal_add_library('system', 'drupal.ajax');
  if (isset($form_state['confirm_term_delete'])) {
    // Show term confirmation deletion form.
    return taxonomy_tools_confirm_term_delete($form, $form_state, $vocabulary, $root);
  }
  $form = array();
  $form['#attached']['js'][] = drupal_get_path('module', 'taxonomy_tools') . '/js/taxonomy_tools.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'taxonomy_tools') . '/style/taxonomy_tools.css';
  $form['#vocabulary'] = $vocabulary;
  // Link for adding new term.
  $href = '';
  $destination = drupal_get_destination();
  $href = $destination['destination'] . '/add';
  if (is_numeric(strpos($destination['destination'], 'taxonomy/term'))) {
    $href .= '/' . $vocabulary->machine_name;
  }
  $form['add_term'] = array(
    '#type' => 'link',
    '#title' => t('Add term'),
    '#href' => $href,
    '#attributes' => array(
      'title' => t('Add term'),
    ),
    '#options' => array(
      'query' => array(
        'destination' => $destination['destination'],
      ),
    ),
    '#access' => taxonomy_tools_overview_access($vocabulary, TRUE),
  );
  if ($root) {
    // Link to move up the term hierarchy.
    $href = '';
    $query = db_select('taxonomy_term_hierarchy', 'foo');
    $query->addField('foo', 'parent');
    $query->condition('foo.tid', $root);
    $parent = $query->execute()->fetchField();
    if (($in_page && $parent > 0) || !$in_page) {
      // @todo Implement use of destination variables to
      // achieve more universal approach.
      // Correct href depending on wether we are in administrative
      // pages or not.
      if ($in_page) {
        $href = 'taxonomy/term/' . $parent . '/overview';
      }
      else {
        $href = 'admin/structure/taxonomy/' . $vocabulary->machine_name . '/overview';
        if ($parent > 0) {
          $href .= '/' . $parent;
        }
      }
      $form['level_up'] = array(
        '#type' => 'link',
        '#title' => t('Up one level'),
        '#href' => $href,
        '#attributes' => array(
          'title' => t('Up one level'),
        ),
      );
    }
  }
  // Fetch all the necessary information from the database.
  $query = db_select('taxonomy_term_data', 'db1');
  $query->addField('db1', 'tid');
  $query->addField('db1', 'name');
  $query->addField('db1', 'weight');
  $query->condition('db1.vid', $vocabulary->vid);
  $query->join('taxonomy_term_hierarchy', 'db2', 'db1.tid = db2.tid');
  if (!$root) {
    $root = 0;
  }
  $query->condition('db2.parent', $root);
  if (module_exists('taxonomy_tools_publisher')) {
    $config = array_filter(variable_get('taxonomy_tools_publisher_config', array()));
    if (!empty($config) && in_array($vocabulary->machine_name, $config)) {
      $query->leftJoin('field_data_field_taxonomy_term_status', 'db3', 'db1.tid = db3.entity_id');
      $query->addField('db3', 'field_taxonomy_term_status_value', 'status');
      $query->leftJoin('field_data_field_taxonomy_term_publish_on', 'db4', 'db1.tid = db4.entity_id');
      $query->addField('db4', 'field_taxonomy_term_publish_on_value', 'publish_on');
      $query->leftJoin('field_data_field_taxonomy_term_unpublish_on', 'db5', 'db1.tid = db5.entity_id');
      $query->addField('db5', 'field_taxonomy_term_unpublish_on_value', 'unpublish_on');
    }
  }
  $query->orderBy('db1.weight');
  $query->orderBy('db1.name');
  $result = $query->execute()->fetchAll();
  $length = count($result);
  $i = 1;
  $form['tree_root'] = array(
    '#type' => 'hidden',
    '#value' => $root,
  );
  foreach ($result as $term) {
    $form[$term->tid] = array();
    // Term name.
    $href = '';
    if (arg(3) == 'overview') {
      $href = 'taxonomy/term/' . $term->tid . '/overview';
    }
    else {
      $href = 'admin/structure/taxonomy/' . $vocabulary->machine_name . '/overview/' . $term->tid;
    }
    $form[$term->tid]['name'] = array(
      '#type' => 'link',
      '#title' => check_plain($term->name),
      '#href' => $href,
    );
    // Term children count.
    $query = db_select('taxonomy_term_hierarchy', 'foo');
    $query->addField('foo', 'tid');
    $query->condition('foo.parent', $term->tid);
    $child_count = $query->countQuery()->execute()->fetchField();
    $form[$term->tid]['child_count'] = array(
      '#markup' => '<span class="term-child-count">' . $child_count . '</span>',
    );
    // Informative icons.
    $icons = module_invoke_all('taxonomy_tools_overview_info_icons', $term->tid);
    foreach ($icons as $key => $icon) {
      $form[$term->tid]['info_icons'][$key] = array(
        '#markup' => theme_image($icon),
      );
    }
    // Term weight.
    $form[$term->tid]['weight_' . $term->tid] = array(
      '#type' => 'weight',
      '#delta' => $length,
      '#title_display' => 'invisible',
      '#title' => t('Weight for added term'),
      '#default_value' => $term->weight,
      '#access' => (user_access('administer taxonomy') || user_access('edit terms in ' . $vocabulary->vid)),
    );
    // @todo Hook to alter operations
    // View link.
    $form[$term->tid]['operations'] = array();
    $view_link = 'taxonomy/term/' . $term->tid;
    $form[$term->tid]['operations']['view'] = array(
      '#type' => 'link',
      '#title' => '',
      '#href' => $view_link,
      '#attributes' => array(
        'class' => array(
          'view-term',
        ),
        'title' => t('view'),
      ),
    );
    // Edit link.
    $edit_link = 'taxonomy/term/' . $term->tid . '/edit';
    $destination = drupal_get_destination();
    $form[$term->tid]['operations']['edit'] = array(
      '#type' => 'link',
      '#title' => '',
      '#href' => $edit_link,
      '#attributes' => array(
        'class' => array(
          'edit-term',
        ),
        'title' => t('edit'),
      ),
      '#options' => array(
        'query' => array(
          'destination' => $destination['destination'],
        ),
      ),
      '#access' => (user_access('administer taxonomy') || user_access('edit terms in ' . $vocabulary->vid)),
    );
    // Any other operation links added by other modules.
    $additional_links = module_invoke_all('taxonomy_tools_overview_links', $term->tid);
    foreach ($additional_links as $key => $link) {
      $form[$term->tid]['operations'][$key] = $link;
    }
    // Deletion checkbox.
    $form[$term->tid]['delete_' . $term->tid] = array(
      '#type' => 'checkbox',
      '#title' => t('delete'),
      '#title_display' => 'invisible',
      '#access' => (user_access('administer taxonomy') || user_access('delete terms in ' . $vocabulary->vid)),
    );
    if (module_exists('taxonomy_tools_publisher')) {
      // Publishing status.
      if (!isset($term->status) || $term->status == 1) {
        $status = t('Published.');
        if (isset($term->unpublish_on)) {
          $status .= ' ';
          $status .= t('Will be unpublished on :date', array(
            ':date' => format_date($term->unpublish_on,
            'custom',
            TAXONOMY_PUBLISHER_MESSAGE_DATE_FORMAT))
          );
        }
      }
      else {
        $status = t('Unpublished.');
        if (isset($term->publish_on)) {
          $status .= ' ';
          $status .= t('Will be published on :date.', array(
            ':date' => format_date($term->publish_on,
            'custom',
            TAXONOMY_PUBLISHER_MESSAGE_DATE_FORMAT))
          );
        }
        if (isset($term->unpublish_on)) {
          $status .= ' ';
          $status .= t('Will be unpublished on :date.', array(
            ':date' => format_date($term->unpublish_on,
            'custom',
            TAXONOMY_PUBLISHER_MESSAGE_DATE_FORMAT))
          );
        }
      }
      $form[$term->tid]['status'] = array(
        '#markup' => $status,
      );
    }
    if (module_exists('taxonomy_tools_role_access')) {
      // Buttons for changing term access for user roles.
      $config = array_filter(variable_get('taxonomy_tools_role_access_vocab_config', array()));
      if (!empty($config) && in_array($vocabulary->machine_name, $config)) {
        $config = variable_get('taxonomy_tools_role_access_role_config', array());
        if (!empty($config)) {
          $roles = user_roles();
          foreach ($roles as $rid => $role) {
            if (in_array($rid, $config)) {
              $access = taxonomy_tools_role_access_get_access($term->tid, $rid);
              $attributes = array(
                'class' => array(
                  'use-ajax',
                  'access_' . $term->tid . '_' . $rid,
                ),
                'title' => t('change'),
              );
              if ($access) {
                $attributes['class'][] = 'true';
              }
              else {
                $attributes['class'][] = 'false';
              }
              $opposite = $access ? 'false' : 'true';
              $link = 'taxonomy-role-access/nojs/' . $vocabulary->machine_name . '/' . $term->tid . '/' . $rid . '/' . $opposite;
              if ($root > 0) {
                $link .= '/' . $root;
              }
              $form[$term->tid]['access'][$role] = array(
                '#type' => 'link',
                '#title' => '',
                '#href' => $link,
                '#attributes' => $attributes,
                '#access' => user_access('administer taxonomy role access'),
              );
            }
          }
        }
      }
    }
    $i++;
  }
  $description = t('The number of child taxonomy terms is displayed next to the taxonomy term name. If the current overview page location is other than root level, the first row of the table will display a link with the functionality to go up one level.');
  $form['description'] = array(
    '#markup' => '<div class="description">' . $description . '</div>',
  );
  $form['actions']['save_order'] = array(
    '#type' => 'submit',
    '#value' => t('Save order'),
    '#access' => (user_access('administer taxonomy') || user_access('edit terms in ' . $vocabulary->vid)),
  );
  $form['actions']['delete_terms'] = array(
    '#type' => 'submit',
    '#value' => t('Delete terms'),
    '#access' => (user_access('administer taxonomy') || user_access('delete terms in ' . $vocabulary->vid)),
  );
  return $form;
}

/**
 * Returns HTML for a taxonomy overview form as a table.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 * @see taxonomy_tools_overview()
 */
function theme_taxonomy_tools_overview($variables) {
  $form = $variables['form'];
  if ((user_access('administer taxonomy') || user_access('edit terms in ' . $form['#vocabulary']->vid))) {
    drupal_add_tabledrag('taxonomy-tools-overview-table', 'order', 'sibling', 'term-weight');
  }
  $rows = array();
  foreach (element_children($form) as $key) {
    if (is_numeric($key)) {
      $row = array();
      $data = &$form[$key];
      $row[] = drupal_render($data['name']) . drupal_render($data['child_count']);
      if (isset($data['info_icons'])) {
        $row[0] .= drupal_render($data['info_icons']);
      }
      $data['weight_' . $key]['#attributes']['class'] = array('term-weight');
      if ((user_access('administer taxonomy') || user_access('edit terms in ' . $form['#vocabulary']->vid))) {
        $row[] = drupal_render($data['weight_' . $key]);
      }
      $row[] = drupal_render($data['operations']);
      if ((user_access('administer taxonomy') || user_access('delete terms in ' . $form['#vocabulary']->vid))) {
        $row[] = drupal_render($data['delete_' . $key]);
      }
      if (module_exists('taxonomy_tools_publisher')) {
        $row[] = drupal_render($data['status']);
      }
      if (module_exists('taxonomy_tools_role_access') && isset($data['access']) && user_access('administer taxonomy role access')) {
        foreach (element_children($data['access']) as $role) {
          $row[] = drupal_render($data['access'][$role]);
        }
      }
      $row = array(
        'data' => $row,
        'class' => array('row_' . $key, 'draggable'),
      );
      $rows[$key] = $row;
    }
  }
  $header = array();
  $header[] = array(
    'data' => t('Name'),
    'class' => array('table-header'),
  );
  if ((user_access('administer taxonomy') || user_access('edit terms in ' . $form['#vocabulary']->vid))) {
    $header[] = array(
      'data' => t('Order'),
      'class' => array('table-header'),
    );
  }
  $header[] = array(
    'data' => t('Operations'),
    'class' => array('table-header'),
  );
  if ((user_access('administer taxonomy') || user_access('delete terms in ' . $form['#vocabulary']->vid))) {
    $header[] = array(
      'data' => t('Delete'),
      'class' => array(
        'table-header',
        'term-delete',
      ),
    );
  }
  if (module_exists('taxonomy_tools_publisher')) {
    $header[] = array(
      'data' => t('Status'),
      'class' => array('table-header'),
    );
  }
  if (module_exists('taxonomy_tools_role_access') && isset($data['access']) && user_access('administer taxonomy role access')) {
    foreach (element_children($data['access']) as $role) {
      $header[] = array(
        'data' => t('Access for :role', array(':role' => $role)),
        'class' => array('table-header'),
      );
    }
  }
  if (empty($rows)) {
    $empty_data = t('Empty branch.');
    if (taxonomy_tools_overview_access($form['#vocabulary'], TRUE)) {
      $url = $form['add_term'];
      $empty_data .= '&nbsp;' . render($url);
    }
    if (isset($form['level_up'])) {
      $rows[] = array(
        'data' => array(
          array(
            'data' => drupal_render($form['level_up']),
            'colspan' => count($header),
          ),
        ),
        'class' => array('level-up'),
      );
    }
    $rows[] = array(array('data' => $empty_data, 'colspan' => count($header)));
  }
  elseif (isset($form['level_up'])) {
    $level_up = array(
      'data' => array(
        array(
          'data' => drupal_render($form['level_up']),
          'colspan' => count($header),
        ),
      ),
      'class' => array('level-up'),
    );
    array_unshift($rows, $level_up);
  }
  $output = '';
  $output .= drupal_render($form['add_term']);
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'taxonomy-tools-overview-table'),
    'sticky' => FALSE)
  );
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Validation handler for Taxonomy Tools overview form.
 *
 * @see taxonomy_tools_overview()
 */
function taxonomy_tools_overview_validate($form, &$form_state) {
  $selected = FALSE;
  if (!isset($form_state['values']['confirm']) && $form_state['triggering_element']['#value'] == t('Delete terms')) {
    // Check if any term is selected for deletion.
    foreach ($form_state['values'] as $key => $value) {
      if (is_numeric(strpos($key, 'delete_')) && $value == 1) {
        $selected = TRUE;
        break;
      }
    }
    if (!$selected) {
      form_set_error('op', t('At least on term must be selected for deletion!'));
    }
  }
}

/**
 * Submit handler for Taxonomy Tools overview form.
 *
 * @see taxonomy_tools_overview()
 */
function taxonomy_tools_overview_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == t('Delete terms')) {
    // Execute term deletion.
    if (isset($form_state['values']['term_delete']) && $form_state['values']['term_delete'] === TRUE) {
      return taxonomy_tools_confirm_term_delete_submit($form, $form_state);
    }
    // Rebuild the form to confirm the term deletion.
    $form_state['rebuild'] = TRUE;
    $form_state['confirm_term_delete'] = TRUE;
    return;
  }
  elseif ($form_state['triggering_element']['#value'] == t('Save order')) {
    // Fetch all the necessary information from the database.
    $query = db_select('taxonomy_term_data', 'db1');
    $query->addField('db1', 'tid');
    $query->addField('db1', 'weight');
    $query->condition('db1.vid', $form['#vocabulary']->vid);
    $query->join('taxonomy_term_hierarchy', 'db2', 'db1.tid = db2.tid');
    $query->condition('db2.parent', $form_state['values']['tree_root']);
    $result = $query->execute()->fetchAllKeyed();
    foreach ($result as $tid => $weight) {
      if ($form_state['values']['weight_' . $tid] != $weight) {
        // Update term weight value.
        $query = db_update('taxonomy_term_data');
        $query->fields(
          array(
            'weight' => $form_state['values']['weight_' . $tid],
          )
        );
        $query->condition('tid', $tid);
        $query->execute();
      }
    }
  }
}

/**
 * Build term deletion confirmation form.
 */
function taxonomy_tools_confirm_term_delete($form, &$form_state, $vocabulary, $root) {
  // Prepare the identifiers of terms that will be deleted.
  foreach ($form_state['values'] as $key => $value) {
    if (is_numeric(strpos($key, 'delete_')) && $value == 1) {
      $explode = explode('_', $key);
      $form[$key] = array(
        '#type' => 'value',
        '#value' => $explode[1],
      );
    }
  }
  // Identifies that term deletion is confirmed.
  $form['term_delete'] = array(
    '#type' => 'value',
    '#value' => TRUE,
  );
  return confirm_form(
    $form,
    t('Are you sure you want to delete the selected terms?'),
    $_GET['q'],
    t('Deleted terms will be removed permanently!'),
    t('Delete terms'),
    t('Cancel')
  );
}

/**
 * Submit handler for term deletion confirmation form.
 *
 * @see taxonomy_tools_confirm_term_delete()
 */
function taxonomy_tools_confirm_term_delete_submit($form, &$form_state) {
  // Delete selected terms.
  foreach ($form_state['values'] as $key => $value) {
    if (is_numeric(strpos($key, 'delete_'))) {
      taxonomy_term_delete($value);
    }
  }
  drupal_set_message(t('Selected terms deleted.'));
  $form_state['redirect'] = $_GET['q'];
}
