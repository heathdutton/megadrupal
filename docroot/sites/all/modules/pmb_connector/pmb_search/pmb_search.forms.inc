<?php

/**
 * @file
 * PMB search forms.
 */

/**
 * Helper for base search form.
 */
function pmb_search_get_fields() {
  return array(
    'all_fields' => t('All fields'),
    'title' => t('Titles'),
    'author' => t('Authors'),
    'publisher' => t('Publishers'),
    'serie' => t('Series'),
    'subserie' => t('Sub-series'),
    // BUG in PMB webservice: Query to PMB with this field removes all results.
//     'category' => t('Categories'),
    'indexint' => t('Decimal indexing'),
  );
}

/**
 * Base search form.
 *
 * @ingroup forms
 * @see pmb_search_base_form_validate()
 * @see pmb_search_base_form_submit()
 */
function pmb_search_base_form($form, &$form_state) {
  $form['search_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Search terms'),
    '#size' => 50,
    '#maxlength' => 255,
    '#description' => t('The search terms'),
  );

  $search_fields = pmb_search_get_fields();
  $form['search_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Fields'),
    '#description' => t('Fields to search'),
    '#options' => $search_fields,
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['Submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * @see pmb_search_base_form()
 */
function pmb_search_base_form_validate($form, &$form_state) {
  $form_state['values']['search_query'] = preg_replace('/\//', '', $form_state['values']['search_query']);
  if (!$form_state['values']['search_query']) {
    form_set_error('search_query', t('Please enter a search query.'));
  }
  $somewhere_to_search = FALSE;
  foreach ($form_state['values']['search_fields'] as $asearch_field) {
    if ($asearch_field) {
      $somewhere_to_search = TRUE;
      break;
    }
  }
  if (!$somewhere_to_search) {
    form_set_error('search_fields', t('Please enter at least one field to search in.'));
  }
}

/**
 * @see pmb_search_base_form()
 */
function pmb_search_base_form_submit($form, &$form_state) {
  $search_term = $form_state['values']['search_query'];
  $search_fields = array();
  foreach ($form_state['values']['search_fields'] as $asearch_field) {
    if ($asearch_field) {
      $search_fields[] = $asearch_field;
    }
  }
  // $search_fields = implode(',', $search_fields);
  $form_state['redirect'] = _pmb_p('catalog/search/local/') . str_replace('/', ',', _pmb_p(implode('/', $search_fields))) . '/' . $search_term;
}

/**
 * Block search form.
 *
 * @ingroup forms
 * @see pmb_search_block_form_validate()
 * @see pmb_search_block_form_submit()
 */
function pmb_search_block_form($form, &$form_state) {
  $form['search_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Quick search terms'),
    '#size' => 15,
    '#maxlength' => 255,
    '#description' => t('Please enter one or more search terms.'),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['Submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * @see pmb_search_block_form()
 */
function pmb_search_block_form_validate($form, &$form_state) {
  $form_state['values']['search_query'] = preg_replace('/\//', '', $form_state['values']['search_query']);
  if (!$form_state['values']['search_query']) {
    form_set_error('search_query', t('Please enter a search query.'));
  }
}

/**
 * @see pmb_search_block_form()
 */
function pmb_search_block_form_submit($form, &$form_state) {
  $search_term = $form_state['values']['search_query'];
  $search_fields = array('all_fields');
  // $search_fields = implode(',', $search_fields);
  $form_state['redirect'] = _pmb_p('catalog/search/local/') . str_replace('/', ',', _pmb_p(implode('/', $search_fields))) . '/' . $search_term;
}

/**
 * Helper for external search form.
 */
function pmb_search_get_external_fields() {
  return array(
    'all_fields' => t('All fields'),
    'title' => t('Titles'),
    'author' => t('Authors'),
    'publisher' => t('Publishers'),
    'serie' => t('Series'),
    'subserie' => t('Sub-series'),
    'isbn_bar_code' => t('ISBN/Bar code'),
  );
}

/**
 * External search form.
 *
 * @ingroup forms
 * @see pmb_search_external_form_validate()
 * @see pmb_search_external_form_submit()
 */
function pmb_search_external_form($form, &$form_state, $sources) {
  $form['search_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Search terms'),
    '#size' => 50,
    '#maxlength' => 255,
    '#description' => t('The search terms'),
  );

  $search_fields = pmb_search_get_external_fields();
  $form['search_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Fields'),
    '#description' => t('Fields to search'),
    '#options' => $search_fields,
  );

  $form['checkboxes'] = array(
    '#type' => 'checkboxes',
    '#options' => array(),
  );

  $form['sources'] = array();
  if ($sources) {
    foreach ($sources as $asource_category_index => $asource_category) {
      $form['sources']['c' . $asource_category_index] = array(
        '#value' => check_plain($asource_category->category_caption),
      );
      foreach ($asource_category->sources as $asource) {
        $form['sources'][$asource->source_id] = array(
          '#value' => check_plain($asource->source_caption),
          '#comment' => check_plain($asource->source_comment),
        );
        $form['checkboxes']['#options'][$asource->source_id] = $asource->source_caption;
      }
    }
  }

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['Submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * @see pmb_search_external_form()
 */
function pmb_search_external_form_validate($form, &$form_state) {
  $form_state['values']['search_query'] = preg_replace('/\//', '', $form_state['values']['search_query']);
  if (!$form_state['values']['search_query']) {
    form_set_error('search_query', t('Please enter a search query.'));
  }
  $somewhere_to_search = FALSE;
  foreach ($form_state['values']['checkboxes'] as $asource) {
    if ($asource) {
      $somewhere_to_search = TRUE;
      break;
    }
  }
  if (!$somewhere_to_search) {
    form_set_error('search_fields', t('Please enter at least one source to search in.'));
  }

  $somewhere_to_search = FALSE;
  foreach ($form_state['values']['search_fields'] as $asearch_field) {
    if ($asearch_field) {
      $somewhere_to_search = TRUE;
      break;
    }
  }
  if (!$somewhere_to_search) {
    form_set_error('search_fields', t('Please enter at least one field to search in.'));
  }
}

/**
 * @see pmb_search_external_form()
 */
function pmb_search_external_form_submit($form, &$form_state) {
  //drupal encode automatiquement ses urls...

  //$search_term = rawurlencode($form_state['values']['search_query']);
  $search_term = $form_state['values']['search_query'];
  $search_fields = array();
  foreach ($form_state['values']['search_fields'] as $asearch_field) {
    if ($asearch_field) {
      $search_fields[] = $asearch_field;
    }
  }
  foreach ($form_state['values']['checkboxes'] as $asource_id) {
    if ($asource_id) {
      $search_sources[] = $asource_id;
    }
  }
//  $search_fields = rawurlencode(implode(',', $search_fields));
  // $search_fields = implode(',', $search_fields);
//  $search_sources = rawurlencode(implode(',', $search_sources));
  $search_sources = implode(',', $search_sources);
  $form_state['redirect'] = _pmb_p('catalog/search/external/') . str_replace('/', ',', _pmb_p(implode('/', $search_fields))) . '/' . $search_sources . '/' . $search_term;
}

/**
 * Helper for advanced search form.
 */
function pmb_search_get_advanced_fields() {
  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $results = $pmb_data->get_search_advanced_fields(TRUE);

  $fields = array();
  foreach ($results as $result) {
    $fields[$result->id]= $result->label;
  }
  return $fields;
}

/**
 * Helper for advanced search form.
 */
function pmb_search_get_advanced_current_fields_form(&$form, $current_fields) {
  require_once(drupal_get_path('module', 'pmb_search') . '/pmb_search.ajax.inc');

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $fields = $pmb_data->get_search_advanced_fields(FALSE);

  $header = $row = array();
  $element = array();
  foreach ($fields as $field) {
    if (in_array($field->id, $current_fields)) {
      //Label
      $opts = array();
      $form['fields']['field_' . $field->id]['label'] = array(
        '#type' => 'item',
        '#value' => $field->label
      );
      //Opérateurs
      foreach ($field->operators as $operator) {
        $opts[$operator->id] = $operator->label;
      }
      $form['fields']['field_' . $field->id]['op_' . $field->id] =array(
        '#type' => 'select',
        '#title' => t('Operators'),
        '#options' => $opts,
      );
      //Valeur
      $values = array();
      foreach ($field->values as $val) {
        $values[$val->value_id] = $val->value_caption;
      }
      switch ($field->type) {
        case 'authoritie' :
          $form['fields']['field_' . $field->id]['value_' . $field->id] =array(
            '#type' => 'textfield',
            '#title' => t('Value'),
          );
          break;
        case 'list' :
        case 'query_list' :
        case 'marc_list' :
          $form['fields']['field_' . $field->id]['value_' . $field->id] =array(
            '#type' => 'select',
            '#title' => t('Value'),
            '#options' => $values,
          );
          break;
        case 'date' :
          $form['fields']['field_' . $field->id]['value_' . $field->id] =array(
            '#type' => 'date',
            '#title' => t('Value'),
          );
          break;
        case 'small_text' :
          if (count($values)) {
            $form['fields']['field_' . $field->id]['value_' . $field->id] =array(
              '#type' => 'select',
              '#title' => t('Value'),
              '#options' => $values,
            );
          }
          else {
            $form['fields']['field_' . $field->id]['value_' . $field->id] =array(
              '#type' => 'textfield',
              '#title' => t('Value'),
            );
          }
          break;
        case 'text' :
        default :
          $form['fields']['field_' . $field->id]['value_' . $field->id] =array(
            '#type' => 'textfield',
            '#title' => t('Value'),
          );
          break;
      }
//      //la suppression d'un critère
//      $form['fields']['field_' . $field->id]['del_' . $field->id] = array(
//        '#type' => 'checkbox',
//        '#title' => t("Delete crit"),
//        '#value' => t("Delete Crit"),
//        '#ajax' => array(
//          'event' => 'click',
//          'method' => 'replace',
//          'wrapper' => 'fields_content',
//          'callback' => 'pmb_search_ajax_get_field',
//        )
//      );
    }
  }
}

/**
 * Advanced search form.
 *
 * @ingroup forms
 * @see pmb_search_advanced_form_validate()
 * @see pmb_search_advanced_form_submit()
 */
function pmb_search_advanced_form($form, &$form_state) {
  require_once(drupal_get_path('module', 'pmb_search') . '/pmb_search.ajax.inc');

  // On doit déjà récupérer la liste des critères...
  $fields = pmb_search_get_advanced_fields();

  $form['#action'] = url(_pmb_p('catalog/search_advanced/result'));

  $form['advancedSearchCrit'] = array(
    '#type' => 'select',
    '#title' => t('Criterias'),
    '#options' => $fields,
    '#ajax' => array(
      'event' => 'change',
      'method' => 'replace',
      'wrapper' => 'fields_content',
      'callback' => 'pmb_search_ajax_get_field',
    )
  );
  $form['current_fields'] = array(
    '#type' => 'hidden',
    '#value' => $form_state['values']['current_fields'],
  );
  $form['fields'] = array(
    '#type' => 'item',
    '#prefix' => '<div id="fields_content">',
    '#suffix' => '</div>',
  );
  $current_fields = unserialize($form_state['values']['current_fields']);
  if (is_array($current_fields)) {
    pmb_search_get_advanced_current_fields_form($form, $current_fields);
  }
  $form['actions'] = array('#type' => 'actions');

  $form['actions']['Submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * @see pmb_search_advanced_form()
 */
function pmb_search_advanced_form_validate($form, &$form_state) {
}

/**
 * @see pmb_search_advanced_form()
 */
function pmb_search_advanced_form_submit($form, &$form_state) {
  $current_fields = unserialize($form_state['values']['current_fields']);
  for ($i = 0; $i < count($current_fields); $i++) {
    if (isset($form_state['values']['del_' . $current_fields[$i]]) && $form_state['values']['del_' . $current_fields[$i]] == "1") {
      array_splice($current_fields, $i, 1);
      $i--;
    }
  }
  $current_fields[] = $form_state['values']['advancedSearchCrit'];
  $form_state['values']['current_fields'] = serialize($current_fields);
  return 'plop';
}
