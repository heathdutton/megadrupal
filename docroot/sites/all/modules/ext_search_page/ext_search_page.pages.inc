<?php

/**
 * @file
 * Front pages for Extended search page.
 */


/**
 * Displays a search page.
 *
 * @param $id
 *   The search page's ID.
 * @param $keys
 *   The keys to search for.
 * @param $mode : the render mode.
 */
function ext_search_page_view($id, $keys = NULL, $mode='default') {
  $page = search_api_page_load($id);
  if (!$page) {
    return MENU_NOT_FOUND;
  }
  if (is_string($page->ext_search_options)) {
      $page->ext_search_options=(array)unserialize($page->ext_search_options);
  }
  
  // Pre-process keys (unescape \ and /).
  if (isset($keys) && $keys !== '') {
      $keys = explode("\\\\", $keys);
      $keys = str_replace("\\", "/", $keys);
      $keys = implode("\\", $keys);
  }
  else {
      $keys = NULL;
  }
  
  $ret = array();
  
  $ret['#contextual_links']['search_api_page'] = array(
      'admin/config/search/search_api/page',
      array($page->machine_name),
  );
  


  $values=ext_search_page_get_filter_values($page);
  
    if (!isset($page->options['result_page_search_form']) || $page->options['result_page_search_form']) {
        //$ret['form'] = drupal_get_form('search_api_page_search_form', $page, $keys);
        $ret['form'] = drupal_get_form('ext_search_page_search_form', $page, $keys, $values, $mode);
    }
  
    // Do a search if we have keys, or our empty behavior and facets dictate.
    if ($keys || !empty($page->options['empty_behavior'])) {
        // Override per_page setting with GET parameter.
        $limit = $page->options['per_page'];
        if (!empty($page->options['get_per_page'])
            && !empty($_GET['per_page'])
            && ((int) $_GET['per_page']) > 0) {
                $limit = (int) $_GET['per_page'];
            }
    
            try {
                $results = ext_search_page_search_execute($page, $keys, $values, FALSE, $limit, pager_find_page() * $limit);
            }
            catch (SearchApiException $e) {
                drupal_set_message(t('An error occurred while executing the search. Please try again, or contact the site administrator if the problem persists.'), 'error');
                $link = l(t('search page'), $_GET['q'], array('query' => drupal_get_query_parameters()));
                watchdog_exception('search_api_page', $e,
                '%type while executing a search: !message in %function (line %line of %file).',
                array(), WATCHDOG_ERROR, $link);
                return $ret;
            }
    
            if (!$results) {
                return $ret;
            }
    
            // If spellcheck results were returned then add them to the render array.
            if (isset($results['search_api_spellcheck'])) {
                $ret['results']['#spellcheck'] = array(
                    '#theme' => 'search_api_spellcheck',
                    '#spellcheck' => $results['search_api_spellcheck'],
                    // Let the theme function know where the key is stored by passing its arg
                    // number. We can work this out from the number of args in the page path.
                    '#options' => array(
                        'arg' => array(count(arg(NULL, $page->path))),
                    ),
                    '#prefix' => '<p class="search-api-spellcheck-suggestion">',
                    '#suffix' => '</p>',
                );
            }
    
            $ret['results']['#theme'] = "search_api_page_results__{$page->machine_name}";
            $ret['results']['#index'] = search_api_index_load($page->index_id);
            $ret['results']['#results'] = $results;
            $ret['results']['#view_mode'] = isset($page->options['view_mode']) ? $page->options['view_mode'] : 'search_api_page_result';
            $ret['results']['#keys'] = $keys;
            $ret['results']['#page'] = $page;
    
            // Load pager.
            if ($results['result count'] > $limit) {
                pager_default_initialize($results['result count'], $limit);
                $ret['results']['#pager']['#theme'] = 'pager';
            }
    
            if (!empty($results['ignored'])) {
                drupal_set_message(t('The following search keys are too short or too common and were therefore ignored: "@list".', array('@list' => implode(t('", "'), $results['ignored']))), 'warning');
            }
            if (!empty($results['warnings'])) {
                foreach ($results['warnings'] as $warning) {
                    drupal_set_message(check_plain($warning), 'warning');
                }
            }
    }
  
  return $ret;
}

/**
 * Add ext search filters widgets to the form.
 * 
 * @param array $form
 * @param Entity $page
 * @param string $mode
 *   The render mode
 * @ingroup forms
 */
function ext_search_page_filter_form(&$form, $page, $values, $mode='default') {
  
  drupal_add_css(drupal_get_path('module', 'ext_search_page') . '/ext_search_page.css');
  
  if (!$page->ext_search_options['enabled']) return;
  
  $filters=array();
  foreach (ext_search_page_get_filter_widgets($page) as $field => $widget) {
    if (_ext_search_page_page_mode($mode)&&!$widget['display']) continue;
    if ($mode=='block'&&!$widget['block']) continue;
    $f=$widget['add_form'];
    $f($filters, $field, $widget, isset($values[$field])?$values[$field]:NULL, $mode);
  } 
  
  if (count($filters)) {    
    $form['filters'] = $filters+array(
      '#type' => 'fieldset',
      '#title' => t('Advanced search'),
      '#theme' => 'exposed_filters__ext_search_page',
      '#collapsible' => FALSE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
      );
    
    $form['filters']['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array('class' => array('container-inline')),
    );
    $form['filters']['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Filter'),
    );
    if (count($values)) {
      $form['filters']['actions']['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Reset')
      );
    }
  
    drupal_add_js('misc/form.js');
  }
}


/**
 * Display a search form.
 *
 * @param Entity $page
 *   The search page for which this form is displayed.
 * @param $keys
 *   The search keys.
 * @param $mode
 *   Render mode (e.g. for blocks) instead of a normal one.
 * @ingroup forms
 */
function ext_search_page_search_form(array $form, array &$form_state, $page, $keys = NULL, $values=array(), $mode = 'default') {

  if (!isset($page->ext_search_options['hide_fulltext'])||!$page->ext_search_options['hide_fulltext']) {
    $fulltext=array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('edit-fulltext')),
    );
    
    $fulltext['keys'] = array(
      '#type' => 'textfield',
      '#default_value' => $keys,
    );
    
    $fulltext['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
    );
    
    if (_ext_search_page_page_mode($mode)) {
      $fulltext['keys']['#title']=t('Enter your keywords');
      $form['fulltext']=$fulltext;
    }
    else {
      $fulltext['keys']['#size'] = 15;
      $form+=$fulltext;
    }
  }
  
  if (_ext_search_page_page_mode($mode)) {
    $form = array(
      '#type' => 'fieldset',
      '#title' => check_plain($page->name),
      'form' => $form,
    );
    if ($page->description) {
      $form['text']['#markup'] = $page->description;
      $form['text']['#weight'] = -5;
    }
  }

  $form['#attributes']['class'][]='ext-search-page-search-form';
  $form['#attributes']['class'][]='search-mode-' . $mode;
  
  ext_search_page_filter_form($form, $page, $values, $mode);
  
  return $form;
}

/**
 * Submit callback for search_api_page_search_form().
 * 
 * @see ext_search_page_search_form()
 */
function ext_search_page_search_form_submit(array $form, array &$form_state) {
  $page=&$form_state['build_info']['args'][0];
  $filter_values = array();
  switch ($form_state['values']['op']) {
    case t('Reset'):
      // nothing ...
      break;
    default:
      if (!isset($form_state['values']['filters'])) break;
      $filter_values = ext_search_page_build_filter_values($page, $form_state['values']['filters']);
      break;
  }
  
  // @todo Take care of "/"s in the keys
  $keys = trim($form_state['values']['keys']);

  // to be nice with other modules callbacks
  $form_state['storage']['keys']=$keys;
  $form_state['storage']['filter_values']=$filter_values;
  
  $form_state['redirect'] = array(
    $page->path . '/' . urlencode($keys),
    array('query' => $filter_values));
}

/**
 * Helper function for taxonomy autocompletion
 * It's a quick rewrite of taxonomy_autocomplete()
 */
function ext_search_page_taxonomy_autocomplete($vocabulary, $tags_typed = '') {

  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary);
  // The user enters a comma-separated list of tags. We only autocomplete the last tag.
  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));

  $matches = array();
  if ($tag_last != '') {

    $query = db_select('taxonomy_term_data', 't');
    $query->addTag('translatable');
    $query->addTag('term_access');

    // Do not select already entered terms.
    if (!empty($tags_typed)) {
      $query->condition('t.name', $tags_typed, 'NOT IN');
    }
    // Select rows that match by term name.
    $tags_return = $query
      ->fields('t', array('tid', 'name'))
      ->condition('t.vid', $vocabulary->vid)
      ->condition('t.name', '%' . db_like($tag_last) . '%', 'LIKE')
      ->range(0, 10)
      ->execute()
      ->fetchAllKeyed();

    $prefix = count($tags_typed) ? implode(', ', $tags_typed) . ', ' : '';

    $term_matches = array();
    foreach ($tags_return as $tid => $name) {
      $n = $name;
      // Term names containing commas or quotes must be wrapped in quotes.
      if (strpos($name, ',') !== FALSE || strpos($name, '"') !== FALSE) {
        $n = '"' . str_replace('"', '""', $name) . '"';
      }
      else {
        $term_matches[$prefix . $n] = check_plain($name);
      }
    }
  }

  drupal_json_output($term_matches);
}
