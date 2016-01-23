<?php

/**
 * @file
 * Communication with REST API.
 */

/**
 * Page callback for search form.
 */
function dsb_portal_rest_api_search() {
  if (!dsb_portal_check_module_usable(TRUE)) {
    return '';
  }

  $client = dsb_portal_get_client_based_on_config();

  try {
    $client->authenticate();
  }
  catch (Exception $e) {
    drupal_set_message(t("Something went wrong. Error message: @message", array(
      '@message' => $e->getMessage(),
    )), 'error');
    watchdog('dsb_portal', "Authentication failed. Error message: @message", array(
      '@message' => $e->getMessage(),
    ), WATCHDOG_ERROR);
    return '';
  }

  // Get the search query, if any.
  list($query, $filters, $offset) = array_values(
    dsb_portal_rest_api_get_search_from_globals()
  );

  // Prepare the facets. This function also allows other modules to alter them.
  $facets = dsb_portal_rest_api_get_facets();

  // Prepare the desired fields. This function also allows other modules to
  // alter them.
  $fields = dsb_portal_rest_api_get_fields();

  // Allow modules to alter the filters and query.
  drupal_alter('dsb_portal_rest_api_query', $query);
  drupal_alter('dsb_portal_rest_api_filters', $filters);

  // Get the limit setting.
  $limit = variable_get('dsb_portal_search_limit', 20);

  // Get the sort setting.
  $sort_by = variable_get('dsb_portal_search_sort_by', 'relevance');

  // Perform the API call.
  try {
    dsb_portal_debug("About to perform a search request. Parameters:<br /><pre>!params</pre>", array(
      '!params' => print_r(array(
        'query' => $query,
        'facets' => $facets,
        'filters' => $filters,
        'fields' => $fields,
        'offset' => $offset,
        'limit' => $limit,
        'sort_by' => $sort_by
      ), 1),
    ));

    $result = $client->search(
      $query,
      $facets,
      $filters,
      $fields,
      $offset,
      $limit,
      $sort_by
    );

    dsb_portal_debug("Performed a search request. Result:<br /><pre>!result</pre>", array(
      '!result' => print_r($result, 1),
    ));
  }
  catch (Exception $e) {
    drupal_set_message(t("Something went wrong. Error message: @message", array(
      '@message' => $e->getMessage(),
    )), 'error');

    watchdog('dsb_portal', "Search failed. Error message: @message", array(
      '@message' => $e->getMessage(),
    ), WATCHDOG_ERROR);
    return '';
  }

  // Re-order the facets from the search result, so we make sure we respect
  // the order provided by the module (and other modules).
  $reordered_facets = array();
  foreach ($facets as $facet_name) {
    if (isset($result['facets'][$facet_name])) {
      $reordered_facets[$facet_name] = $result['facets'][$facet_name];
    }
  }
  $result['facets'] = $reordered_facets;

  // Store the results statically for this request. If the search form is
  // exposed in a block, the form there will need this result information to
  // render the facets. Other modules might want this information as well.
  dsb_portal_set_static_search_result($result);

  // Theme the results. Each result object is converted to a
  // \Educa\DSB\Client\Lom\LomDescriptionSearchResult before being passed to
  // the templates.
  $themed_results = theme('dsb_portal_search_results', array(
    'num_found' => $result['numFound'],
    'results' => array_map(function($item) {
      return new Educa\DSB\Client\Lom\LomDescriptionSearchResult($item);
    }, $result['result']),
  ));

  // Must the search form be shown on the same page, or is it exposed in a
  // block?
  if (!variable_get('dsb_portal_search_form_expose_form_in_block', 0)) {
    // Get the search form.
    $form = drupal_get_form('dsb_portal_rest_api_search_form', $result['facets']);

    // Render the form.
    $themed_form = render($form);
  }
  else {
    $themed_form = '';
  }

  // Prepare the pager, if necessary.
  if ($result['numFound'] > $limit && $sort_by != 'random') {
    // Initialize the pager.
    pager_default_initialize($result['numFound'], $limit);

    // Get the parameters for the pager links, but remove the offset key.
    $parameters = dsb_portal_rest_api_get_search_from_globals();
    unset($parameters['offset']);

    // Render it.
    $themed_pager = theme('pager', array(
      'parameters' => $parameters,
    ));
  }
  else {
    $themed_pager = '';
  }

  // It is probable the CSS was already added, because it is attached to the
  // search form. However, as a failsafe, we add it here explicitly.
  drupal_add_css(drupal_get_path('module', 'dsb_portal') . '/css/dsb_portal.search.css');

  return $themed_form . $themed_results . $themed_pager;
}

/**
 * Search form callback.
 *
 * Defines the dsb Portal search form. This form allows user to search the
 * Swiss national catalog of the digital school library for specific resources.
 *
 * @see _dsb_portal_rest_api_get_facet_checkboxes()
 * @see js/dsb_portal.facets.js
 */
function dsb_portal_rest_api_search_form($form, $form_state, $facets = array()) {
  // We do use a POST method, because this allows us to use the validation
  // and submission callbacks. GET forms in Drupal don't register callbacks.
  // However, upon submission, we simply redirect to the search page, putting
  // the POSTed parameters in the URL. The reason we do this is because of the
  // "reset" button. The reset button will redirect to the search page with
  // no parameters. But to know which button was pressed, we need the form
  // callbacks... Because of the redirect, POSTed values get lost and Drupal
  // doesn't put in the default values anymore. This is why we have to add the
  // default values by hand.

  $path = drupal_get_path('module', 'dsb_portal');

  $form['#attributes'] = array(
    'class' => array('dsb-portal-search-form'),
  );

  $form['#attached'] = array(
    // This enables a very small hack for recursively checking checkboxes.
    // See the JS file's source for more information.
    'js' => array(
      $path . '/js/dsb_portal.facets.js',
    ),
    'css' => array(
      $path . '/css/dsb_portal.search.css',
    ),
  );

  $form['query'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($_GET['query']) ? $_GET['query'] : '',
  );

  $form['do_search'] = array(
    '#type' => 'submit',
    '#value' => t("Search"),
    '#name' => 'do_search',
  );

  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t("Reset"),
    '#name' => 'reset',
  );

  // Render the facets.
  if (!empty($facets)) {
    $weight = 100;
    foreach ($facets as $facet_id => $facet_data) {
      // Only render a group if there are actually children to show.
      if (!empty($facet_data['childTerms'])) {
        // Group the facets in a collapsible fieldset.
        $form["filter_group_{$facet_id}"] = array(
          '#type' => 'fieldset',
          '#title' => _dsb_portal_rest_api_get_facet_name($facet_data),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#weight' => $weight++,
        );

        // Recursively create a tree of faceted filters.
        _dsb_portal_rest_api_get_facet_checkboxes(
          $form["filter_group_{$facet_id}"],
          $facet_id,
          $facet_data['childTerms'],
          0
        );
      }
    }
  }

  // Provide a recap of all active filters. This is especially important for
  // filters that are not enabled as facets (like filtering by content partner).
  // This fieldset groups filters that were activated through the search,
  // regardless if they are used as facets or not. We need to do this *after*
  // rendering the facets, because each active filter will become a hidden input
  // in order to remain active. But facets will still have a checkbox; so, to
  // avoid duplicates, we do not add a hidden input for facets.
  if (!empty($_GET['filter'])) {
    $form['active_filters'] = array(
      '#type' => 'fieldset',
      '#title' => t("Active filters"),
      '#attributes' => array(
        'class' => array('dsb-portal-search-form__active-filters')
      ),
    );

    foreach ($_GET['filter'] as $group => $filters) {
      $form['active_filters'][$group] = array(
        '#prefix' => '<div class="dsb-portal-search-form__active-filters__group">',
        '#suffix' => '</div>',
        '#tree' => FALSE,
      );

      $form['active_filters'][$group]['label'] = array(
        '#prefix' => '<div class="dsb-portal-search-form__active-filters__group__label">',
        // Is this group part of the facets? If so, try to get its name.
        // Otherwise, just use its "raw" value.
        '#markup' => isset($facets[$group]) ?
          _dsb_portal_rest_api_get_facet_name($facets[$group]) :
          check_plain($group),
        '#suffix' => '</div>',
      );

      foreach ($filters as $i => $value) {
        // Render the "Remove" button.
        $form['active_filters'][$group]["remove_filter_{$group}_{$i}"] = array(
          '#type' => 'submit',
          '#prefix' => '<div class="dsb-portal-search-form__active-filters__group__item"><span class="dsb-portal-search-form__active-filters__group__item__label">' . check_plain($value) . '</span>',
          '#value' => t("Remove"),
          '#name' => "remove_filter_{$group}_{$i}",
          '#suffix' => '</div>',
          '#attributes' => array(
            'class' => array('dsb-portal-search-form__active-filters__group__item__button'),
          ),
        );

        // Do we need a hidden input in order to keep this filter active? If
        // it's a facet and already present in the form, we don't need it.
        if (!isset($form["filter_group_{$group}"][$value])) {
          $form['active_filters'][$group]["hidden_{$group}_{$i}"] = array(
            '#type' => 'hidden',
            '#value' => $value,
            '#parents' => array('filter', $group, $i),
          );
        }
      }
    }
  }

  return $form;
}

/**
 * Submission callback for dsb_portal_rest_api_search_form().
 *
 * See dsb_portal_rest_api_search_form() for explanation on the redirects.
 */
function dsb_portal_rest_api_search_form_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#name'] === 'reset') {
    $form_state['redirect'] = 'dsb-portal/search';
  }
  else {
    $post_values = array();
    foreach (array(
      'query',
      'filter'
    ) as $key) {
      $post_values[$key] = isset($_POST[$key]) ? $_POST[$key] : '';
    }

    // Reset all numeric keys. Default values will continue to work for the
    // facets, but hidden inputs will be easier to manage this way. Furthermore,
    // the remove buttons work with a 0-based index.
    if (!empty($post_values['filter']) && is_array($post_values['filter'])) {
      foreach ($post_values['filter'] as $group => $values) {
        $post_values['filter'][$group] = array_values($post_values['filter'][$group]);
      }
    }

    // Do we need to remove a filter?
    if (preg_match('/^remove_filter_/', $form_state['triggering_element']['#name'])) {
      list(, , $filter, $key) = explode('_', $form_state['triggering_element']['#name']);
      if (!empty($post_values['filter'][$filter][$key])) {
        unset($post_values['filter'][$filter][$key]);
      }

      // Reset the keys again.
      if (!empty($post_values['filter'][$filter])) {
        $post_values['filter'][$filter] = array_values($post_values['filter'][$filter]);
      }
    }

    $form_state['redirect'] = array(
      'dsb-portal/search',
      array(
        'query' => $post_values,
      ),
    );
  }
}

/**
 * Get the facet human-readable name from a facet.
 *
 * The facet can have multiple formats. The main format is a facet with a "name"
 * property, which is an array, keyed by language. Some facets have no
 * multilingual data, so the "name" property is simply a string. As a fallback,
 * we might even return the Ontology ID.
 *
 * The data returned by this function can be altered by
 * hook_dsb_portal_facet_name_alter().
 *
 * @see hook_dsb_portal_facet_name_alter()
 *
 * @param object $facet
 *    A facet object.
 * @param bool $no_alter
 *    (optional) Skips the call to hook_dsb_portal_facet_name_alter()
 *    implementations. Defaults to false.
 *
 * @return string
 *    The facet name.
 */
function _dsb_portal_rest_api_get_facet_name($facet, $no_alter = FALSE) {
  global $language;

  if (is_array($facet['name']) && isset($facet['name'][$language->language])) {
    $facet_name = $facet['name'][$language->language];
  }
  elseif (is_string($facet['name'])) {
    $facet_name = $facet['name'];
  }
  elseif (isset($facet['ontologyId'])) {
    $facet_name = $facet['ontologyId'];
  }
  else {
    $facet = 'n/a';
  }

  if (!$no_alter) {
    drupal_alter('dsb_portal_facet_name', $facet_name, $facet);
  }

  return check_plain($facet_name);
}

/**
 * Construct the facet tree.
 *
 * Based on data from the Ontology server, passed with the search results,
 * recursively construct a tree of faceted-filters as checkboxes.
 *
 * The form element is passed by referenced and modified directly.
 *
 * @see dsb_portal_rest_api_search_form()
 * @see js/dsb_portal.facets.js
 *
 * @param array &$form
 *    The form element to which the checkboxes should be added.
 * @param string $root_facet_name
 *    The name of the "root" facet. This is the one actually used by the
 *    search query for filtering.
 * @param array $facets
 *    The facets to render as checkboxes.
 * @param int $depth
 *    (optional) The depth at which the facet is located.
 * @param string $parent_facet_value
 *    (optional) The parent facet field value. This is used to recursively check
 *    checkboxes, if needed. Defaults to an empty string.
 */
function _dsb_portal_rest_api_get_facet_checkboxes(&$form, $root_facet_name, $facets, $depth, $parent_facet_value = '') {
  foreach ($facets as $facet_data) {
    // Fetch the facet name from the facet data.
    $facet_name = _dsb_portal_rest_api_get_facet_name($facet_data, TRUE);

    // Fetch the facet name again, but this time allow modules to alter it. We
    // do this to make sure we send the correct data to the API. If a facet is
    // altered, it may not be valid anymore.
    $altered_facet_name = _dsb_portal_rest_api_get_facet_name($facet_data);

    // Get the facet ID, one we can use to send to the API. Usually, this is the
    // Ontology ID. But, in some cases (like language), there is no Ontology ID.
    // In that case, fallback to the facet name, hoping the API will recognize
    // it.
    $facet_id = !empty($facet_data['ontologyId']) ? $facet_data['ontologyId'] : $facet_name;

    // Get the facet result count.
    $facet_count = !empty($facet_data['resultCount']) ? $facet_data['resultCount'] : 0;

    $form[$facet_id] = array(
      '#type' => 'checkbox',
      '#title' => $altered_facet_name . (!empty($facet_count) ? " ($facet_count)" : ''),
      // The default value is gotten via the GET params, just as for the rest
      // of the form.
      '#default_value' => isset($_GET['filter'][$root_facet_name]) && in_array($facet_id, $_GET['filter'][$root_facet_name]) ? $facet_id : 0,
      // We construct the field value here. The following will result in
      // a field being treated as filter[group][facet]=facet. This way we can use
      // it as is, instead of using complex redirection workflows.
      '#parents' => array('filter', $root_facet_name, $facet_id),
      // This is very important. We want this field to equal the facet name,
      // not "1" (as is the default with Drupal).
      '#return_value' => $facet_id,
      // Indent the element, if necessary.
      '#prefix' => theme('indentation', array('size' => $depth)),
    );

    // Make sure it will get checked if the parent checkbox is checked.
    // This only applies if the checkbox is not at the root ($depth > 1).
    if ($depth && $parent_facet_value) {
      $form[$facet_id]['#states'] = array(
        'checked' => array(
          // If the parent checkbox is checked, we check ours as well. Same for
          // unchecking. Careful: this does require a very small JS hack.
          // See js/dsb_portal.facets.js
          ':input[name^="filter[' . $root_facet_name . ']["][value="' . $parent_facet_value . '"]' => array('checked' => TRUE),
        ),
      );
    }

    // Does this element have any children? If so, render them as well.
    if (!empty($facet_data['childTerms'])) {
      _dsb_portal_rest_api_get_facet_checkboxes(
        $form,
        $root_facet_name,
        $facet_data['childTerms'],
        $depth + 1,
        $facet_id
      );
    }
  }
}

/**
 * Page callback for viewing a single LOM-CH description.
 */
function dsb_portal_rest_api_view_description(Educa\DSB\Client\Lom\LomDescriptionInterface $lom) {
  drupal_add_css(drupal_get_path('module', 'dsb_portal') . '/css/dsb_portal.description.css');
  return theme('dsb_portal_description', array('description' => $lom));
}

/**
 * Helper function to get the search query.
 *
 * Get the search query from the super globals. Return an array of parameters.
 *
 * @return array
 *    An array, keyed as follows:
 *    - query: The keywords searched for, as a single string.
 *    - filters: An array of filters, keyed by facet names.
 *    - offset: The numeric offset, in case of pagination.
 */
function dsb_portal_rest_api_get_search_from_globals() {
  $query = isset($_GET['query']) ? $_GET['query'] : '';
  $filters = !empty($_GET['filter']) ? $_GET['filter'] : array();
  $page = pager_find_page();

  return array(
    'query' => $query,
    'filters' => $filters,
    'offset' => $page * variable_get('dsb_portal_search_limit', 20),
  );
}

/**
 * Get a list of facets.
 *
 * @see hook_dsb_portal_rest_api_facets_alter()
 *
 * @return array
 *    A list of facets to use.
 */
function dsb_portal_rest_api_get_facets() {
  // Defaults.
  $facets = array(
    'learningResourceType',
  );

  // If we don't filter by current language, we also add the language as a
  // default.
  if (!variable_get('dsb_portal_search_only_current_lang', 0)) {
    $facets[] = 'language';
  }

  drupal_alter('dsb_portal_rest_api_facets', $facets);

  return $facets;
}

/**
 * Get a list of fields.
 *
 * @see hook_dsb_portal_rest_api_fields_alter()
 *
 * @return array
 *    A list of fields to use.
 */
function dsb_portal_rest_api_get_fields() {
  // Defaults.
  $fields = array(
    'metaContributorLogos',
    'ownerDisplayName',
    'ownerUsername',
  );

  drupal_alter('dsb_portal_rest_api_fields', $fields);

  return $fields;
}
