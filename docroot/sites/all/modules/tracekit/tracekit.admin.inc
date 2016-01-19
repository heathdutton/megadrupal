<?php

/**
 * Shows a single TraceKit report.
 */
function tracekit_report_details($id) {
  $result = db_query("SELECT * FROM {tracekit_report} WHERE tracekit_report_id = :id", array(':id' => $id))->fetchObject();

  $no_value = '<em>&lt;none&gt;</em>';

  if ($report = $result) {
    if (isset($report->stack)) {
      $stack = json_decode($report->stack);

      if (count($stack)) {
        foreach ($stack as $key => $trace) {
          $data = array(
            'url' => $no_value,
            'function' => $no_value,
            'arguments' => $no_value,
            'line' => $no_value,
            'column' => $no_value,
            'context' => $no_value,
          );

          if (isset($trace->context) && count($trace->context)) {
            $data['context'] = '<pre>';

            foreach ($trace->context as $context) {
              $data['context'] .= "\n" . check_plain($context);
            }

            $data['context'] .= '</pre>';
          }

          if (isset($trace->url)) {
            $data['url'] = check_plain($trace->url);
          }

          if (isset($trace->func)) {
            $data['function'] = check_plain($trace->func);
          }

          if (isset($trace->args)) {
            $data['arguments'] = check_plain($trace->args);
          }

          if (isset($trace->line)) {
            $data['line'] = check_plain($trace->line);
          }

          if (isset($trace->column)) {
            $data['column'] = check_plain($trace->column);
          }

          $rows = array(
            array(
              array('data' => t('URL'), 'header' => TRUE),
              $data['url'],
            ),
            array(
              array('data' => t('Function'), 'header' => TRUE),
              $data['function'],
            ),
            array(
              array('data' => t('Arguments'), 'header' => TRUE),
              $data['arguments'],
            ),
            array(
              array('data' => t('Line'), 'header' => TRUE),
              $data['line'],
            ),
            array(
              array('data' => t('Column'), 'header' => TRUE),
              $data['column'],
            ),
            array(
              array('data' => t('Context'), 'header' => TRUE),
              $data['context'],
            ),
          );

          $stacktrace['stacktrace' . $key] = array(
            '#theme' => 'table',
            '#rows' => $rows,
          );
        }
      }
    }

    $data = array(
      'mode' => $no_value,
      'created' => $no_value,
      'message' => $no_value,
      'username' => $no_value,
      'url' => $no_value,
      'useragent' => $no_value,
      'stacktrace' => $no_value,
    );

    if (isset($report->mode)) {
      $data['mode'] = check_plain($report->mode);
    }

    if (isset($report->created)) {
      $data['created'] = format_date($report->created, 'long');
    }

    if (isset($report->message)) {
      $data['message'] = check_plain($report->message);
    }

    if (isset($report->uid)) {
      $account = user_load($report->uid);
      $data['username'] = theme('username', array('account' => $account));
    }

    if (isset($report->url)) {
      $data['url'] = l($report->url, $report->url);
    }

    if (isset($report->useragent)) {
      $data['useragent'] = check_plain($report->useragent);
    }

    if (isset($stacktrace) && count($stacktrace)) {
      $data['stacktrace'] = drupal_render($stacktrace);
    }

    $rows = array(
      array(
        array('data' => t('Mode'), 'header' => TRUE),
        $data['mode'],
      ),
      array(
        array('data' => t('Date'), 'header' => TRUE),
        $data['created'],
      ),
      array(
        array('data' => t('Message'), 'header' => TRUE),
        $data['message'],
      ),
      array(
        array('data' => t('User'), 'header' => TRUE),
        $data['username'],
      ),
      array(
        array('data' => t('Location'), 'header' => TRUE),
        $data['url'],
      ),
      array(
        array('data' => t('Useragent'), 'header' => TRUE),
        $data['useragent'],
      ),
      array(
        array('data' => t('Stacktrace'), 'header' => TRUE),
        $data['stacktrace'],
      ),
    );

    $build['tracekit_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
    );

    return $build;
  }
  else {
    return '';
  }
}
