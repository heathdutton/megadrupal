<?php

/**
 * @file
 * User page callbacks for the AChecker integration module.
 */

/**
 * Menu callback for general reporting.
 */
function achecker_integration_admin_report_page() {

  $build = array();
  $rows = array();

  $header = array(
    array('data' => t('nid')               , 'field' => 'nid', 'sort' => 'desc'),
    array('data' => t('Title')             , 'field' => 'title'),
    array('data' => t('Status')            , 'field' => 'status'),
    array('data' => t('Errors')            , 'field' => 'error_qty'),
    array('data' => t('Likely problems')   , 'field' => 'likely_qty'),
    array('data' => t('Potential problems'), 'field' => 'potential_qty'),
    array('data' => t('Operations')),
  );

  $query = db_select('achecker_integration_scan', 'AIS')
    ->extend('PagerDefault')
    ->extend('TableSort');
  $query->fields('AIS', array(
    'nid',
    'status',
    'error_qty',
    'likely_qty',
    'potential_qty',
  ));
  $query->fields('N', array('title', 'vid'));
  $query->innerJoin('node', 'N', 'N.vid = AIS.vid');

  $results = $query->limit(50)->orderByHeader($header)->execute();
  foreach ($results as $result) {
    $row = array();
    array_push($row, $result->nid);
    array_push($row, l($result->title, 'node/' . $result->nid));
    array_push($row, $result->status);
    array_push($row, $result->error_qty);
    array_push($row, $result->likely_qty);
    array_push($row, $result->potential_qty);
    array_push($row, l(t('inspect'), 'admin/reports/achecker/' . $result->vid));
    array_push($rows, $row);
  }

  $build['achecker_integration_table'] = array(
    '#theme'  => 'table',
    '#header' => $header,
    '#rows'   => $rows,
    '#empty'  => t('No nodes have been scanned.'),
  );
  $build['achecker_integration_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Menu callback for specific scan reporting.
 */
function achecker_integration_admin_report_page_vid($form, $form_state) {

  $webservice_url = variable_get('achecker_integration_webservice_url', 'http://achecker.ca') . '/checker/suggestion.php?id=';

  $form['vid'] = array(
    '#type'  => 'value',
    '#value' => $form_state['build_info']['args'][0],
  );

  $form['Error'] = array(
    '#collapsed'   => TRUE,
    '#collapsible' => TRUE,
    '#description' => t('Errors confirmed via automation.'),
    '#title'       => 'Errors',
    '#type'        => 'fieldset',
  );

  $form['Likely Problem'] = array(
    '#collapsed'   => TRUE,
    '#collapsible' => TRUE,
    '#title'       => 'Likely Problems',
    '#description' => t('Likely problems requiring human analysis.'),
    '#type'        => 'fieldset',
  );

  $form['Potential Problem'] = array(
    '#collapsed'   => TRUE,
    '#collapsible' => TRUE,
    '#description' => t('Potential problems requiring human analysis.'),
    '#title'       => 'Potential Problems',
    '#type'        => 'fieldset',
  );

  $form['PASS'] = array(
    '#collapsed'   => TRUE,
    '#collapsible' => TRUE,
    '#description' => t('Potential/Likely problems judged as not issues.'),
    '#title'       => 'Passes',
    '#type'        => 'fieldset',
  );

  $query = db_select('achecker_integration_scan_result', 'AISV');
  $query->fields('AISV', array(
    'type',
    'line',
    'col',
    'message',
    'suggestion_id',
    'source',
    'repair',
    'sequence_id',
    'decision_pass',
    'decision_fail',
    'decision_made',
    'decision_made_date',
  ));
  $query->condition('AISV.vid', $form_state['build_info']['args'][0]);

  $results = $query->execute();
  foreach ($results as $result) {

    // Identify if a decision has previously been made.
    if ($result->decision_made) {
      $decision_made = substr($result->decision_made, 0, 1);
    }
    else {
      $decision_made = 'N';
    }

    switch ($result->type) {

      // AChecker has flagged an error.
      case 'Error':
        $form[$result->type][] = array(
          '#description'   => t(
            'Line #@line<br />Column: #@col<br />Source: @source',
            array(
              '@col' => $result->col,
              '@line' => $result->line,
              '@source' => $result->source,
            )
          ),
          '#title'       => l(ucfirst($result->message), $webservice_url . $result->suggestion_id),
          '#type'        => 'item',
          '#markup'      => check_plain($result->repair),
        );
        break;

      // A pass decision has been made on a previously flagged error.
      case $result->decision_made == 'PASS':
        $form[$result->decision_made]['seq_' . $result->sequence_id] = array(
          '#default_value' => $decision_made,
          '#description'   => t(
            'Line #@line<br />Column: #@col<br />Decision date: @decision_made_date<br />Source: @source',
            array(
              '@col'                => $result->col,
              '@line'               => $result->line,
              '@source'             => $result->source,
              '@decision_made_date' => $result->decision_made_date,
            )
          ),
          '#options'       => array(
            'N' => t('No decision'),
            'P' => $result->decision_pass,
            'F' => $result->decision_fail,
          ),
          '#title'         => l(ucfirst($result->message), $webservice_url . $result->suggestion_id),
          '#type'          => 'radios',
        );
        break;

      // AChecker has flagged a likely / potential problem.
      case 'Likely Problem':
      case 'Potential Problem':
        $form[$result->type]['seq_' . $result->sequence_id] = array(
          '#default_value' => $decision_made,
          '#description'   => t(
            'Line #@line<br />Column: #@col<br />Source: @source',
            array(
              '@col' => $result->col,
              '@line' => $result->line,
              '@source' => $result->source,
            )
          ),
          '#options'       => array(
            'N' => t('No decision'),
            'P' => $result->decision_pass,
            'F' => $result->decision_fail,
          ),
          '#title'         => l(ucfirst($result->message), $webservice_url . $result->suggestion_id),
          '#type'          => 'radios',
        );
        break;
    }
  }

  // Append quantity of errors to fieldset titles.
  foreach ($form as &$element) {
    if ($element['#type'] == 'fieldset') {
      $element['#title'] .= ' (' . (count($element) - 5) . ')';
    }
  }

  // Add submit button.
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Apply decisions'),
  );

  return $form;
}

/**
 * Validate handler for specific scan reporting.
 */
function achecker_integration_admin_report_page_vid_validate($form, &$form_state) {

  $node = node_load(NULL, $form_state['values']['vid']);
  if (!$node) {
    form_error($form, t('Could not load node.'));
    return;
  }
  $form_state['values']['nid'] = $node->nid;

  $session = db_select('achecker_integration_scan', 'AIS')
    ->fields('AIS', array('session'))
    ->condition('vid', $node->vid)
    ->execute()
    ->fetchField();
  $form_state['values']['session'] = $session;

  $webservice_url = parse_url(variable_get('achecker_integration_webservice_url', 'http://achecker.ca') . '/decisions.php');
  $query_string = array(
    'id'      => variable_get('achecker_integration_webservice_id', ''),
    'output'  => 'rest',
    'session' => $session,
    'uri'     => url('/node/' . $node->nid, array('absolute' => TRUE)),
  );
  foreach ($form_state['values'] as $sequence => $decision) {
    if (strpos($sequence, 'seq_') === 0) {
      $key = substr($sequence, 4);
      $query_string[$key] = $decision;
    }
  }

  $url = $webservice_url['scheme']
    . '://' . $webservice_url['host']
    . $webservice_url['path']
    . '?'
    . http_build_query($query_string);

  // Send decisions.
  $result = drupal_http_request($url);

  // If http errors are encountered handle them.
  if ($result->code != 200) {
    watchdog(
      'AChecker Integration',
      'Could not contact AChecker service at @url, code @code (@error).',
      array(
        '@code'  => $result->code,
        '@error' => $result->error,
        '@url'   => $url,
      ),
      WATCHDOG_ERROR
    );
    form_error($form, t('Could not contact AChecker service.'));
    return;

  }
  elseif (!$result->data) {
    watchdog(
      'AChecker Integration',
      'AChecker service at @url, returned no data.',
      array(
        '@url'   => $url,
      ),
      WATCHDOG_ERROR
    );
    form_error($form, t('AChecker service returned no data.'));
    return;
  }

  // No http errors, process result.
  $dom = new DOMDocument();

  // If XML can be loaded.
  if ($dom->loadXML($result->data)) {

    // Check for errors from AChecker response.
    $errors = $dom->getElementsByTagName('errors')->item(0);
    if ($errors) {
      watchdog(
        'AChecker Integration',
        'AChecker encountered error code @code (@error).',
        array(
          '@code'  => $errors->getElementsByTagName('error')->item(0)->getAttribute('code'),
          '@error' => strip_tags(
            $errors->getElementsByTagName('error')->item(0)->getElementsByTagName('message')->item(0)->nodeValue
          ),
        ),
        WATCHDOG_ERROR
      );
      form_error($form, t('AChecker encountered errors.'));
      return;
    }
  }

  // If XML cannot be loaded.
  else {
    watchdog(
      'AChecker Integration',
      'Could not interpret AChecker service response, XML may be invalid.',
      array(),
      WATCHDOG_ERROR
    );
    form_error($form, t('Could not interpret AChecker service response.'));
    return;
  }
}

/**
 * Submit handler for specific scan reporting.
 */
function achecker_integration_admin_report_page_vid_submit($form, &$form_state) {

  // Perform rescan.
  achecker_integration_scan(
    array($form_state['values']['nid']),
    $form_state['values']['session']
  );
}
