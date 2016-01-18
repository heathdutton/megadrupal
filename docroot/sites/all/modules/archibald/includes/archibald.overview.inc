<?php

/**
 * @file
 * controller for the frontend search
 */

require_once ARCHIBALD_PATH . 'includes/archibald.search.inc';

/**
 * menu callback for archibald content overview
 *
 * @return string html
 */
function archibald_content_overview() {
  global $language;

  $reset_form = FALSE;

  if (!user_access('view archibald content')) {
    $nid = variable_get('archibald_alternative_homepage_node', '');
    $node = '';
    if (!empty($nid)) {
      $node = node_load($nid);
    }
    if (!empty($node)) {
      $html = node_show($node);
    }
    else {
      $html = t('Welcome to our website');
    }
    return $html;
  }
  else {
    drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald.js');
    drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');

    $query = '';
    $facets = array();

    // Has the form been submitted?
    if (empty($_POST)) { // No submission
      // The URL calling this function contains and explicit query string
      // Let's use it and clear the facet filters.
      if (isset($_GET['query'])) {
        $_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY] = filter_xss($_GET['query']);
        unset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER]);
        $_GET['kst'] = 1;
      }

      // Do we explicitely want to keep the search query and/or facet filters?
      if (isset($_GET['kst'])) { // Keep both
        if (isset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY])) $query = $_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY];
        if (isset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER])) $facets = $_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER];
      } else { // Clear both
        unset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY]);
        unset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER]);
      }
    } else { // Form has been submitted
      // Reset search
      if (isset($_POST['op']) && $_POST['op'] == t('Reset')) {
        $reset_form = TRUE;
        unset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY]);
        unset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER]);
      } else {
        if (!empty($_POST['query'])) {
          // Set the query session variable
          $_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY] = filter_xss($_POST['query']);
        } elseif (!empty($_POST['ajax'])) {
          // get facet checkboxes
          foreach ($_POST as $key => $val) {
            if ( drupal_substr($key, 0, 6) == 'facet_') {
              if (!is_array($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER])) $_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER] = array();
              $_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER][drupal_substr($key, 6)] = array_keys($val);
            }
          }
        }
        if (isset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY])) $query = $_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY];
        if (isset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER])) $facets = $_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER];
      }
    }

    if (variable_get('archibald_solr_search_activ', 0) == 1 && module_exists('apachesolr')) {
      $search_type = 'solr';
      drupal_add_js(
        drupal_get_path('module', 'archibald') .
        '/js/client_overview_facets.js'
      );
    } else {
      $search_type = 'native';
    }

    $search = archibald_search_notices(
      $query,
      $facets,
      array(
        'search_type' => $search_type,
        'num_per_page' => variable_get('archibald_content_num_per_page', 25),
        'restrict_lang' => (variable_get('archibald_search_in_current_language', 1) == TRUE)
      )
    );
  }

  if ($search_type =='solr') {
    $search_form = drupal_get_form(
      'archibald_content_overview_form',
      $search['facet_options'],
      $search['num_results']
    );
  } else {
    $search_form = drupal_get_form('archibald_content_overview_form');
  }

  $html = theme(
    'archibald_content_overview',
    array(
      'search_result' => $search['results'],
      'search_form' => $search_form,
      'search_result_count' => $search['num_results'],
    )
  );

  if (!empty($_POST['ajax'])) {
    echo $html;
    die();
  }

  return $html;
}

/**
 * search formular for archibald resources
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_content_overview_form($form, &$form_state, $filter_options = array(), $search_results = 0) {

  $query = '';
  if (isset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY])) {
    $query = $_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_KEY];
  }

  $form['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Term'),
    '#default_value' => $query,
    '#size' => 40
  );

  if ($search_results > 0 && !empty($filter_options)) {
    $weight = 5;
    if( isset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER]) )
    {
      $form['reset_facets'] = array(
        '#markup' => l(t('Reset filters'), 'archibald',
          array(
            'query' => ( empty($query) ) ? '' : array('query' => $query),
            'attributes' => array('class' => array('archibald_reset_facets'))
          )
        ),
        '#weight' => 1000
      );
    }
    foreach ($filter_options as $filter_option_key => $filter_option) {
      // Count the number of results for this option
      $count = archibald_content_overview_form_countfacets($filter_option);
      // Show the accordion only if the facet filter contains an option with results
      if ($count) {
        $key = 'facet_' . $filter_option_key;
        $form[$key] = array(
          '#type' => 'fieldset',
          '#title' => t('Filter by') . ' ' .  t($filter_option_key),
          '#weight' => $weight,
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#tree' => TRUE,
        );
        archibald_content_overview_form_facets2form_recursiv(
          $form[$key],
          $filter_option,
          $key
        );
        $weight++;
      }
    }
  }

  $form['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#weight' => 9999,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#weight' => 9999,
  );
  $form['#theme'] = 'archibald_content_overview_form';

  return $form;
}

/**
 * process facets recursively and return count of results in an option
 *
 * @param array $options
 * @param int $count
 *
 */
function archibald_content_overview_form_countfacets($options, $count = 0) {
  foreach ($options as $option) {
    $count += $option->count;
    if (count($option->vals)) $count = archibald_content_overview_form_countfacets($option->vals, $count);
  }
  return( $count );
}

/**
 * process facets recursively and display them
 *
 * @param array $form
 * @param array $items
 * @param string $parent_key
 *   Populate the form elements
 */
function archibald_content_overview_form_facets2form_recursiv(&$form, $items, $parent_key) {

  foreach ($items as $item) {
    $form[$item->key] = array(
      '#type' => 'checkbox',
      '#title' => $item->name,
      '#default_value' => $item->activ,
      '#result_count' => $item->count,
      '#attributes' => array(
        'key' => $item->key,
        'class' => array('facet_id_' . $item->key),
      ),
      '#tree' => TRUE,
      '#parents' => array($parent_key, $item->key),
    );

    archibald_content_overview_form_facets2form_recursiv(
      $form[$item->key],
      $item->vals,
      $parent_key
    );
  }
}

/**
 * theme the search form
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_content_overview_form($form) {
  $form = reset($form);
  unset($form['query']['#title']);
  $html = '';
  $html .= '<div>';
  $html .= theme('textfield', $form['query']) . drupal_render($form['search']) . drupal_render($form['reset']);
  if( isset($_SESSION[ARCHIBALD_FRONT_END_SESSION_SEARCH_FILTER]) ) {
    $html .= drupal_render($form['reset_facets']);
  }
  $html .= '</div>';

  foreach (element_children($form) as $key) {
    if (drupal_substr($key, 0, 6) == 'facet_') {
      $html .= '<div class="details">';
      $html .= ' <p><a href="JavaScript:void(0);" id="' . $key . '" class="btndetails inactive remember_choice sorl_search_facet">' . '<span>' . t($form[$key]['#title']) . '</span></a></p>';
      $html .= ' <div style="display: block;" class="detailscontainer recusrivcontainer">';
      $html .= '  <ul class="' . $key . '">';
      $html .= theme_archibald_content_overview_facet_form($form[$key]);
      $html .= '  </ul>';
      $html .= '  <div class="clear"></div>';
      $html .= ' </div>';
      $html .= '</div>';
    }
  }

  return $html;
}

/**
 * not an offical theme functions, it ill called direcktly.
 * Here we render the filter checkboxes
 *
 * @param array $form
 * @param integer $sum_count
 *
 * @return string html
 */
function theme_archibald_content_overview_facet_form($form, &$sum_count = 0) {
  $content = '';
  $found_enabled = FALSE;
  foreach (element_children($form) as $checkbox_key) {
    $found_enabled_child = FALSE;

    // this nee ned to find out if a child have any matches
    $child_count = 0;
    $tmp_content = theme_archibald_content_overview_facet_form(
      $form[$checkbox_key],
      $child_count
    );

    // a child is enabled but all have not counts
    if ($child_count == -1) {
      $found_enabled       = TRUE;
      $found_enabled_child = TRUE;
      $child_count         = 0;
    }

    // echo '<pre>';
    // print_r( $checkbox_key );
    if ($form[$checkbox_key]['#default_value'] == TRUE) {
      // current child is enabled
      $found_enabled = TRUE;
    }

    $result_count = (INT) $form[$checkbox_key]['#result_count'];
    $child_count += $result_count;
    $sum_count += $child_count;

    // if current has 0 relts but its child ,get the result sum of child
    // work around for lerning resurce type
    if (empty($result_count) && !empty($child_count)) {
      $result_count = $child_count;
      $form[$checkbox_key]['#disabled'] = TRUE;
    }

    // append count to title
    $form[$checkbox_key]['#title'] .= ' (' . $result_count . ')';
    $form[$checkbox_key]['#attributes']['class'][] = 'sorl_search_facet_select';

    /* display item if:
     *  item or child have facet hits
     * OR
     *  item or child is activ (checkbox was checked)
     */

    if (!empty($child_count) ||
      $form[$checkbox_key]['#default_value'] == TRUE ||
      $found_enabled_child == TRUE
    ) {
      $content .= '<li>';
      if (!empty($form[$checkbox_key]['#disabled'])) {
        // only show title not the text box if it is disabled
        $content .= $form[$checkbox_key]['#title'];
      }
      else {
        $content .= drupal_render($form[$checkbox_key]);
      }

      if (!empty($tmp_content)) {
        $content .= '<ul>' . $tmp_content . '</ul>';
      }
      $content .= '</li>';
    }
  }

  // if a child is enabled but all have no facet hits
  // set to -1 to notify parent instance to let this tree open
  if ($found_enabled == TRUE && $sum_count < 1) {
    $sum_count = -1;
  }
  // die();
  return $content;
}

