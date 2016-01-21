<?php

/**
 * @file
 * Blocks for the document_library module.
 */

/**
 * Document Library Documents listing block with sort, filter and search forms.
 */
function document_library_block() {
  $state = _document_library_state();

  $content = array();

  $content['#attached']['css'][] = drupal_get_path('module', 'document_library') . '/css/document_library.css';
  $content['#attached']['js'][]  = drupal_get_path('module', 'document_library') . '/js/document_library.js';
  $content['#attached']['js'][]  = array(
    'data' => array(
      'document_library' => array(
        'search_label'         => variable_get('document_library_search_label', t('SEARCH')),
        'search_auto_submit'   => variable_get('document_library_sort_js_submit', 1),
        'show_filter_form'     => variable_get('document_library_allow_filtering', 1),
        'show_advanced_search' => variable_get('document_library_search_options_label', t('Show advanced search options')),
        'hide_advanced_search' => variable_get('document_library_search_options_open_label', t('Hide advanced search options')),
        'filter_form_status'   => ($state->filtered) ? 1 : 0,
      ),
    ),
    'type' => 'setting',
  );

  // Only print search/filter form if configured to do so.
  if (variable_get('document_library_allow_searching', 1)) {
    $content['search_form'] = drupal_get_form('document_library_search_form');

    if (variable_get('document_library_allow_filtering', 1)) {
      if ($state->filtered) {
        $classes = array('document-library-filter-form-open');
      }
      else {
        $classes = array('document-library-filter-form-closed');
      }

      $content['filter_form_container'] = array(
        '#type'       => 'container',
        '#attributes' => array(
          'id'    => 'document-library-filter-form-container',
          'class' => $classes,
        ),
      );

      $content['filter_form_container']['filter_form'] = drupal_get_form('document_library_filter_form');
    }
  }

  // Only print sort form if configured to do so.
  if (variable_get('document_library_allow_sorting', 1) && _document_library_sort_field()) {
    $content['sort_form'] = drupal_get_form('document_library_sort_form');
  }

  $query = _document_library_build_query();

  // Set up pager if required.
  $per = variable_get('document_library_per_page', 10);
  if ($per > 0) {
    $total = $query->countQuery()
      ->execute()
      ->fetchField();

    $page = pager_find_page();

    $query->extend('PagerDefault')
      ->limit($per)
      ->range($page * $per, $per);
  }

  $data = $query->execute();

  $content['results'] = array(
    '#type'       => 'container',
    '#attributes' => array(
      'class' => 'document-library-results',
    ),
    '#prefix' => '<a name="results" id="results">&nbsp;</a>',
  );

  if ($data->rowCount()) {
    foreach ($data as $record) {
      $content['results'][] = array(
        '#theme' => 'document_library_document',
        '#node'   => node_load($record->nid),
      );
    }
    if ($per > 0) {
      $content['results']['pager'] = array(
        '#pre_render' => array('_document_library_intialize_pager'),
        '#theme' => 'pager',
        '#variables' => array(
          'total' => $total,
        ),
      );
    }
  }
  else {
    $content['results']['no_results'] = array(
      '#type'   => 'markup',
      '#markup' => check_plain(variable_get('document_library_no_data_message', t('There are currently no documents in the library that match your search criteria.'))),
    );
  }

  return $content;
}

/**
 * Document Library Folder block.
 */
function document_library_folder_block() {
  $state   = _document_library_state();
  $field   = variable_get('document_library_default_folder_list', 'library_document_category');
  $folders = _document_library_get_folders($field);

  // Get the selected folders.
  $selected = array();
  if ($state->filtered) {
    if (isset($state->filters[$field])) {
      $selected = $state->filters[$field];
    }
  }

  // Set the active trail.
  foreach ($folders as $term) {
    if (count($selected) && in_array($term->name, $selected)) {
      $term->active_trail = TRUE;
      _document_library_calculate_trail_recursive($term, $folders);
    }
  }

  // Output the block.
  $content = array();

  $content['#attached']['css'][] = drupal_get_path('module', 'document_library') . '/css/document_library.css';

  $content['document_library_menu'] = array(
    '#type'       => 'container',
    '#attributes' => array(
      'class' => 'document-library-menu',
    ),
  );

  // Only output top level folder if configured to do so.
  if (strtolower(variable_get('document_library_folder_label')) != '<none>') {
    $content['document_library_menu']['document_library_menu_home'] = array(
      '#type'       => 'markup',
      '#markup'     => l(
        t('<span class="document-menu-top-open"></span>@label', array('@label' => variable_get('document_library_folder_label', t('Documents')))),
        request_path(),
        array(
          'attributes' => array(
            'class' => array(
              'document-home',
            ),
          ),
          'html'       => TRUE,
          'fragment'   => 'results',
        )
      ),
    );
  }

  $content['document_library_menu']['document_library_menu_items'] = array(
    '#type'       => 'ul',
    '#items'      => _document_library_folders_recursive($field, $folders),
    '#theme'      => 'item_list',
  );

  return $content;
}

/**
 * Search form.
 */
function document_library_search_form($form, &$form_state) {
  $state = _document_library_state();

  $form['document_library_search'] = array(
    '#type'             => 'fieldset',
    '#title'            => check_plain(variable_get('document_library_search_legend', t('Search Documents'))),
  );

  $form['document_library_search']['query'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Search'),
    '#default_value'    => $state->query,
    '#max_length'       => 255,
  );

  $form['document_library_search']['search-submit'] = array(
    '#type'             => 'submit',
    '#value'            => check_plain(variable_get('document_library_search_button_label', t('GO'))),
  );

  if (variable_get('document_library_allow_filtering', 1)) {
    $form['document_library_search']['filter_toggle'] = array(
      '#type'       => 'container',
      '#attributes' => array(
        'class' => array(
          'document-library-filter-toggle',
        ),
      ),
    );

    $form['document_library_search']['filter_toggle']['toggle_link'] = array(
      '#markup' => l(
        $state->filtered ? variable_get('document_library_search_options_open_label', t('Hide advanced search options')) : variable_get('document_library_search_options_label', t('See advanced search options')),
        '',
        array(
          'attributes' => array(
            'id' => 'document-library-filter-form-toggle',
          ),
          'fragment'   => 'toggle',
        )
      ),
    );
  }

  if (variable_get('document_library_show_link', 1)) {
    if (!$state->default) {
      $form['document_library_search']['filter_link'] = array(
        '#type'       => 'container',
        '#attributes' => array(
          'class' => array(
            'document-library-link',
          ),
        ),
      );

      $form['document_library_search']['filter_link']['permalink'] = array(
        '#markup' => l(
          variable_get('document_library_show_link_label', t('Permalink')),
          '',
          array(
            'attributes' => array(
              'title' => check_plain(variable_get('document_library_show_link_title_label', t('Copy this link to share your filtered documents with others.'))),
            ),
            'query' => $state->permalink,
          )
        ),
      );
    }
  }

  $form['actions']['#submit'][] = 'document_library_search_form_submit';

  // Remove the query if we submit the filter form.
  $form['#action'] = strtok(request_uri(), '?') . '#results';

  return $form;
}

/**
 * Search form submit.
 */
function document_library_search_form_submit($form, &$form_state) {
  $_SESSION['DOCUMENT_LIBRARY_QUERY'] = $form_state['values']['query'];

  // Clear query if query is technically blank.
  if (strtolower($_SESSION['DOCUMENT_LIBRARY_QUERY']) == strtolower(variable_get('document_library_search_label', t('SEARCH')))) {
    unset($_SESSION['DOCUMENT_LIBRARY_QUERY']);
  }
}

/**
 * Sort form.
 */
function document_library_sort_form($form, &$form_state) {
  $state = _document_library_state();

  $form['document_library_sort'] = array(
    '#type'             => 'fieldset',
    '#title'            => check_plain(variable_get('document_library_sort_legend', t('Sort Documents'))),
  );

  $options = array();

  if (variable_get('document_library_use_title_asc_sort', 1)) {
    $options['1'] = variable_get('document_library_sorting_title_asc_label', t('Title: A-Z'));
  }

  if (variable_get('document_library_use_title_desc_sort', 1)) {
    $options['2'] = variable_get('document_library_sorting_title_desc_label', t('Title: Z-A'));
  }

  if (variable_get('document_library_use_date_asc_sort', 1)) {
    $options['3'] = variable_get('document_library_sorting_date_asc_label', t('Date: newest to oldest'));
  }

  if (variable_get('document_library_use_date_desc_sort', 1)) {
    $options['4'] = variable_get('document_library_sorting_date_desc_label', t('Date: oldest to newest'));
  }

  $form['document_library_sort']['sort'] = array(
    '#type'             => 'select',
    '#title'            => check_plain(variable_get('document_library_sorting_label', t('Sort by'))),
    '#options'          => $options,
    '#default_value'    => $state->sort,
  );

  $form['document_library_sort']['sort-submit'] = array(
    '#type'             => 'submit',
    '#value'            => check_plain(variable_get('document_library_sort_button_label', t('GO'))),
  );

  // Remove the query if we submit the filter form.
  $form['#action'] = strtok(request_uri(), '?') . '#results';

  return $form;
}

/**
 * Sort form submit.
 */
function document_library_sort_form_submit($form, &$form_state) {
  $_SESSION['DOCUMENT_LIBRARY_SORT'] = $form_state['values']['sort'];
}

/**
 * Filter form.
 */
function document_library_filter_form($form, &$form_state) {
  $state = _document_library_state();

  $field_instances = field_info_instances('node', 'document_library_document');

  $form['#parents'] = array();
  $form['document_library_filter'] = array(
    '#type'             => 'fieldset',
    '#title'            => check_plain(variable_get('document_library_filter_legend', t('Filter Documents'))),
  );

  foreach ($field_instances as $field_name => $field_instance) {
    $field_info = field_info_field($field_name);
    if ($field_info['type'] == 'taxonomy_term_reference') {
      if (variable_get('document_library_' . $field_name . '_filter_use', 1)) {

        $field_form = field_default_form('node', NULL, $field_info, $field_instance, $state->language, array(), $form, $form_state);

        $field_form[$field_name][$state->language]['#title']         = variable_get('document_library_' . $field_name . '_filter_label', '');
        $field_form[$field_name][$state->language]['#description']   = '';

        // Autocomplete needs a flattened array of names.
        if (isset($state->filters[$field_name])) {
          if ($field_instance['widget']['type'] == 'taxonomy_autocomplete') {
            $terms = array();
            foreach ($state->filters[$field_name] as $tid) {
              $term = taxonomy_term_load($tid);
              $terms[] = $term->name;
            }
            $field_form[$field_name][$state->language]['#default_value'] = drupal_implode_tags($terms);
          }
          // All others need an array of ids.
          else {
            $field_form[$field_name][$state->language]['#default_value'] = $state->filters[$field_name];
          }
        }

        $form['document_library_filter']['document_library_' . $field_name . '_filter'][$field_name] = $field_form[$field_name];
      }
    }
  }

  $form['document_library_filter']['actions']['submit'] = array(
    '#type'             => 'submit',
    '#name'             => 'filter-submit',
    '#value'            => check_plain(variable_get('document_library_filter_button_label', t('Get Results'))),
  );

  if ($state->filtered) {
    $form['document_library_filter']['actions']['reset'] = array(
      '#type'             => 'submit',
      '#name'             => 'filter-reset',
      '#value'            => check_plain(variable_get('document_library_filter_reset_label', t('Reset'))),
    );
  }

  // Remove the query if we submit the filter form.
  $form['#action'] = strtok(request_uri(), '?') . '#results';

  return $form;
}

/**
 * Filter form submit.
 */
function document_library_filter_form_submit($form, &$form_state) {
  // Reset button.
  if (isset($form_state['values']['filter-reset'])) {
    $_SESSION['DOCUMENT_LIBRARY_FIELDS'] = array();
    $_SESSION['DOCUMENT_LIBRARY_QUERY']  = '';
    $_SESSION['DOCUMENT_LIBRARY_SORT']   = '';

    return;
  }

  $state = _document_library_state();

  $field_instances = field_info_instances('node', 'document_library_document');

  // Set values in session.
  foreach ($field_instances as $field_name => $field_instance) {
    $field_info = field_info_field($field_name);
    if ($field_info['type'] == 'taxonomy_term_reference') {

      unset($_SESSION['DOCUMENT_LIBRARY_FIELDS'][$field_name]);

      if (count($form_state['values'][$field_name])) {
        foreach ($form_state['values'][$field_name][$state->language] as $term) {
          if (!empty($term['tid'])) {
            $_SESSION['DOCUMENT_LIBRARY_FIELDS'][$field_name][] = $term['tid'];
          }
        }
      }
    }
  }
}

/**
 * Retrieve documents data.
 */
function _document_library_build_query() {
  $state = _document_library_state();

  $query = db_select('node', 'n')
    ->distinct()
    ->fields('n')
    ->condition('n.type', 'document_library_document')
    ->condition('n.status', 1)
    ->addTag('node_access');

  if ($state->filtered) {
    foreach ($state->filters as $field => $terms) {
      $field_info = field_info_field($field);
      if ($field_info['type'] == 'taxonomy_term_reference') {
        if (variable_get('document_library_' . $field . '_filter_use', 1)) {
          if (count($terms)) {

            // Check to make sure the field exists before trying to join it.
            $result = db_query('SHOW TABLES LIKE :table', array(':table' => 'field_data_' . $field));
            if ($result->rowCount() == 1) {
              $query->leftjoin('field_data_' . $field, 'field_data_' . $field, 'field_data_' . $field . '.entity_id = n.nid');
              $query->condition($field . '_tid', $terms);
            }
          }
        }
      }
    }
  }

  // Add search terms to the query.
  if (isset($state->query) && drupal_strlen($state->query)) {
    $query->join('search_dataset', 's', 's.sid = n.nid');

    $keys = explode(' ', $state->query);
    foreach ($keys as $key) {
      $query->condition('s.data', '%' . $key . '%', 'LIKE');
    }
  }

  // Add order by clause.
  switch ($state->sort) {
    // Date: oldest to newest.
    // Date: newest to oldest.
    case 4:
    case 3:
      $sort_field = _document_library_sort_field();
      if ($sort_field) {
        $query->leftjoin('field_data_' . $sort_field, 'fd', 'fd.entity_id = n.nid');

        // Date: oldest to newest.
        if ($state->sort == 4) {
          $query->orderBy('fd.' . $sort_field . '_value');
        }
        // Date: newest to oldest.
        else {
          $query->orderBy('fd.' . $sort_field . '_value', 'DESC');
        }
      }
      break;

    // Title: Z-A.
    case 2:
      $query->orderBy('n.title', 'DESC');
      break;

    // Title: A-Z.
    default:
      $query->orderBy('n.title');
      break;
  }

  return $query;
}

/**
 * Determine the state of the search, filter and sort forms.
 */
function _document_library_state() {

  static $state;

  // Only create state once.
  if (!isset($state)) {
    $state            = new stdClass();
    $state->filters   = array();
    $state->query     = '';
    $state->sort      = variable_get('document_library_default_sort_order', 1);
    $state->filtered  = FALSE;
    $state->default   = TRUE;
    $state->permalink = array();
    $state->language  = LANGUAGE_NONE;

    $params = drupal_get_query_parameters();

    // Use node language if it is available.
    if ($node = menu_get_object()) {
      $state->language = $node->language;
    }

    // Get a list of fields to compare against.
    $field_instances = field_info_instances('node', 'document_library_document');
    $fields          = array();
    foreach ($field_instances as $field_name => $field_instance) {
      $field_info    = field_info_field($field_name);
      if ($field_info['type'] == 'taxonomy_term_reference') {
        $fields[str_replace('field_', '', $field_info['field_name'])] = TRUE;
      }
    }
    $fields = array_keys($fields);

    if (count($params)) {
      // Query resets session.
      $_SESSION['DOCUMENT_LIBRARY_FIELDS'] = array();
      $_SESSION['DOCUMENT_LIBRARY_QUERY']  = "";
      $_SESSION['DOCUMENT_LIBRARY_SORT']   = "";

      // Set session with query parameters.
      if (isset($params['page']) && drupal_strlen($params['page'])) {
        unset($params['page']);
      }
      if (isset($params['query']) && drupal_strlen($params['query'])) {
        $_SESSION['DOCUMENT_LIBRARY_QUERY'] = $params['query'];
        unset($params['query']);
      }
      if (isset($params['sort']) && drupal_strlen($params['sort'])) {
        $_SESSION['DOCUMENT_LIBRARY_SORT'] = $params['sort'];
        unset($params['sort']);
      }
      if (count($params)) {
        foreach ($params as $field => $terms) {
          if (in_array($field, $fields)) {
            if (is_array($terms)) {
              foreach ($terms as $term) {
                $_SESSION['DOCUMENT_LIBRARY_FIELDS']['field_' . $field][] = $term;
              }
            }
            // From folder block or term link.
            else {
              $_SESSION['DOCUMENT_LIBRARY_FIELDS']['field_' . $field][] = $terms;
            }
          }
        }
      }
    }

    // Build state from session.
    if (isset($_SESSION['DOCUMENT_LIBRARY_QUERY']) && drupal_strlen($_SESSION['DOCUMENT_LIBRARY_QUERY'])) {
      $state->query   = $_SESSION['DOCUMENT_LIBRARY_QUERY'];
      $state->default = FALSE;

      $state->permalink["query"] = $_SESSION['DOCUMENT_LIBRARY_QUERY'];
    }
    if (isset($_SESSION['DOCUMENT_LIBRARY_SORT']) && drupal_strlen($_SESSION['DOCUMENT_LIBRARY_SORT'])) {
      $state->sort    = $_SESSION['DOCUMENT_LIBRARY_SORT'];
      $state->default = FALSE;

      $state->permalink["sort"] = $_SESSION['DOCUMENT_LIBRARY_SORT'];
    }
    if (isset($_SESSION['DOCUMENT_LIBRARY_FIELDS'])) {
      foreach ($_SESSION['DOCUMENT_LIBRARY_FIELDS'] as $field => $terms) {
        if (in_array(str_replace('field_', '', $field), $fields)) {
          $state->default  = FALSE;
          $state->filtered = TRUE;
          foreach ($terms as $term) {
            $state->filters[$field][] = $term;

            $state->permalink[str_replace('field_', '', $field)][] = $term;
          }
        }
      }
    }
  }

  return $state;
}

/**
 * Reverse map mime-types to extensions.
 */
function _document_library_get_mime_types() {
  include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';

  $map = file_mimetype_mapping();

  $mime_map = array();
  foreach ($map['extensions'] as $extension => $mime) {
    $mime_map[$map['mimetypes'][$mime]] = $extension;
  }

  return $mime_map;
}

/**
 * Get vocabularies that can be used for folders.
 */
function _document_library_get_vocabularies_for_folders() {
  $field_instances = field_info_instances('node', 'document_library_document');
  $fields          = array();
  foreach ($field_instances as $field_name => $field_instance) {
    $field_info = field_info_field($field_name);
    if ($field_info['type'] == 'taxonomy_term_reference') {
      $fields[$field_info['field_name']] = $field_instance['label'];
    }
  }

  return $fields;
}

/**
 * Get the terms used for folders.
 */
function _document_library_get_folders($_field_name = '') {
  $field_instances = field_info_instances('node', 'document_library_document');
  $tree            = array();
  foreach ($field_instances as $field_name => $field_instance) {
    $field_info = field_info_field($field_name);

    if ($field_info['type'] == 'taxonomy_term_reference' && $field_info['field_name'] == $_field_name) {
      $vocabulary = taxonomy_vocabulary_machine_name_load($field_info['settings']['allowed_values'][0]['vocabulary']);
      $tree       = taxonomy_get_tree($vocabulary->vid);
    }
  }

  return $tree;
}

/**
 * Build folder trail at a specific depth (recursively used).
 */
function _document_library_calculate_trail_recursive($_term, $_tree) {
  foreach ($_term->parents as $parent) {
    foreach ($_tree as $term_parent) {
      if ($term_parent->tid == $parent) {
        $term_parent->active_trail = TRUE;
        _document_library_calculate_trail_recursive($term_parent, $_tree);
      }
    }
  }
}

/**
 * Build folders at a specific depth (recursively used).
 */
function _document_library_folders_recursive($_field, $_folders, $_parent = 0) {
  $items = array();
  foreach ($_folders as $term) {
    if (in_array($_parent, $term->parents)) {

      // Set class and gather subitems.
      $class     = "closed";
      $sub_items = array();
      if (isset($term->active_trail)) {
        $class     = "open";
        $sub_items = _document_library_folders_recursive($_field, $_folders, $term->tid);
      }

      // Create item.
      $items[] = array(
        'data' => l(
          t('<span class="document-menu-@class"></span>@term', array('@class' => $class, '@term' => $term->name)),
          request_path(),
          array(
            'attributes' => array(
              'class' => array(
                'document-sub',
              ),
            ),
            'html'       => TRUE,
            'query'      => array(
              str_replace('field_', '', $_field) => check_plain($term->name),
            ),
          )
        ),
        'children' => $sub_items,
      );
    }
  }

  return $items;
}

/**
 * Check to make sure the date field exists before trying to join it.
 */
function _document_library_sort_field() {
  $sort_field = variable_get('document_library_default_sort_field', 0);
  if ($sort_field) {
    $result = db_query("SHOW TABLES LIKE :field", array(':field' => '%' . db_like($sort_field)));
    if ($result->rowCount() == 0) {
      $sort_field = 0;
    }
  }

  return $sort_field;
}

/**
 * Custom pre_render function to initialize a pager on the block.
 */
function _document_library_intialize_pager($_element) {
  pager_default_initialize($_element['#variables']['total'], variable_get('document_library_per_page', 10));

  return $_element;
}
