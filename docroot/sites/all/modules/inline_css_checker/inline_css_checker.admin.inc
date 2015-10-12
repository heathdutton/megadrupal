<?php
define('INLINE_CSS_CHECKER_SCAN_PER_PASS', 20);

function inline_css_checker_admin_overview() {
  $output = array();

  $output['scan_form'] = drupal_get_form('inline_css_checker_scan_form');
  $output['table'] = inline_css_checker_admin_overview_table();

  return $output;
}

function inline_css_checker_admin_overview_table() {
  $output = array(
    'intro' => array(
      '#type' => 'markup',
      '#markup' => t('After you have performed the scan, results will appear in a table here.'),
    ),
  );

  $output['#attached'] = array(
    'css' =>  array(drupal_get_path('module', 'inline_css_checker') . '/inline_css_checker.admin.css'),
  );

  $header = array(
    array('data' => t('Type')),
    array('data' => t('Node (nid)')),
    array('data' => t('Revision (vid)')),
    array('data' => t('Snippet')),
    array('data' => t('Language')),
    array('data' => t('Alias')),
    array('data' => t('Actions')), // edit node
  );

  $query = db_select('inline_css_checker_results', 'iccr')->extend('PagerDefault');
  $query->fields('iccr');

  // @todo: Allow changing this.
  $result = $query->limit(100)
    ->orderBy('iccr.entity_id', 'DESC')
    ->execute();

  $rows = array();
  foreach ($result as $row) {
    // We don't need a perfectly-altered/whatever title here, just an indicator
    $title = db_query("select title from {node} where vid = :vid", array(':vid' => $row->revision_id))->fetchField();

    $rows[] = array('data' => array(
      $row->bundle,
      "{$title} ({$row->entity_id})",
      $row->revision_id,
      nl2br(check_plain($row->snippet)),
      $row->language,
      $row->alias,
      // @todo: Add ability to re-scan individual node
      l('Edit node', "node/{$row->entity_id}/edit"),
    ));
  }

  $output['contents'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t("No results to show. You either haven't completed a scan or you're living the dream."),
    '#attributes' => array('class' => array('inline-css-checker-results-table')),
  );

  $output['pager'] = array('#theme' => 'pager');

  return $output;
}

function inline_css_checker_scan_form($form, &$form_state) {
  $form = array();

  $form['scan'] = array(
    '#prefix' => t('<div>Click the button to refresh the table below.</div>'),
    '#type' => 'submit',
    '#value' => t('Scan for nodes containing inline CSS'),
    '#suffix' => t('<div>This might take a while.</div>'),
  );

  return $form;
}

function inline_css_checker_scan_form_submit(&$form, &$form_state) {
  // Delete any previous results from the table.
  db_delete('inline_css_checker_results')->execute();

  $operations[] = array('inline_css_checker_process_scan', array());
  // Set up and execute the batch.
  $batch = array(
    'operations' => $operations,
    'finished' => 'inline_css_checker_scan_finished',
    'title' => t('Scanning nodes for inline CSS'),
    'init_message' => t('Starting scan...'),
    'progress_message' => t('Scanning @pass nodes at a time.', array('@pass' => INLINE_CSS_CHECKER_SCAN_PER_PASS)),
    'error_message' => t('An error occurred while scanning.'),
    'file' => drupal_get_path('module', 'inline_css_checker') . '/inline_css_checker.admin.inc',
  );

  batch_set($batch);
}

function inline_css_checker_process_scan(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox'] = array();
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = db_query('SELECT COUNT(DISTINCT nid) FROM {node}')->fetchField();
  }

  // It might take a few seconds to render and parse each node, so do several per
  // iteration.
  $limit = INLINE_CSS_CHECKER_SCAN_PER_PASS;

  // With each pass through the callback, retrieve the next group of nids.
  $result = db_query_range("SELECT nid FROM {node} WHERE nid > :current_nid ORDER BY nid ASC", 0, $limit, array(':current_nid' => $context['sandbox']['current_node']), array('fetch' => PDO::FETCH_ASSOC));
  foreach ($result as $row) {
    // Here we actually perform our processing on the current node.
    $node = node_load($row['nid'], NULL, TRUE);

    // Add an indicator that it's us rendering the node so that other modules
    // can react appropriately (e.g. not show messages they normally would).
    // We don't save this.
    $node->inline_css_checker = TRUE;

    // Get the render array for the node.
    $node_array = node_view($node);

    // Get the rendered HTML for the node.
    $rendered_node = drupal_render($node_array);

    // Put that into a DOMDocument.
    $dd = new DOMDocument();
    $dd->loadHTML($rendered_node);

    $dx = new DOMXPath($dd);

    // Find style tags and tags with style attributes, and store them in our
    // results table as we go. That way, we still have some results even if
    // the batch fails for some reason.

    // Find and output the content of <style> tags
    foreach ($dx->query('//style') as $styleNode) {
      $context['results'][] = "{$node->title}";

      $potential_alias = url(drupal_get_path_alias("node/{$node->nid}"));

      $result = array(
        'entity_type' => 'node',
        'bundle' => $node->type,
        'entity_id' => $node->nid,
        'revision_id' => $node->vid,
        'language' => $node->language,
        // Leave blank if it doesn't have an alias
        'alias' => $potential_alias != url("node/{$node->nid}") ? $potential_alias : '',
        'snippet' => $dd->saveHTML($styleNode),
      );

      db_insert('inline_css_checker_results')
        ->fields($result)
        ->execute();
    }

    // Find and output the content of tags with style attributes
    foreach ($dx->query('//*[@style]') as $tagNode) {
      $context['results'][] = "{$node->title}";

      // @todo: Consider ignoring result when nodeValue is only display: none
      // or display: none;
      $result = array(
        'entity_type' => 'node',
        'bundle' => $node->type,
        'entity_id' => $node->nid,
        'revision_id' => $node->vid,
        'language' => $node->language,
        'alias' => isset($node->path) && isset($node->path['alias']) ? $node->path['alias'] : '',
        'snippet' => $dd->saveHTML($tagNode),
      );

      db_insert('inline_css_checker_results')
        ->fields($result)
        ->execute();
    }

    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['sandbox']['current_node'] = $node->nid;
    $context['message'] = t('Now processing %node (@progress/@max)', array(
      '%node' => $node->title,
      '@progress' => $context['sandbox']['progress'],
      '@max' => $context['sandbox']['max'],
    ));
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] >= $context['sandbox']['max'];
  }
}

function inline_css_checker_scan_finished($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = count($results) .' matches found.';
    drupal_set_message($message);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
}
