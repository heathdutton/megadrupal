<?php

/**
 * @file
 * Page include file that renders the TARDIS list of recent nodes.
 */

/**
 * This function builds each page according to the argument received.
 *
 * @return array
 *   A rendered, paged list of node teasers.
 */
function tardis_page($tardis_page_name) {
  // $tardis_page_name is the argument used to retrieve a TARDIS page from
  // the db. It's immediately stripped from the $args array afterwards.
  $args = func_get_args();
  array_shift($args);
  $tardis_page = db_select('tardis_pages', 'tp')
    ->fields('tp')
    ->condition('name', $tardis_page_name)
    ->execute()
    ->fetchObject();

  // Those are the page settings as retrieved from the db:
  $name = $tardis_page->name;
  $title_date = $tardis_page->title_date;
  $nodes_per_page = $tardis_page->nodes_per_page;
  // Node types are stored in serialized form and must be unserialized.
  $node_types = unserialize($tardis_page->node_types);

  // Fetch fields used by submodules in the query.
  $tardistoken_page_titles = array();
  _tardis_submodule_fields($tardis_page, 'page', $tardisdate_field, $tardisi18n_lang, $tardistoken_page_titles);

  // How did the user choose to append the current date to the page title?
  $tardis_page_title_date = $title_date;

  // Unless the user asked not to append the date to the page title:
  if ($tardis_page_title_date != 'no_date') {
    // Get the current page title.
    $title = drupal_get_title();

    // Retrieve URL arguments.
    $tardis_query_arg = _tardis_query_arg($nodes_per_page, $node_types);

    // If we have a month, there's a distinction between
    // how months are displayed: numbers or names.
    if (count($tardis_query_arg) > 1) {
      // Is there a tokenized title for year and month arguments?
      if (module_exists('tardistoken') && $tardistoken_page_titles['year_month'] != '') {
        // Token replacement as defined in tardistoken_tokens().
        // Here we set an array of options so that the callback
        // can differentiate between a TARDIS page and block.
        $tardis_options = array('object' => 'page');
        $title = token_replace($tardistoken_page_titles['year_month'], $tardis_options);
      }
      else {
        switch ($tardis_page_title_date) {
          // Month names:
          case 'years_month_names':
            // Retrieve month names using this helper function.
            $tardis_month_names = _tardis_month_names();
            $tardis_month_number = $tardis_query_arg[1];
            $title .= ": $tardis_month_names[$tardis_month_number], $tardis_query_arg[0]";
            break;

          // Month numbers:
          case 'years_month_numbers':
            $title .= ": $tardis_query_arg[1]/$tardis_query_arg[0]";
            break;
        }
      }
    }
    elseif (count($tardis_query_arg) == 1 && $tardis_query_arg[0] != '') {
      // Is there a tokenized title for a single year argument?
      if (module_exists('tardistoken') && $tardistoken_page_titles['year'] != '') {
        $tardis_options = array('object' => 'page');
        $title = token_replace($tardistoken_page_titles['year'], $tardis_options);
      }
      else {
        $title .= ": $tardis_query_arg[0]";
      }
    }
  }
  $title = filter_xss($title);
  drupal_set_title($title);

  // Calls the page query function.
  $result = _tardis_query_page($nodes_per_page, $node_types, $tardisdate_field, $tardisi18n_lang);

  // Assemble a list of NIDs.
  $nids = array();
  foreach ($result as $data) {
    $nids[] = $data->nid;
  }

  // Load a list of node objects with the NIDs.
  $nodes = node_load_multiple($nids);

  // TARDIS Render integration.
  if (module_exists('tardisrender')) {
    $tardisrender_view_mode = $tardis_page->tardisrender_view_mode;
  }
  else {
    $tardisrender_view_mode = 'teaser';
  }
  $views = node_view_multiple($nodes, $tardisrender_view_mode);

  // Add those objects to the output and render them.
  $output = '';
  foreach ($views as $view) {
    $output .= render($view);
  }

  // Page results?
  $output .= theme('pager');

  return $output;
}

/**
 * This function queries the db to build a page.
 *
 * @return array
 *   An array of nodes ready for rendering.
 */
function _tardis_query_page($nodes_per_page, $node_types, $tardisdate_field, $tardisi18n_lang) {
  // This is the query. The variable $nodes_per_page sets how many nodes
  // per page to be displayed.
  // The variable $node_types aids in filtering the node types that were chosen
  // to be displayed.
  // The query also limits nodes by access permission,
  // so if users have no permissions for certain node types,
  // they won't see them.
  $query = db_select('node', 'n')
    ->fields('n', array(
      'nid',
      'type',
      'created',
    ))
    ->addTag('node_access')
    ->extend('PagerDefault')
    ->condition('type', $node_types)
    ->limit($nodes_per_page);

  // Query joins made by submodules.
  _tardis_submodule_query_joins($query, $tardisi18n_lang, $tardisdate_field, $tardis_query_date);

  if (!module_exists('tardisdate')) {
    // If TARDIS Date is disabled,  order the query by the default created date.
    $query->orderBy('created', 'DESC');
  }

  // Were a year AND a month supplied? Let's see the last two arguments.
  // This helper function returns the year and month from the URL, if present.
  $tardis_query_arg = _tardis_query_arg();

  // If there are arguments to work with:
  if (isset($tardis_query_arg[0]) && $tardis_query_arg[0] != '') {
    // Is there a year and a month in the URL?
    if (count($tardis_query_arg) > 1 && preg_match('/^[0-9]{4}$/', $tardis_query_arg[0]) && preg_match('/^[0-9]{2}$/', $tardis_query_arg[1])) {
      $tardis_query_year = $tardis_query_arg[0];
      $tardis_query_month = $tardis_query_arg[1];

      // Define the first day of a month and the next as limits.
      $tardis_query_last_day = _tardis_query_last_day($tardis_query_year, $tardis_query_month);
      $tardis_query_date_max = $tardis_query_year . '-' . $tardis_query_month . '-' . $tardis_query_last_day;
      $tardis_query_date_min = $tardis_query_year . '-' . $tardis_query_month . '-01';
    }

    // Or just a year?
    if (count($tardis_query_arg) < 2 && preg_match('/^[0-9]{4}$/', $tardis_query_arg[0])) {
      $tardis_query_year = $tardis_query_arg[0];

      // Define the first day of the year and the next as limits.
      $tardis_query_date_min = $tardis_query_year . '-01-01';
      $tardis_query_date_max = $tardis_query_year . '-12-31';
    }

    // Determines whether date.module fields should be used.
    _tardisdate_query_dates($query, $tardis_query_date, $tardis_query_date_min, $tardis_query_date_max);
  }

  return $query->execute()->fetchAllAssoc('nid');
}
