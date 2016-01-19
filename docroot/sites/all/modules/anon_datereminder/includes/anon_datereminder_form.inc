<?php
/**
 * @file
 * Support for forms displaying or entering reminders.
 */

/**
 * Build the reminder form.
 *
 * @param string $form
 *   Name of the form? (New in D7)
 * @param array $form_state
 *   What's built so far
 * @param node $node
 *   The fully loaded node object.
 * @param array $fieldset
 *   Boolean that indicates if the reminder form should be in a fieldset.
 *
 * @returns array
 *   The constructed form
 */
function _anon_datereminder_form($form, &$form_state, $node, $fieldset = NULL) {
  $form = array();
  if (empty($node->nid)) {
    return;
  }

  $display = variable_get('anon_datereminder_theme', 'table');

  if (!_anon_datereminder_has_future_dates($node)) {
    // Already past last repeat. Don't present option.
    return $form;
  }

  $form['nid'] = array('#type' => 'value', '#value' => $node->nid);

  if ($fieldset == 'f') {
    $c = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => variable_get('anon_datereminder_fieldset_collapsed', 1),
      '#title' => t("Get reminder for '@title'",
            array('@title' => $node->title)),
    );
  }
  else {
    $c = array();
  }

  $tbl = array();
  $tbl['#table_name'] = 'anon_datereminder';

  $tbl['#table_cols'] = array(array('lead', t('Lead time')));
  $tbl['#table_cols'][] = array('email', t('Email Addresses'));

  // How many should we allow visitor to add at a time?
  $maxrem = variable_get('anon_datereminder_max_reminders', ANON_DATEREMINDER_MAX_REMINDERS);
  $minadd = variable_get('anon_datereminder_min_add', 1);
  $toadd = $maxrem;
  if ($toadd > $minadd) {
    $toadd = $minadd;
  }
  while ($toadd > 0) {
    $rows[] = "n$toadd";
    $tbl["anon_datereminder_lead_n$toadd"] = _anon_datereminder_leadtime_form(0);
    $tbl["anon_datereminder_email_n$toadd"] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#size' => 72,
      '#maxlength' => 512,
    );
    $toadd -= 1;
  }

  $tbl['#table_rows'] = $rows;
  $tbl['rids'] = array('#type' => 'value', '#value' => implode(',', $rows));

  $tbl['#theme'] = 'anon_datereminder_table';

  if ($display == 'fields') {
    $tbl['#theme'] = 'anon_datereminder_fields';
  }

  $c['anon_datereminder'] = $tbl;

  $allow_mail = user_access(ANON_DATEREMINDER_ADMINISTER_REMINDERS);
  if ($allow_mail) {
    $c['send-now'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send test email'),
      '#description' => t('Check to send example email now'),
      '#default_value' => FALSE,
    );
  }

  $c['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set reminder'),
    '#submit' => array('_anon_datereminder_form_submit_user'),
  );

  $form['collapse'] = $c;
  $form['#validate'][] = '_anon_datereminder_form_validate_user';

  return $form;
}

/**
 * Generates a form element to select a valid leadtime.
 */
function _anon_datereminder_leadtime_form($default) {
  if ($default == 0) {
    $menu_values[0] = t('Add reminder');
  }
  else {
    $menu_values[0] = t('Delete reminder');
  }

  $menu_values += _anon_datereminder_leadtime_list();

  return array(
    '#type' => 'select',
    '#default_value' => $default,
    '#options' => $menu_values,
  );
}

/**
 * Generates list of leadtime options.
 */
function _anon_datereminder_leadtime_list() {
  $leads = variable_get('anon_datereminder_leadtime_list', '');
  // Need to make default?
  if ($leads == '') {
    return _anon_datereminder_default_leadtime_list();
  }

  $intervals = explode(',', $leads);
  $opts = drupal_map_assoc($intervals, 'format_interval');
  return $opts;
}

/**
 * Get default leadtime list.
 */
function _anon_datereminder_default_leadtime_list() {
  $hour = 60 * 60;
  $day = $hour * 24;
  $intervals = array(
    $hour,
    $hour * 2,
    $hour * 3,
    $hour * 4,
    $hour * 5,
    $hour * 6,
    $hour * 7,
    $hour * 8,
    $hour * 9,
    $hour * 10,
    $hour * 11,
    $hour * 12,
    $hour * 10,
    $hour * 12,
    $hour * 14,
    $hour * 16,
    $hour * 18,
    $day * 1,
    $day * 2,
    $day * 3,
    $day * 4,
    $day * 5,
    $day * 6,
  );
  return drupal_map_assoc($intervals, 'format_interval');
}

/**
 * Create a disablayable text item for a table form.
 *
 * @param string $str
 *   String value to be displayed.
 *
 * @returns array A form item.
 */
function _anon_datereminder_form_text_item($str) {
  return array(
    '#type' => 'item',
    '#markup' => $str,
  );
}
/**
 * Build a form with a list of reminders.
 *
 * @param array $formname
 *   The name of the form
 * @param array $form_state
 *   Current state of the form
 * @param object $arg
 *   node object
 * @param string $content
 *   Determines content of the list. Allowed values:
 *    'all_node' -- Show all reminders for this node
 *    'all' -- Show all reminders
 * @param string $fieldset
 *   String that indicates if the reminder form should be in a fieldset.
 *
 * @return array
 *   Form array
 *
 * @ingroup forms
 */
function _anon_datereminder_form_summary($formname, &$form_state, $arg, $content, $fieldset) {
  global $user;
  module_load_include('inc', 'anon_datereminder', 'includes/date');
  module_load_include('inc', 'anon_datereminder', 'includes/defines');
  module_load_include('inc', 'anon_datereminder', ANON_DATEREMINDER_DB);

  $form = array();
  $donode = FALSE;
  $allow_del = user_access(ANON_DATEREMINDER_ADMINISTER_REMINDERS);
  $sel = array();

  switch ($content) {
    case 'all_node':
      $sel['nid'] = $arg->nid;
      break;

    default:
      break;
  }

  $all = _anon_datereminder_load_reminders($sel, NULL, 'next_due');

  $reminders = array();

  foreach ($all as $rem) {
    $reminders[$rem->rid] = '';
    if ($rem->email != '' && ($allow_del == $user->uid)) {
      $emails[$rem->rid] = _anon_datereminder_form_text_item(
              str_replace(',', '<br/>', check_plain($rem->email)));
    }

    if ($donode) {
      $form['title'][$rem->rid]
        = _anon_datereminder_form_text_item(l($rem->title, 'node/' . $rem->nid));
    }
    // Get reminder leadtimes
    // Note: Can simplify this now that there's one reminder
    // per entry, not multiple.
    $times = drupal_map_assoc(array($rem->leadtime), 'format_interval');
    $stimes = implode('</br>', array_values($times));
    $form['leads'][$rem->rid] = _anon_datereminder_form_text_item($stimes);

    // Get time of next reminder in local tz.
    $dobj = _anon_datereminder_internal_date_to_datetime($rem->next_due);
    $tz = timezone_open(drupal_get_user_timezone());
    $dobj->setTimeZone($tz);
    // date_timezone_set($dobj, $tz);
    if ($rem->expired) {
      $n = _anon_datereminder_form_text_item(t('Expired'));
    }
    else {
      $shortformat = variable_get('date_format_short', 'm/d/Y - H:i');
      $n = _anon_datereminder_form_text_item($dobj->format($shortformat));
    }
    $form['next'][$rem->rid] = $n;
  }
  if (isset($emails)) {
    $form['email'] = $emails;
  }
  if ($allow_del) {
    $form['reminders']
      = array('#type' => 'checkboxes', '#options' => $reminders);
    if (!empty($reminders)) {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Delete selected reminders'),
        '#submit' => array('_anon_datereminder_admin_delete_set'),
      );
    }
  }
  $form['#theme'] = 'anon_datereminder_manage_reminders';

  return $form;
}

/**
 * Theme a table of reminders.
 *
 * @param array $variables
 *   Theme variables.
 *
 * @return string
 *   The rendered table
 */
function theme_anon_datereminder_manage_reminders($variables) {

  $form = &$variables['form table'];
  $output = '';
  $header = array();
  $has_reminders = isset($form['next']) && is_array($form['next']);
  if ($has_reminders) {
    $select_header = theme('table_select_header_cell');

    $douser = $doemail = $dotitle = FALSE;
    $header = array($select_header);
    $cols = 2;
    if (!empty($form['user'])) {
      $header[] = t('User');
      ++$cols;
      $douser = TRUE;
    }
    if (!empty($form['email'])) {
      $header[] = t('Email');
      ++$cols;
      $doemail = TRUE;
    }
    if (!empty($form['title'])) {
      $header[] = t('Event');
      ++$cols;
      $dotitle = TRUE;
    }
    $header[] = t('Lead time');
    $header[] = t('Next Remind At');
    $output = '';

    foreach (element_children($form['next']) as $rid) {
      $row = array();
      $row[] = drupal_render($variables['form table']['reminders'][$rid]);

      if ($douser) {
        $row[] = drupal_render($variables['form table']['user'][$rid]);
      }
      if ($doemail) {
        $row[] = drupal_render($variables['form table']['email'][$rid]);
      }
      if ($dotitle) {
        $row[] = drupal_render($variables['form table']['title'][$rid]);
      }
      $row[] = drupal_render($variables['form table']['leads'][$rid]);
      $row[] = drupal_render($variables['form table']['next'][$rid]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No reminders pending')));
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($variables['form table']);

  return $output;
}

/**
 * Theme function to render a anon_datereminder table.
 *
 * @param array $variables
 *   Theme variables.
 *
 * @return array
 *   The rendered table
 */
function theme_anon_datereminder_table(&$variables) {

  $form = $variables['form'];
  $output = '';

  $header = array();
  $rows = array();
  if (isset($form['#table_select'])) {
    $header[] = theme('table_select_header_cell');
  }

  foreach ($form['#table_cols'] as $c) {
    if ($c[0] == 'select') {
      $header[] = theme('table_select_header_cell');
    }
    else {
      $header[] = $c[1];
    }

  }
  $tbl = $form['#table_name'];

  foreach ($form['#table_rows'] as $rtag) {
    $row = array();
    foreach ($form['#table_cols'] as $c) {
      $ctag = $c[0];

      $row[] = drupal_render($variables['form']["${tbl}_${ctag}_${rtag}"]);
    }
    $rows[] = $row;
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($variables['form']);

  return $output;
}

/**
 * Theme function to render a anon_datereminder form as fields.
 *
 * @param array $variables
 *   Theme variables.
 *
 * @return array
 *   The rendered fields
 */
function theme_anon_datereminder_fields(&$variables) {
  $output = drupal_render_children($variables['form']);

  return $output;
}


/**
 * Check if this reminder has future dates.
 *
 * Check if this node has reminders enabled, and if there are any
 * occurrances of the date field in the future.
 *
 * @param node  $node
 *   The node to query
 *
 * @todo
 *   Should this test the type enabled variable first?
 */
function _anon_datereminder_has_future_dates($node) {
  module_load_include('inc', 'anon_datereminder', 'includes/date');

  $type = $node->type;
  $dfname = variable_get("anon_datereminder_datefield_$type", '');

  if ($dfname != '') {
    $dfcontent = $node->{$dfname};
    return (_anon_datereminder_get_next_occurrence($dfcontent, 0) != NULL);
  }
  return FALSE;
}
