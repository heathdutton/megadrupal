<?php
/**
 * @file
 * Menu include file with admin pages of the Mail Tracking module.
 */

/**
 * Form builder; Builds the module settings form.
 */
function mail_tracking_admin_settings($form, &$form_state) {
  $form['mail_tracking_statistics_period'] = array(
    '#type' => 'select',
    '#title' => t('Statistics period'),
    '#description' => t('The statistics are calculated (or grouped) by the selected period. Changing this setting will not affect the current statistics.'),
    '#default_value' => variable_get('mail_tracking_statistics_period', 'week'),
    '#options' => array(
      'day' => t('Day'),
      'week' => t('Week'),
      'month' => t('Month'),
      'year' => t('Year'),
    ),
    '#required' => TRUE,
  );

  $options = array();
  for ($i = 1; $i < 4; $i++) {
    $options[$i . ' week'] = format_plural($i, '@count week', '@count weeks');
  }

  for ($i = 1; $i < 12; $i++) {
    $options[$i . ' month'] = format_plural($i, '@count month', '@count months');
  }

  $options['1 year'] = t('1 year');

  $form['mail_tracking_token_retention'] = array(
    '#type' => 'select',
    '#title' => t('Token retention'),
    '#description' => t('How long tokens will be kept in the database, after this period links will keep working but the actions are no longer tracked.'),
    '#default_value' => variable_get('mail_tracking_token_retention', '1 month'),
    '#options' => $options,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Menu callback; Builds a list of collected e-mail statistics.
 */
function mail_tracking_admin_statistics($sid = NULL) {
  if (isset($sid) && !(is_int($sid) || ctype_digit($sid))) {
    return drupal_not_found();
  }

  // Table headers.
  $headers = array(
    array('data' => t('Period / Newsletter'), 'field' => 'sid', 'sort' => 'desc'),
    array('data' => t('Module'), 'field' => 'module'),
    array('data' => t('Message'), 'field' => 'message_key'),
    array('data' => t('Send'), 'field' => 'send_count'),
    array('data' => t('Read'), 'field' => 'read_count'),
    array('data' => t('Clicked'), 'field' => 'clicked_count'),
    array('data' => t('Unsubscribed'), 'field' => 'unsubscribed_count'),
  );

  // Select the statistics.
  $query = db_select('mail_tracking_statistics', 's')->fields('s');
  $query->addJoin('LEFT', 'node', 'n', 's.nid = n.nid');
  $query->fields('n', array('title'));

  if (isset($sid)) {
    $query->condition('s.sid', $sid);
  }

  $query = $query->extend('PagerDefault')->limit(30);
  $query = $query->extend('TableSort')->orderByHeader($headers);
  $results = $query->execute();

  // "n/a" text.
  $na = drupal_placeholder(t('n/a'));

  // Loop over the different records.
  $rows = array();
  foreach ($results as $record) {
    if ($record->nid) {
      $title = l($record->title, 'node/' . $record->nid);
    }
    else {
      $title = _mail_tracking_format_period($record->period);

      if (user_access('access mail tracking tokens')) {
        $title = l($title, 'admin/reports/mail-tracking/tokens/' . $record->sid, array(
          'html' => TRUE,
        ));
      }
    }

    $rows[] = array(
      'data' => array(
        $title,
        $record->module,
        check_plain($record->message_key),
        $record->send_count,
        $record->read_count,
        ($record->clicked_count == '' ? $na : $record->clicked_count),
        ($record->unsubscribed_count == '' ? $na : $record->unsubscribed_count),
      )
    );
  }

  // Theme the table.
  $html = theme('table', array(
    'header' => $headers,
    'rows' => $rows,
    'sticky' => TRUE,
    'empty' => (isset($sid) ? t('The statistics could not be found.') : t('There are no statistics collected yet.')),
  ));

  // Theme the pager.
  $html .= theme('pager', array(
    'tags' => array()
  ));

  return $html;
}

/**
 * Menu callback; Builds a list of mail tracking tokens.
 */
function mail_tracking_admin_tokens($sid = NULL) {
  if (isset($sid) && !(is_int($sid) || ctype_digit($sid))) {
    return drupal_not_found();
  }

  // Table headers.
  $headers = array(
    array('data' => t('Period / Newsletter'), 'field' => 't.sid', 'sort' => 'desc'),
    array('data' => t('Module'), 'field' => 'module'),
    array('data' => t('Message'), 'field' => 'message_key'),
    array('data' => t('E-mail'), 'field' => 'mail'),
    array('data' => t('Read'), 'field' => 'is_read'),
    array('data' => t('Clicked'), 'field' => 'is_clicked'),
    array('data' => t('Unsubscribed'), 'field' => 'is_unsubscribed'),
    array('data' => t('Created'), 'field' => 'timestamp'),
  );

  // Select the statistics.
  $query = db_select('mail_tracking_token', 't')->fields('t');
  $query->addJoin('INNER', 'mail_tracking_statistics', 's', 't.sid = s.sid');
  $query->fields('s', array('period', 'module', 'message_key'));
  $query->addJoin('LEFT', 'node', 'n', 's.nid = n.nid');
  $query->fields('n', array('nid', 'title'));

  if (isset($sid)) {
    $query->condition('t.sid', $sid);
  }

  $query = $query->extend('PagerDefault')->limit(30);
  $query = $query->extend('TableSort')->orderByHeader($headers);
  $results = $query->execute();

  // "n/a" text.
  $na = drupal_placeholder(t('n/a'));

  // Loop over the different records.
  $rows = array();
  foreach ($results as $record) {
    if ($record->title) {
      $title = check_plain($record->title);
    }
    else {
      $title = _mail_tracking_format_period($record->period);
    }

    if (user_access('access mail tracking statistics')) {
      $title = l($title, 'admin/reports/mail-tracking/statistics/' . $record->sid, array(
        'html' => TRUE,
      ));
    }
    elseif ($record->nid) {
      $title = l($title, 'node/' . $record->nid, array(
        'html' => TRUE,
      ));
    }

    $rows[] = array(
      'data' => array(
        $title,
        $record->module,
        check_plain($record->message_key),
        check_plain($record->recipient),
        ($record->is_read ? t('yes') : t('no')),
        ($record->is_clicked == '' ? $na : ($record->is_clicked ? t('yes') : t('no'))),
        ($record->is_unsubscribed == '' ? $na : ($record->is_unsubscribed ? t('yes') : t('no'))),
        format_date($record->timestamp, 'short'),
      )
    );
  }

  // Theme the table.
  $html = theme('table', array(
    'header' => $headers,
    'rows' => $rows,
    'sticky' => TRUE,
    'empty' => (isset($sid) ? t('The tokens have already been cleaned-up.') : t('There are no tokens stored.')),
  ));

  // Theme the pager.
  $html .= theme('pager', array(
    'tags' => array()
  ));

  return $html;
}
