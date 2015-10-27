<?php
/**
 * @file
 * Define Verticrawl search page in front office.
 */

/**
 * Callback function from hook_menu().
 *
 * @see verticrawl_search_menu()
 */
function verticrawl_search_results_form($form, &$form_state) {
  global $language;

  // Adding items to Drupal forms.
  _verticrawl_search_results_form_set_form_items($form);
  $form['#submit'][] = 'verticrawl_search_results_form_submit';

  // Prevent double api request caused by POST then REDIRECT.
  if (!isset($_POST['form_id'])) {
    // If keywords empty or doesn't exist, don't display results.
    if (isset($_GET['keywords']) && empty($_GET['keywords'])) {
      $form['keywords']['#default_value'] = '';
      $form['verticrawl_search']['urls'] = array(
        '#markup' => "",
      );
      drupal_set_message(t('Please enter some keywords.'), 'error');
    }
    // If keywords are defined.
    else {
      $api_key = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_API_CONFIG_ID);
      $api = new VerticrawlSearchApi($api_key);
      $api->setHeaderLanguage($language->language);
      $get_keys = array();

      $verticrawl_parameters = array(
        'searchresult' => 'search_results',
        'URL' => 'urls',
        'RELATED_TERMS' => 'related_terms',
        'ANSWORD' => 'answords',
        'suggest_ortho' => 'spell_suggestion',
      );
      $api->setParameters($verticrawl_parameters);

      $keyword_wrapper = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_KEYWORD_WRAPPER_CONFIG_ID);
      $api->setKeywordWrapper($keyword_wrapper);

      if (isset($_GET['page'])) {
        $api->setPageNum(intval($_GET['page']));
      }

      if (isset($_GET['keywords'])) {
        $form['keywords']['#default_value'] = urldecode($_GET['keywords']);
        $keywords = explode(" ", trim($_GET['keywords']));

        $api->setKeywords($keywords);
      }

      // Managing extra Keys.
      _verticrawl_search_results_form_managing_extra_keys($form, $api, $get_keys);

      $results = $api->fetchResults();
      $search_page_path = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_PAGE_PATH_CONFIG_ID, VERTICRAWL_SEARCH_DEFAULT_PATH);

      // If we have some results to display.
      if (is_array($results['urls']) && !empty($results['urls']) && isset($_GET['keywords'])) {
        // Adding results summary.
        $results_summary_output = t('Showing @first - @last of @total results', array(
          '@first' => $results['search_results']['first'],
          '@last' => $results['search_results']['last'],
          '@total' => $results['search_results']['total'],
        ));

        $form['verticrawl_search']['results_summary'] = array(
          '#markup' => '<div class="results-summary">' . $results_summary_output . '</div>',
        );

        // Adding adwords.
        if (!empty($results['answords']) && !isset($results['answords']['NOANSWER'])) {
          $form['verticrawl_search']['answords'] = array(
            '#markup' => theme(
              'verticrawl_search__answords',
              array(
                'answords' => $results['answords'],
              )
            ),
          );
        }

        // Adding results.
        $groupbysite = FALSE;
        if (isset($_GET['group_by_site']) && $_GET['group_by_site'] == TRUE) {
          $groupbysite = TRUE;
        }

        global $base_url;
        $current_url = $base_url . request_uri();

        $form['verticrawl_search']['urls'] = array(
          '#markup' => theme(
            'verticrawl_search__results',
            array(
              'urls' => $results['urls'],
              'group_by_site' => $groupbysite,
              'current_url' => $current_url,
            )
          ),
        );

        // Adding pagination.
        $pager = $api->getPager($get_keys);

        if (!empty($pager)) {
          $form['verticrawl_search']['pager'] = array(
            '#markup' => theme(
              'verticrawl_search__pager',
              array(
                'pages' => $pager,
                'base_url' => $search_page_path,
              )
            ),
          );
        }
      }
      // If no result to display.
      elseif (isset($_GET['keywords'])) {
        $no_result_output = '<div class="no-results">';
        $no_result_output .= t('<p>No documents correspond to the specified search words (<strong>@keywords</strong>).</p>', array('@keywords' => urldecode($_GET['keywords'])));

        // Adding Spell suggestion.
        if (isset($results['spell_suggestion']) && !empty($results['spell_suggestion']) && is_array($results['spell_suggestion'])) {
          $no_result_output .= '<div class="spell-suggestion"><p>' . t('Did you mean:') . '</p><ul>';

          foreach ($results['spell_suggestion'] as $spell_suggestion => $spell_suggestion_exists) {
            $no_result_output .= '<li><a href="' . $search_page_path . '?keywords=' . $spell_suggestion . '">' . $spell_suggestion . '</a></li>';
          }

          $no_result_output .= '</ul></div>';
        }

        $no_result_output .= t('<p>Suggestions :</p>
                                <ul>
                                  <li>Check the spelling of search terms.</li>
                                  <li>Try other words.</li>
                                  <li>Try more general keywords.</li>
                                </ul>');

        $no_result_output  .= '</div>';

        $form['verticrawl_search']['urls'] = array(
          '#markup' => $no_result_output,
        );
      }

      // Adding related terms.
      if (isset($results['related_terms']) && is_array($results['related_terms']) && !empty($results['related_terms'])) {
        $related_terms_output = '<div class="verticrawl-search-related-terms"><h3 class="related-terms-title">' . t('Related terms:') . '</h3><ul class="related-terms-list">';

        foreach ($results['related_terms'] as $related_term) {
          $related_terms_output .= '<li class="related-terms-item"><a href="' . $search_page_path . '?keywords=' . $related_term . '">' . $related_term . '</a></li>';
        }

        $related_terms_output .= '</ul></div>';

        $form['verticrawl_search']['related_terms'] = array(
          '#markup' => $related_terms_output,
        );
      }
    }
  }

  $display_powered_by = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_POWERED_BY, 1);
  if ($display_powered_by) {
    $form['verticrawl_search']['powered_by'] = array(
      '#markup' => '<div class="verticrawl-powered-by">' . t('Powered by <a href="http://www.verticrawl.com" target="_blank">VERTICRAWL</a>') . '</div>',
    );
  }

  return $form;
}

/**
 * Add items to search_page drupal form.
 *
 * @see verticrawl_search_results_form()
 */
function _verticrawl_search_results_form_set_form_items(&$form) {
  $description = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_PAGE_DESCRIPTION_CONFIG_ID);
  $description_format = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_PAGE_DESCRIPTION_FORMAT_CONFIG_ID);

  if (!empty($description_format)) {
    $description = check_markup($description, $description_format);
  }

  $form['description'] = array(
    '#markup' => '<div class="page-description">' . t($description) .  '</div>',
  );

  // Keywords - text.
  $form['keywords'] = array(
    '#type' => 'textfield',
    '#title' => t("Enter keywords"),
  );

  // Items per page - select.
  $form['items_per_page'] = array(
    '#key_type' => 'associative',
    '#multiple_toggle' => '0',
    '#type' => 'select',
    '#options' => array(
      5 => 5,
      10 => 10,
      25 => 25,
      50 => 50,
      100 => 100,
    ),
    '#title' => t("Results per page"),
    '#default_value' => 10,
  );

  // Submit.
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  if (user_access('use verticrawl advanced_search')) {
    // Advanced search - fieldset.
    $form['advanced_search'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced search'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Sort by - select.
    $form['advanced_search']['sort_by'] = array(
      '#key_type' => 'associative',
      '#multiple_toggle' => '0',
      '#type' => 'select',
      '#options' => array(
        'r' => t('Relevance'),
        'd' => t('Date ascending'),
        'D' => t('Date descending'),
      ),
      '#title' => t("Sort by"),
      '#default_value' => 'r',
    );

    // Match mode - select.
    $form['advanced_search']['match_mode'] = array(
      '#key_type' => 'associative',
      '#multiple_toggle' => '0',
      '#type' => 'select',
      '#options' => array(
        'wrd' => t('Exact word'),
        'sub' => t('Word contains'),
        'beg' => t('Word begin with'),
        'end' => t('Word end with'),
      ),
      '#title' => t("Match mode"),
      '#default_value' => 'wrd',
    );

    // Document type - select.
    $document_types = array(
      '' => t('-- All --'),
      'document/html' => 'HTML',
      'text/rtf' => 'RTF',
      'application/pdf' => 'PDF',
      'application/msword' => 'Microsoft Word',
      'application/vnd.ms-excel' => 'Microsoft Excel',
      'application/vnd.ms-powerpoint' => 'Microsoft Powerpoint',
    );

    $form['advanced_search']['document_type'] = array(
      '#key_type' => 'associative',
      '#multiple_toggle' => '0',
      '#type' => 'select',
      '#options' => $document_types,
      '#title' => t("Document type"),
    );

    // Site URL - textfield.
    $form['advanced_search']['site_url'] = array(
      '#title' => t('Site URL'),
      '#type' => 'textfield',
      '#description' => t('E.g, http://www.example.com'),
    );

    // Group by site - checkbox.
    $form['advanced_search']['group_by_site'] = array(
      '#type' => 'checkbox',
      '#title' => t("Group by site"),
      '#return_value' => 1,
      '#default_value' => 0,
    );

    // Filter by date - checkbox.
    $form['advanced_search']['filter_by_date'] = array(
      '#type' => 'checkbox',
      '#title' => t("Filter by date"),
      '#return_value' => 1,
      '#default_value' => 0,
    );

    // Advanced search date - fieldset.
    $form['advanced_search']['date_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Date filter'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#states' => array(
        'visible' => array(
          ':input[name="filter_by_date"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Date filter mode - select.
    $form['advanced_search']['date_fieldset']['date_filter_mode'] = array(
      '#key_type' => 'associative',
      '#multiple_toggle' => '0',
      '#type' => 'select',
      '#options' => array(
        '-1' => t('older than'),
        '1' => t('newer than'),
      ),
      '#title' => t("Date filter mode"),
    );

    // Date select - date popup.
    $form['advanced_search']['date_fieldset']['date_select'] = array(
      '#type' => 'date_popup',
      '#title' => t('Select a date'),
      '#date_format' => 'Y-m-d',
    );

    $advanced_search_items = array(
      'sort_by' => VERTICRAWL_SEARCH_ADVANCED_SORT_BY_CONFIG_ID,
      'match_mode' => VERTICRAWL_SEARCH_ADVANCED_MATCH_MODE_CONFIG_ID,
      'document_type' => VERTICRAWL_SEARCH_ADVANCED_DOCUMENT_TYPE_CONFIG_ID,
      'site_url' => VERTICRAWL_SEARCH_ADVANCED_SITE_URL_CONFIG_ID,
      'group_by_site' => VERTICRAWL_SEARCH_ADVANCED_GROUP_BY_SITE_CONFIG_ID,
      'filter_by_date' => VERTICRAWL_SEARCH_ADVANCED_FILTER_BY_DATE_CONFIG_ID,
      'date_fieldset' => VERTICRAWL_SEARCH_ADVANCED_FILTER_BY_DATE_CONFIG_ID,
    );

    foreach ($advanced_search_items as $field => $config) {
      $form['advanced_search'][$field]['#access'] = VerticrawlHelper::getConfigValue($config, 1);
    }
  }
}

/**
 * Managing extra key (or filters) in api instance from search.
 *
 * @see verticrawl_search_results_form()
 */
function _verticrawl_search_results_form_managing_extra_keys(&$form, &$api, &$get_keys) {
  $extra_keys = array();

  // Extra key - item per page.
  if (isset($_GET['items_per_page'])) {
    $extra_keys['items_per_page'] = array(
      'api_method' => 'setItemsPerPage',
      'api_method_args' => intval($_GET['items_per_page']),
      'form_keys' => "['items_per_page']",
    );
  }

  if (user_access('use verticrawl advanced_search')) {
    // Extra key - Match mode.
    if (isset($_GET['match_mode'])) {
      $extra_keys['match_mode'] = array(
        'api_method' => 'setMatchMode',
        'api_method_args' => $_GET['match_mode'],
        'form_keys' => "['advanced_search']['match_mode']",
      );
    }

    // Extra key - Sort by.
    if (isset($_GET['sort_by'])) {
      $extra_keys['sort_by'] = array(
        'api_method' => 'setSortBy',
        'api_method_args' => $_GET['sort_by'],
        'form_keys' => "['advanced_search']['sort_by']",
      );
    }

    // Managing Filters from API.
    $filters = array();

    // Extra key - Site URL.
    if (isset($_GET['site_url'])) {
      $filters['ul'] = $_GET['site_url'];
      $extra_keys['site_url'] = array(
        'form_keys' => "['advanced_search']['site_url']",
      );
    }

    // Extra key - group by site.
    if (isset($_GET['group_by_site']) && !empty($_GET['group_by_site'])) {
      $filters['groupbysite'] = $_GET['group_by_site'] == 1 ? 'yes' : 'no';

      $extra_keys['group_by_site'] = array(
        'form_keys' => "['advanced_search']['group_by_site']",
      );
    }

    // Extra key - site.
    if (isset($_GET['site']) && !empty($_GET['site'])) {
      $filters['site'] = $_GET['site'];
      $extra_keys['site'] = array();

      // Remove groupbysite in API Request.
      unset($filters['groupbysite']);
      unset($extra_keys['group_by_site']);
    }

    // Extra key - Document type.
    if (isset($_GET['document_type']) && !empty($_GET['document_type'])) {
      $filters['type'] = $_GET['document_type'];
      $extra_keys['document_type'] = array(
        'form_keys' => "['advanced_search']['document_type']",
      );
    }

    $api->setFilters($filters);

    // Extra key - date.
    if (isset($_GET['date_select']) && isset($_GET['date_filter_mode'])) {
      $date_time = strtotime($_GET['date_select']);
      $date_method_args = array(
        'dx' => $_GET['date_filter_mode'],
        'dd' => date('d', $date_time),
        'dm' => date('m', $date_time),
        'dy' => date('Y', $date_time),
      );

      $extra_keys['filter_by_date'] = array(
        'form_keys' => "['advanced_search']['filter_by_date']",
      );
      $extra_keys['date_filter_mode'] = array(
        'form_keys' => "['advanced_search']['date_fieldset']['date_filter_mode']",
      );
      $extra_keys['date_select'] = array(
        'api_method' => 'setFilterDate',
        'api_method_args' => $date_method_args,
        'form_keys' => "['advanced_search']['date_fieldset']['date_select']",
      );
    }
  }

  $is_advanced_search = FALSE;

  // Managing all extra keys in this loop.
  foreach ($extra_keys as $key => $value) {
    if (isset($_GET[$key])) {
      if (isset($value['api_method']) && isset($value['api_method_args'])) {
        $method = $value['api_method'];
        $api->$method($value['api_method_args']);
      }

      if (isset($value['form_keys'])) {
        $expression = '$form' . $value['form_keys'] . '[\'#default_value\'] = $_GET[$key];';
        eval($expression);

        if (!$is_advanced_search && strpos($value['form_keys'], 'advanced_search')) {
          $form['advanced_search']['#collapsed'] = FALSE;
        }
      }

      $get_keys[$key] = urldecode($_GET[$key]);
    }
  }
}

/**
 * Callback function after submitting drupal form.
 *
 * @see verticrawl_search_results_form()
 */
function verticrawl_search_results_form_submit($form, &$form_state) {
  // Adding Keywords.
  $query['query']['keywords'] = trim($form_state['values']['keywords']);

  // Adding items per page.
  if (isset($form_state['values']['items_per_page']) && $form_state['values']['items_per_page'] != 10) {
    $query['query']['items_per_page'] = intval($form_state['values']['items_per_page']);
  }

  // Managing advanced search.
  // SORT BY.
  if (isset($form_state['values']['sort_by']) && $form_state['values']['sort_by'] != 'r') {
    $query['query']['sort_by'] = $form_state['values']['sort_by'];
  }

  // MATCH MODE.
  if (isset($form_state['values']['match_mode']) && $form_state['values']['match_mode'] != 'wrd') {
    $query['query']['match_mode'] = $form_state['values']['match_mode'];
  }

  // DOCUMENT TYPE.
  if (isset($form_state['values']['document_type']) && !empty($form_state['values']['document_type'])) {
    $query['query']['document_type'] = $form_state['values']['document_type'];
  }

  // SITE URL.
  if (isset($form_state['values']['site_url']) && !empty($form_state['values']['site_url'])) {
    $query['query']['site_url'] = $form_state['values']['site_url'];
  }

  // GROUP BY.
  if (isset($form_state['values']['group_by_site']) && $form_state['values']['group_by_site'] == 1) {
    $query['query']['group_by_site'] = $form_state['values']['group_by_site'];
  }

  // FILTER BY DATE.
  if (isset($form_state['values']['filter_by_date']) && !empty($form_state['values']['filter_by_date'])) {
    // DATE FILTER.
    if (isset($form_state['values']['date_select']) && !empty($form_state['values']['date_select'])) {
      $query['query']['filter_by_date'] = $form_state['values']['filter_by_date'];
      $query['query']['date_select'] = $form_state['values']['date_select'];
      $query['query']['date_filter_mode'] = $form_state['values']['date_filter_mode'];
    }
  }

  $search_page_path = VerticrawlHelper::getConfigValue(VERTICRAWL_SEARCH_PAGE_PATH_CONFIG_ID, VERTICRAWL_SEARCH_DEFAULT_PATH);

  $form_state['redirect'] = array(
    $search_page_path,
    $query,
  );
}
