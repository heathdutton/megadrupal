<?php

/**
 * @file
 * Page rendering callbacks for the Solr Best Bets module.
 */

/**
 * Renders the elevate.xml file.
 *
 * @param array $environment
 *   The environment definition returned by solr_best_bets_environment_load().
 */
function solr_best_bets_xml_page(array $environment) {
  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  module_load_include('inc', 'solr_best_bets', 'solr_best_bets.crud');
  echo solr_best_bets_get_xml($environment);
}

/**
 * Displays the search queries this node is a best bet for.
 *
 * @param stdClass $node
 *   The node being viewed.
 *
 * @return array
 *   The page's render array.
 */
function solr_best_bets_node_overview($form, &$form_state, $node) {
  $build = array();
  $environments = solr_best_bets_get_environments();
  $content_id = 'entity:node:' . $node->nid;

  // Get table rows.
  // @todo Move to API function.
  $sql = '
    SELECT environment, query_text, exclude
    FROM {solr_best_bets}
    WHERE content_id = :content_id
    ORDER BY environment, query_text, weight
  ';
  $result = db_query($sql, array(':content_id' => $content_id));

  $form['content_id'] = array(
    '#type' => 'value',
    '#value' => $content_id,
  );

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => array(
      'delete' => t('Remove as best bet for search query'),
      'exclude_yes' => t('Exclude content from result set'),
      'exclude_no' => t('Do not exclude content from result set'),
    ),
    '#default_value' => 'approve',
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('solr_best_bets_node_overview_submit'),
  );

  // Build each table row.
  $rows = array();
  foreach ($result as $record) {
    // Get human readable label of the environment.
    $env_id = $record->environment;
    $label = isset($environments[$env_id]) ? $environments[$env_id]['label'] : $env_id;

    // Generates ID, escapes so we can pmore easily parse in the submit handler.
    $id = rawurlencode($record->query_text) . ':' . rawurlencode($env_id);

    // Build row.
    $rows[$id] = array(
      'query' => check_plain($record->query_text),
      'exclude' => ($record->exclude) ? t('Yes') : t('No'),
      'environment' => check_plain($label),
    );
  }

  $header = array(
    'query' => t('Search query text'),
    'exclude' => t('Excluded from result set'),
    'environment' => t('Environment'),
  );

  // Build the table.
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('This content is not configured as a best bet for any search queries.'),
  );

  return $form;
}

/**
 * Form submission handler for solr_best_bets_node_overview().
 */
function solr_best_bets_node_overview_submit($form, &$form_state) {

  // Flag whether we the content should be excluded or not. This is ignored for
  // delete operations.
  $exclude = (int) ($form_state['values']['operation'] == 'exclude_yes');

  // Get query test and environment for best bets being acted on.
  $records = array_filter($form_state['values']['table']);
  foreach ($records as $id) {
    list($query_text, $environment) = array_map('rawurldecode', explode(':', $id));
    $records[$id] = array(
      'content_id' => $form_state['values']['content_id'],
      'query_text' => $query_text,
      'environment' => $environment,
      'exclude' => $exclude,
    );
  }

  // Take action on selected items.
  if ($records) {
    module_load_include('inc', 'solr_best_bets', 'solr_best_bets.crud');
    switch ($form_state['values']['operation']) {
      case 'delete':
        solr_best_bets_delete($records);
        break;

      case 'exclude_yes':
      case 'exclude_no':
        $exclude = ($form_state['values']['operation'] == 'exclude_yes');
        solr_best_bets_update_exclude($records, $exclude);
        break;
    }
  }
}

/**
 * Configure the node to rise to the top of various search results.
 *
 * @ingroup forms
 */
function solr_best_bets_node_form($form, $form_state, $node) {
  $content_id = 'entity:node:' . $node->nid;

  $form['content_id'] = array(
    '#type' => 'value',
    '#value' => $content_id,
  );

  $form['query_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Search query text'),
    '#description' => t('Search queries matching this text will show the content at the top of the search result set.'),
    '#maxlength' => 64,
  );

  // Only display environment option if there is a choice.
  $options = solr_best_bets_get_environment_options();
  if (count($options) > 1) {
    $form['environment'] = array(
      '#type' => 'select',
      '#title' => t('Search environment'),
      '#options' => $options,
      '#description' => t('The search environment the best bet will be applied to.'),
    );
  }
  else {
    $form['environment'] = array(
      '#type' => 'value',
      '#value' => key($options),
    );
  }

  $form['exclude'] = array(
    '#type' => 'select',
    '#title' => t('Exclude from results'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#description' => t('Exclude this content from searches queries matching the entered text.'),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 5,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add as best bet for search query'),
    '#submit' => array('solr_best_bets_node_form_submit'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'node/' . $node->nid . '/solr_best_bets',
    '#attributes' => array('title' => t('Return to the best bet overview page.')),
  );

  return $form;
}

/**
 * Form submission handler for solr_best_bets_node_form().
 */
function solr_best_bets_node_form_submit($form, &$form_state) {
  $node = $form_state['build_info']['args'][0];
  if (!empty($form_state['values']['query_text'])) {

    // Save the best bet, build status message and type based on the result.
    module_load_include('inc', 'solr_best_bets', 'solr_best_bets.crud');
    if (solr_best_bets_save($form_state['values'])) {
      $message = '%title is set as a best bet for the search query %query.';
      $type = 'status';
    }
    else {
      $message = 'Error setting %title as a best bet for the search query %query.';
      $type = 'error';
    }

    // Display the status message to the user.
    $args = array('%title' => $node->title, '%query' => $form_state['values']['query_text']);
    drupal_set_message(t($message, $args), $type);
  }

  $form_state['redirect'] = 'node/' . $node->nid . '/solr_best_bets';
}
