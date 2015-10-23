<?php

/**
 * @file alpha_pagination_handler_pagination.inc
 * Definition of alpha_pagination_handler_pagination.
 */

/**
 * Views area handler to display an alphabetic pagination representive of the entire
 * result set for this view and not just the limited number being displayed by the
 * view's configuration
 *
 * @ingroup views_area_handlers
 */
class alpha_pagination_handler_pagination extends views_handler_area {
  /**
   * Our option default definitions. Exclude the default options.
   */
  function option_definition() {
    // $options = parent::option_definition();
    $options['pre_letter_path'] = array(
      'default' => 'by-first-letter',
      'translatable' => FALSE,
    );
    $options['paginate_view_field'] = array(
      'default' => 'title',
      'translatable' => TRUE,
    );
    return $options;
  }

  /**
   * Our options form. Exclude the default options.
   */
  function options_form(&$form, &$form_state) {
    // parent::options_form($form, $form_state);
    $form['pre_letter_path'] = array(
      '#title' => t('Path to results view.'),
      '#type' => 'textfield',
      '#size' => 60,
      '#default_value' => $this->options['pre_letter_path'],
      '#description' => t('This is the path to the view that handles the search by letter. No beginning or ending slashes.'),
    );

    // Get an array list of all non-image, non-entity or other assorted reference fields.
    $fields = array('title' => 'title');

    $compound_field_types = array('name');
    $single_field_types = array('text', 'text_long', 'text_with_summary');
    $all_field_types = array_merge($single_field_types, $compound_field_types);
    foreach (field_info_field_map() as $field_name => $field_definition) {
      if (in_array($field_definition['type'], $all_field_types)) {
        if (in_array($field_definition['type'], $compound_field_types)) {
          $field_info = field_info_field($field_name);
          foreach (array_keys($field_info['columns']) as $compoundFieldKey) {
            $compound_field_field_name = sprintf('%s:%s', $field_name, $compoundFieldKey);
            $fields[$compound_field_field_name] = $compound_field_field_name;
          }
        } else {
          $fields[$field_name] = $field_name;
        }
      }
    }
    $form['paginate_view_field'] = array(
      '#title' => t('View field to paginate against.'),
      '#type' => 'select',
      '#options' => $fields,
      '#default_value' => $this->options['paginate_view_field'],
      '#description' => t('This will be the content field that drives the pagination.'),
    );
  }

  /**
   * Extract the SQL query from the query information.
   *
   * Once extracted, place it into the options array so it is passed to the render
   * function. This code was lifted nearly verbatim from the views module where the
   * query is constructed for the ui to show the query in the administrative area.
   * I am open to suggestions on how to make this better.
   */
  function post_execute(&$values) {
    $query = $this->view->build_info['query'];
    $quoted = $query->getArguments();
    $connection = Database::getConnection();
    foreach ($quoted as $key => $val) {
      if (is_array($val)) {
        $quoted[$key] = implode(', ', array_map(array($connection, 'quote'), $val));
      }
      else {
        $quoted[$key] = $connection->quote($val);
      }
    }
    $this->options['query'] = check_plain(strtr($query, $quoted));
  }

  /**
   * Render the alphabetic paginator
   *
   * @param bool $empty
   *   If this area should be emptym then return it as empty.
   *
   * @return string $paginator
   *   A string representing the complete paginator including linked and unlinked options.
   */
  function render($empty = FALSE) {
    // Initialize our alphabet array so it can be used regardless of caching.
    $alphabet = range('A', 'Z');
    $alphabet[] = 'all';

    // Check to see if this query is cached. If it is, then just pull our results set
    // from it so that things can move quickly through here. We're caching in the event
    // the view has a very large result set.
    if ($cache = cache_get('alpha_pagination:' . md5($this->options['query']))) {
      $has_results = $cache->data;
    }
    else {
      // Construct the actual SQL query for the view being generated. We then need to parse
      // it to short-circuit certain conditions that may exist and make any alterations.
      // This is not the most elegant of solutions, but it is very effective.
      $query_parts = explode("\n", $this->options['query']);

      // Get the base field. This will change depending on the type of view we are
      // putting the paginator on.
      $base_field = $this->view->base_field;

      // If we are dealing with a substring, then short circuit it as we are most likely
      // dealing with a glossary contextual filter.
      foreach ($query_parts as $k => $part) {
        if ($position = strpos($part, "SUBSTRING")) {
          $part = substr($part, 0, $position) . " 1 OR " . substr($part, $position);
          $query_parts[$k] = $part;
        }
      }

      // Evaluate the last line looking for anything which may limit the result set as we
      // need results against the entire set of data and not just what is configured in
      // the view.
      $last_line = array_pop($query_parts);
      if (substr($last_line, 0, 5) != "LIMIT") {
        $query_parts[] = $last_line;
      }

      // Construct the query from the array and change the single quotes from HTML special
      // characters back into single quotes.
      $query = join("\n", $query_parts);
      $query = str_replace("&#039;", '\'', $query);
      $query = str_replace("&amp;", '&', $query);
      $query = str_replace("&lt;", '<', $query);
      $query = str_replace("&gt;", '>', $query);

      // Based on our query, get the list of node id's that are affected. We will use these
      // to generate our alphabetical pagination.
      $nids = array();
      $result = db_query($query);
      while($idata = $result->fetchObject()) {
        $nids[] = $idata->$base_field;
      }

      // Initialize our results array with the FALSE state so each is disabled by default.
      $has_results = array();
      foreach ($alphabet as $letter) {
        $has_results[$letter] = FALSE;
      }

      // If we have nodes (which .... well we should), then get the distinct first letter
      // from our field tables and mark them as TRUE so their letters link up. Note that
      // the node title is a special case that we have to take from the node table as
      // opposed to the body or any custom fields.
      if (count($nids) > 0) {
        switch ($this->options['paginate_view_field']) {
          case 'title':
            $table = 'node';
            $field = 'title';
            $where = 'nid';
            break;
          default:
            if (strpos($this->options['paginate_view_field'], ':') === FALSE) {
              // Format field name and table for single value fields
              $field = $this->options['paginate_view_field'] . '_value';
              $table = 'field_data_' . $this->options['paginate_view_field'];
            } else {
              // Format field name and table for compound value fields
              $field = str_replace(':', '_', $this->options['paginate_view_field']);
              $field_name_components = explode(':', $this->options['paginate_view_field']);
              $table = 'field_data_' . $field_name_components[0];
            }
            $where = 'entity_id';
            break;
        }
        $result = db_query('SELECT DISTINCT(SUBSTR(' . $field . ', 1, 1)) AS letter
                            FROM {' . $table . '}
                            WHERE ' . $where . ' IN ( :nids )', array(':nids' => $nids));
        while($idata = $result->fetchObject()) {
          $has_results[strtoupper($idata->letter)] = TRUE;
        }
        // Set the cache with the results of our efforts.
        cache_set('alpha_pagination:' . md5($this->options['query']), $has_results, 'cache', CACHE_TEMPORARY);
      }
    }

    $elements = array('paginator' => array(
      '#type' => 'container',
      '#attributes' => array('class' => 'alpha_pagination'),
      '#attached' => array('css' => array(drupal_get_path('module', 'alpha_pagination') . '/css/alpha_pagination.css' => array('group' => CSS_THEME))),
    ));

    $items = array();
    foreach ($alphabet as $pager_option) {
      $items[] = $pager_option === 'all' || $has_results[$pager_option] === TRUE ?
        l(t(ucfirst($pager_option)), sprintf('%s/%s', $this->options['pre_letter_path'], $pager_option)) :
        $pager_option;
    }

    $elements['paginator'][] = array(
      '#theme' => 'item_list',
      '#items' => $items,
    );

    return drupal_render($elements);
  }
}