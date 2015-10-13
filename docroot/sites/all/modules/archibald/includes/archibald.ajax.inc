<?php

/**
 * @file
 * all ajax menu callback for archibald module
 */
require_once 'archibald.pages.inc';

/**
 * Page callback for new responsible editor user autocomplete
 *
 * @param string $string
 *   search string
 */
function archibald_user_editor_autocomplete_data($string = '') {
  $required_user_access = 'edit archibald content';

  $matches = array();
  if ($string != '') {
    $sql = 'SELECT u.name FROM {users} u ';
    $sql .= 'LEFT JOIN {users_roles} ur ON (u.uid=ur.uid) ';
    $sql .= 'LEFT JOIN {role_permission} p ON (ur.rid=p.rid) ';
    $sql .= 'WHERE u.name = :string AND p.permission = :required_user_access';

    $result = db_query_range(
      $sql, 0, 15, array(
        ':string' => $string,
        ':required_user_access' => $required_user_access,
      )
    );

    foreach ($result as $account) {
      $n = $account->name;
      // Commas and quotes in terms are special cases, so encode 'em.
      if (strpos($account->name, ',') !== FALSE || strpos($account->name, '"') !== FALSE) {
        $n = '"' . str_replace('"', '""', $account->name) . '"';
      }
      $matches[$n] = check_plain($account->name);
    }
  }

  drupal_json_output($matches);
}

/**
 * Search Tags for autocomplete
 *
 * @param string $type
 *  langstring, coverage
 * @param string $language
 *  2 char language identifier
 * @param string $search_string
 */
function archibald_autocomplete_data($type = '', $language_key = '', $search_string = '') {
  global $language;
  $min_search_length = 3;
  $max_autocomplete_entries = 10;
  $return_data = array();

  if (empty($search_string)) {
    $search_string = $language_key;
    $language_key = $language->language;
  }

  if (drupal_strlen($search_string) >= $min_search_length) {
    if ($type == 'langstring') {

      $sh = db_select('archibald_langstring_terms ', 'lt')
        ->fields(
          'lt',
          array(
            'term',
          )
        );
      $sh->where('LENGTH(term)<=200');
      $sh->condition('term', '%' . db_like($search_string) . '%', 'LIKE');
      $sh->range(0, $max_autocomplete_entries);

      $result = $sh->execute();
    }
    elseif ($type == 'keyword') {
      $args = array();
      $lang_filter = '';

      $sh = db_select('archibald_keyword_list ', 'kl');
      $sh->addField('kl', 'keyword', 'term');
      $sh->condition('keyword', '%' . db_like($search_string) . '%', 'LIKE');
      $sh->condition('language', $language_key, '=');
      $sh->range(0, $max_autocomplete_entries);
      $result = $sh->execute();
    }
    elseif ($type == 'coverage') {
      $args = array();
      $lang_filter = '';
      $just_search_within_language = variable_get('archibald_coverage_autocomplete_behaviour', 1);

      $sh = db_select('archibald_coverage_list ', 'cl');
      $sh->addField(
          'cl',
          'coverage',
          'term'
        );
      $sh->where('LENGTH(coverage)<=200');
      $sh->condition('coverage', '%' . db_like($search_string) . '%', 'LIKE');
      $sh->range(0, $max_autocomplete_entries);


      if (!empty($just_search_within_language)) {
        $sh->condition('language', $language_key, '=');
      }

      $result = $sh->execute();
    }

    $tmp_array = array();
    while ($row = $result->fetchAssoc()) {

      $content_output = drupal_substr($row['term'], 0, 255);

      preg_match("/(.*)" . preg_quote($search_string, '/') . '/', $row['term'], $matches);

      $near = drupal_strlen($matches[1]);
      if (!isset($tmp_array[$near])) {
        $tmp_array[$near] = array();
      }
      $tmp_array[$near][] = array(
        'content' => $content_output,
        'term' => $row['term'],
      );
    }
    foreach ($tmp_array as $i => & $tmp_arr) {
      foreach ($tmp_arr as & $term_arr) {
        $return_data[$term_arr['term']] = '<div class="langstring_autocomplete_row" ' . 'style="width: auto;border: ' . '1px solid black;padding:5px;white-space pre;">' . $term_arr['content'] . '</div>';
      }
    }
  }

  if (empty($return_data)) {
    die("[]");
  }

  drupal_json_output($return_data);
}

/**
 * lets download a file
 *
 * @param integer $fid
 * @param string $fileName
 */
function archibald_file_download($fid, $file_name) {

  $file = file_load((INT) $fid);
  if ($file_name == 'get_url') {
    echo archibald_file_get_url($file);
  }
  elseif (!empty($file)) {
    drupal_add_http_header('Content-Type', $file->filemime);
    drupal_add_http_header('Content-Disposition', 'attachment; filename="' . $file->filename . '"');
    drupal_add_http_header('Content-Length', $file->filesize);

    readfile($file->uri);
  }
  die();
}

/**
 * ajaxcalled function with display image preview
 *
 * @param integer $fid
 * @param string $style_name
 */
function archibald_file_preview_image($fid, $style_name = '') {
  $file = file_load((INT) $fid);

  echo theme(
    'image_style',
    array(
      'style_name' => (empty($style_name)) ? 'thumbnail' : $style_name,
      'path' => $file->uri,
      'getsize' => FALSE,
      'attributes' => array(
        'fid' => $file->fid,
      ),
    )
  );
  die();
}

/**
 * Returns html output which shows all changes for the given lom resource.
 *
 * @param string $lom_id
 *   The lom id
 *
 * @return string
 *   the html output
 */
function archibald_change_log($lom_id) {
  if (empty($lom_id)) {
    return '';
  }

  $res = db_select('archibald_change_log', 'acl')
    ->fields('acl', array(
      'language',
      'field',
      'old_value',
      'new_value',
    ))
    ->condition('lom_id', $lom_id)
    ->execute();
  $formated_array = array();
  while ($row = $res->fetchAssoc()) {
    if (!isset($formated_array[$row['language']])) {
      $formated_array[$row['language']] = array();
    }

    $formated_array[$row['language']][$row['field']] = array(
      'old' => $row['old_value'],
      'new' => $row['new_value'],
    );
  }
  $output = '<h3 style="border-bottom:1px solid black;width:400px;">' . t('Changes') . '</h3>';
  foreach ($formated_array AS $language => $fields) {
    if (empty($language)) {
      $output .= '<div class="change_log_language"><span>' . t('Global changes:') . '</span>';
    }
    else {
      $output .= '<div class="change_log_language"><span>' . t('Changes in language: @lang', array(
        '@lang' => t('@lang', array(
          '@lang' => $language)
        )
      )) . '</span>';
    }
    foreach ($fields AS $field => $values) {
      $output .= '<ul>';
      $output .= archibald_get_change_log_line($field, $values['old'], $values['new']);
      $output .= '</ul>';
    }
    $output .= '</div>';
  }

  echo $output;
  die();
}

/**
 * Returns a single formated row for the changelog.
 *
 * @param string $field
 *   the field
 * @param string $old_value
 *   the old value
 * @param string $new_value
 *   the new value
 *
 * @return string
 *   the html output line
 */
function archibald_get_change_log_line($field, $old_value, $new_value) {

  $tmp_arr = explode('|', $field, 2);
  $field = $tmp_arr[0];
  $output = '<li class="change_log_row">';
  $text_output = '';

  if ($old_value == 'ADDED') {
    $text_output = '<span class="archibald_change_log_added">' . t('@field added with value "@value"', array(
      '@field' => $field,
      '@value' => $new_value,
    )) . '</span>';
  }
  elseif ($new_value == 'DELETED') {
    $text_output = '<span class="archibald_change_log_deleted">' . t('@field removed with value "@value"', array(
      '@field' => $field,
      '@value' => $old_value,
    )) . '</span>';
  }
  elseif ($new_value == 'CHANGED') {
    $text_output = '<span class="archibald_change_log_old">' . t('@field changed', array(
      '@field' => $field,
    )) . '</span>';
  }
  elseif ($new_value == 'DELETED_PLAIN') {
    $text_output = '<span class="archibald_change_log_deleted">' . $field . '</span>';
  }
  else {
    $text_output = t('@field changed from "!old" to "!new"', array(
      '@field' => $field,
      '!old' => '<span class="archibald_change_log_old">' . $old_value . '</span>',
      '!new' => '<span class="archibald_change_log_new">' . $new_value . '</span>',
    ));
  }

  $output .= $text_output;


  return $output . '</li>';
}

/**
 * ajax menu callback for archibald content overview lifecylce.status selector
 *
 * @param string $lom_id
 */
function archibald_content_set_status($lom_id) {
  // Include the CTools tools that we need.
  ctools_include('ajax');
  ctools_include('modal');

  drupal_add_http_header('Content-Type', 'application/json');

  $lom = archibald_load($lom_id);


  if (empty($lom) || !($lom instanceof ArchibaldLom) || !archibald_is_new_proposal($lom)) {
    if (empty($lom) || !($lom instanceof ArchibaldLom)) {
      $output = theme('archibald_content_set_status_faulty', array('errors' => array(t('Invalid description'))));
      drupal_add_http_header('Content-Type', 'application/json');
      print ajax_render(array(ctools_modal_command_display(t('Set status'), $output)));
      die();
    }
    else if( !archibald_is_new_proposal($lom) ){
      $validator = new ArchibaldLomValidate($lom);

      if ($validator->readyForPublish() != TRUE) {
        // Can not switch status of lom object, cause it is faulty
        $output = theme('archibald_content_set_status_faulty', array('errors' => $validator->getErrors()));
        drupal_add_http_header('Content-Type', 'application/json');
        print ajax_render(array(ctools_modal_command_display(t('Set status'), $output)));
        die();
      }
    }
  }

  $form_state = array(
    'title' => t('Change status'),
    'ajax' => TRUE,
    'lom' => $lom,
    'admin_content_overview_type' => filter_xss(
      @$_REQUEST['admin_content_overview_type']
    ),
  );
  $commands = ctools_modal_form_wrapper('archibald_content_set_status_form', $form_state);

  if (!empty($form_state['was_saved']) || (!empty($form_state['values']['op']) && $form_state['values']['op'] == $form_state['complete form']['cancel']['#value'])) {
    $commands = array(ctools_modal_command_dismiss());
    $commands[] = ajax_command_replace(
      '#archibald_admin_content_overview', archibald_admin_content_overview(
        filter_xss(@$_REQUEST['admin_content_overview_type']), TRUE
      )
    );
  }

  drupal_add_http_header('Content-Type', 'application/json');
  print ajax_render($commands);
}

/**
 * status selector
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_content_set_status_form($form, &$form_state) {

  if (!isset($form_state['admin_content_overview_type'])) {
    $form_state['admin_content_overview_type'] = ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_DRAFT;
  }

  if (!isset($form_state['lom'])) {
    // Get LOM ID from second part in url path (archibald/%/set_status/form)
    $variables = array('lom_id' => arg(1));
    $form_state['lom'] = archibald_load($variables['lom_id']);
  }
  else {
    $variables = array('lom_id' => $form_state['lom']->getLomId());
  }

  $form['lom_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $variables['lom_id'],
    '#attributes' => array('class' => array('new_life_cycle_status_lom_id')),
  );

  $form['admin_content_overview_type'] = array(
    '#type' => 'hidden',
    '#default_value' => $form_state['admin_content_overview_type'],
  );

  $old_status = '';
  if (!empty($_POST['current_value'])) {
    $old_status = check_plain($_POST['current_value']);
  }
  $form['old_status'] = array(
    '#type' => 'hidden',
    '#default_value' => $old_status,
    '#attributes' => array(
      'class' => array(
        'new_life_cycle_status_old_status',
      ),
    ),
  );

  $unavailable_value = ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_UNAVAILABLE;
  $default_value     = '';
  $options           = archibald_get_life_cycle_status_options();

  foreach ($options as $k => $v) {
    if (!empty($_POST['current_value']) && $k == filter_xss($_POST['current_value'])) {
      $default_value = $k;
    }
    if ($k == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_UNAVAILABLE) {
      $unavailable_value = $k;
      $options[$k] = t('Proposal');
    }
    if ($k == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE) {
      $final_value = $k;
    }
  }
  if (drupal_strlen($default_value) && $default_value != $unavailable_value) {
    unset($options[$unavailable_value]);
  }
  if ($default_value == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_UNAVAILABLE || archibald_is_new_proposal($form_state['lom'])) {
    unset($options[$final_value]);
  }

  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Select status of description'),
    '#default_value' => $default_value,
    '#options' => $options,
    '#attributes' => array(
      'class' => array(
        'new_life_cycle_status_selector',
      ),
    ),
    '#required' => TRUE,
  );

  // For a new proposal, set the content partner
  // Hidden if only one cp exists, otherwise a DDM
  if (archibald_is_new_proposal( archibald_load($variables['lom_id']))) {
    $options = archibald_can_publish_for_content_partner();
    $values = array_keys($options);
    if ( count($options) == 1 ) {
      $form['content_partner'] = array(
        '#type' => 'hidden',
        '#value' => $values[0],
      );
    }
    else if(!empty($values)) {
      $form['content_partner'] = array(
        '#type' => 'select',
        '#title' => t('Content partner'),
        '#default_value' => $values[0],
        '#options' => $options,
        '#required' => TRUE,
      );
    }
    else {
      drupal_set_message(t('You are not allowed to work for any content partner.'), 'error');
      return array();
    }
  }

  $status2final = ($old_status != ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE && variable_get('archibald_send_mails_status2final', TRUE));
  $status2draft = ($old_status == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE && variable_get('archibald_send_mails_status2draft', TRUE));

  if ($status2final || $status2draft) {
    $form['mail'] = array(
      '#type' => 'fieldset',
      '#title' => t('Info E-Mail'),
      '#collapsible' => TRUE,
      '#attributes' => array(
        'style' => array('display:none;'),
        'class' => array('new_life_cycle_status_mail_block'),
      ),
    );

    $form['mail']['mailconfirm'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send info E-Mail.'),
      '#default_value' => 1,
    );

    $form['mail']['mailsubject'] = array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#default_value' => archibald_mail_text('status2draft_subject', NULL, $variables),
      '#attributes' => array(
        'class' => array(
          'new_life_cycle_status_mail_subject',
        ),
      ),
    );

    $form['mail']['mailtext'] = array(
      '#type' => 'textarea',
      '#title' => t('Text'),
      '#default_value' => archibald_mail_text('status2draft_body', NULL, $variables),
      '#row' => 10,
      '#cols' => 60,
      '#attributes' => array(
        'class' => array(
          'new_life_cycle_status_mail_body',
        ),
      ),
    );
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * status selector validate handler
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_set_status_form_validate($form, &$form_state) {
  archibald_content_set_content_partner_form_validate($form, $form_state);
}

/**
 * status selector submit handler
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_content_set_status_form_submit($form, &$form_state) {
  global $user;
  $form_state['lom_stats'] = archibald_get_lom_stats($form_state['values']['lom_id']);
  $lom_id         = $form_state['values']['lom_id'];
  $old_status     = $form_state['values']['old_status'];
  $new_status     = $form_state['values']['status'];
  $send_info_mail = ($form_state['values']['mailconfirm'] == 1) ? TRUE : FALSE;
  $new_status_parts = explode('|', $new_status);

  if ($old_status != $new_status_parts[1]) {
    if (!isset($form_state['values']['mailsubject'])) {
      $form_state['values']['mailsubject'] = '';
    }
    if (!isset($form_state['values']['mailtext'])) {
      $form_state['values']['mailtext'] = '';
    }

    $lom = archibald_load($lom_id);
    $form_state['new_status'] = archibald_set_status($lom, $old_status, $new_status, $send_info_mail, $form_state['values']['mailsubject'], $form_state['values']['mailtext']);
    $form_state['was_saved'] = 'TRUE';
  }


  archibald_content_set_content_partner_form_submit($form, $form_state);
}

/**
 * ajax menu callback for archibald content overview owner / content partner selector
 *
 * @param string $lom_id
 */
function archibald_content_set_content_partner($lom_id) {
  // Include the CTools tools that we need.
  ctools_include('ajax');
  ctools_include('modal');

  drupal_add_http_header('Content-Type', 'application/json');

  $status = archibald_get_lom_stats($lom_id);



  if (!empty($status->publication_version)) {

    if (!user_access('publish archibald content central')) {
      $output = t('This description is actually published in the national catalogue.') .
        '<br />' .
        t("You don't have the permission to publish/unpublish descriptions to the national catalogue.") .
        '<br />' .
        t('Please inform the publisher that this description needs to be unpublished.');
    }
    else {
      $output = t('This description is actually published in the national catalogue.') .
        '<br />' .
        t('Content partner cannot be changed, please unpublish first');
    }
    $commands = array();
    $commands[] = ctools_modal_command_display(t('Change content partner'), $output);

    drupal_add_http_header('Content-Type', 'application/json');
    print ajax_render($commands);
    die();
  }

  $form_state = array(
    'title' => t('Change content partner'),
    'ajax' => TRUE,
    'lom_id' => $lom_id,
    'lom_stats' => $status,
    'admin_content_overview_type' => filter_xss($_REQUEST['admin_content_overview_type']),
  );
  $commands = ctools_modal_form_wrapper('archibald_content_set_content_partner_form', $form_state);

  if (!empty($form_state['was_saved']) || (!empty($form_state['values']['op']) && $form_state['values']['op'] == $form_state['complete form']['cancel']['#value'])) {

    $commands = array(ctools_modal_command_dismiss());
    $commands[] = ajax_command_replace(
      '#archibald_admin_content_overview',
      archibald_admin_content_overview(filter_xss($_REQUEST['admin_content_overview_type']), TRUE)
    );
  }

  drupal_add_http_header('Content-Type', 'application/json');
  print ajax_render($commands);
}

/**
 * content_parnter selector
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_content_set_content_partner_form($form, &$form_state) {

  $variables = array('lom_id' => $form_state['lom_id']);

  $form['lom_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $form_state['lom_id'],
  );

  $form['admin_content_overview_type'] = array(
    '#type' => 'hidden',
    '#default_value' => $form_state['admin_content_overview_type'],
  );

  $form['content_partner'] = array(
    '#type' => 'select',
    '#title' => t('Content partner'),
    '#default_value' => $form_state['lom_stats']->content_partner_id,
    '#options' => archibald_can_publish_for_content_partner(),
    '#required' => TRUE,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * content_parnter selector validate handler
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_set_content_partner_form_validate($form, &$form_state) {
  global $user;
  if (!isset($form_state['values']['content_partner'])) {
    return;
  }
  $new_content_partner_id = $form_state['values']['content_partner'];

  if (!archibald_can_publish_for_content_partner($new_content_partner_id)) {
    form_set_error('content_partner', t('You have not the permission to set this content partner, choose a different one.'));
  }

  $lom_id = $form_state['values']['lom_id'];

  $lom = archibald_load($lom_id);
  $content_partner = archibald_load_content_partner($new_content_partner_id);

  $has_uploaded_files = FALSE;

  // General identifier.
  foreach ($lom->getGeneral()->getIdentifier() as $identifier) {
    if ($identifier->getCatalog() == 'URL') {
      preg_match('/archibald_file\/([0-9]+)\//', $identifier->getEntry(), $fid);

      if (!empty($fid[1])) {
        $has_uploaded_files = TRUE;
      }
    }
  }

  if (!empty($content_partner['username']) && $content_partner['file_server_type'] == 'no' && $has_uploaded_files == TRUE) {
    form_set_error(
      'content_partner',
      t('This description contains files which have to be uploaded, but you don’t have the rights to do so. Remove identifier files from description.')
    );
  }
}

/**
 * content_parnter selector submit handler
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_content_set_content_partner_form_submit($form, &$form_state) {
  global $user;
  if (!isset($form_state['values']['content_partner'])) {
    return;
  }
  $new_content_partner_id = $form_state['values']['content_partner'];
  $lom_id = $form_state['values']['lom_id'];

  $lom = archibald_load($lom_id);

  $content_partners = archibald_load_content_partner();

  db_update('archibald_lom_stats')
    ->fields(
      array(
        'content_partner_id' => $new_content_partner_id,
      )
    )
    ->condition('lom_id', $lom->getLomId())
    ->execute();

  $title = ArchibaldLom::determinTitle($lom);
  if (!isset($content_partners[$form_state['lom_stats']->content_partner_id])) {
    $content_partners[$form_state['lom_stats']->content_partner_id] = array(
      'name' => '(' . t('Deleted') . ')'
    );
  }
  watchdog(
    'Content',
    'Description "!title": content partner was changed @from -> @to',
    array(
      '!title' => $title,
      '@lom_id' => $lom->getLomId(),
      '@from' => $content_partners[$form_state['lom_stats']->content_partner_id]['name'],
      '@to' => $content_partners[$new_content_partner_id]['name'],
    ),
    WATCHDOG_NOTICE,
    'archibald/' . $lom->getLomId()
  );

  $form_state['was_saved'] = 'TRUE';
}

/**
 * error theme for set status ajax call
 *
 * @param array $variables
 *  lom ArchibaldLom Object
 *  errors array( list of error message strings)
 *
 * @return string html
 */
function theme_archibald_content_set_status_faulty($variables) {
  $html = '<b>';
  $html .= t('The status cannot be changed because the description is incomplete.');
  $html .= '</b><br /><br />';

  $table = array(
    'header' => array(),
    'rows' => array(),
  );

  foreach ($variables['errors'] as $error) {
    $table['rows'][] = array($error);
  }

  $html .= theme('table', $table);

  return $html;
}

/**
 * ajax menu callback for archibald content overview content responsible selector
 *
 * @param string $lom_id
 */
function archibald_content_set_responsible($lom_id) {
  $lom = archibald_load($lom_id);

  // Include the CTools tools that we need.
  ctools_include('ajax');
  ctools_include('modal');

  $form_state = array(
    'title' => t('Change person responsible'),
    'ajax' => TRUE,
    'lom' => $lom,
    'admin_content_overview_type' => filter_xss($_REQUEST['admin_content_overview_type']),
  );
  $commands = ctools_modal_form_wrapper('archibald_content_set_responsible_form', $form_state);

  if (!empty($form_state['was_saved']) || (!empty($form_state['values']['op']) && $form_state['values']['op'] == $form_state['complete form']['cancel']['#value'])) {
    $commands = array(ctools_modal_command_dismiss());
    $commands[] = ajax_command_replace(
      '#archibald_admin_content_overview',
      archibald_admin_content_overview(filter_xss($_REQUEST['admin_content_overview_type']), TRUE)
    );
  }

  drupal_add_http_header('Content-Type', 'application/json');
  print ajax_render($commands);
}

/**
 * new responsible/owner selector
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_content_set_responsible_form($form, &$form_state) {
  global $user;

  if ( !isset($form_state['admin_content_overview_type']) ) {
    $form_state['admin_content_overview_type'] = 'draft';
  }

  if ( !isset($form_state['lom']) ) {
    // Get LOM ID from second part in url path (archibald/%/set_status/form)
    $variables = array('lom_id' => arg(1));
  }
  else {
    $variables = array('lom_id' => $form_state['lom']->getLomId());
  }

  $form['lom_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $variables['lom_id'], //$form_state['lom']->getLomId(),
    '#attributes' => array('class' => array('set_responsible_lom_id')),
  );

  $form['admin_content_overview_type'] = array(
    '#type' => 'hidden',
    '#default_value' => $form_state['admin_content_overview_type'],
  );

  $options = archibald_get_all_user_editor(TRUE);
  unset($options[$user->uid]);
  /* woow here are really to mutch editors for a select box */
  if (count($options) < variable_get('archibald_max_editor_euser_for_select', 100)) {
    $form['new_responsible'] = array(
      '#type' => 'select',
      '#title' => t('New person responsible'),
      '#default_value' => reset(array_keys($options)),
      '#options' => $options,
      '#attributes' => array('class' => array('set_responsible_selector')),
      '#required' => TRUE,
    );
  }
  else {
    $form['new_responsible'] = array(
      '#type' => 'textfield',
      '#title' => t('New person responsible'),
      '#description' => t('Enter user name'),
      '#default_value' => '',
      '#autocomplete_path' => 'archibald/user_editor_autocomplete',
    );
  }
  if (variable_get('archibald_send_mails_set_responsible', TRUE)) {
    $form['mail'] = array(
      '#type' => 'fieldset',
      '#title' => t('Info E-Mail'),
      '#collapsible' => TRUE,
      '#attributes' => array('class' => array('set_responsible_mail_block')),
    );

    $form['mail']['mailconfirm'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send notification E-Mail to new person responsible'),
      '#default_value' => 1,
    );

    $form['mail']['mailsubject'] = array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#default_value' => archibald_mail_text('set_responsible_subject', NULL, $variables),
      '#attributes' => array('class' => array('set_responsible_mail_subject')),
    );

    $form['mail']['mailtext'] = array(
      '#type' => 'textarea',
      '#title' => t('Text'),
      '#default_value' => archibald_mail_text('set_responsible_body', NULL, $variables),
      '#row' => 10,
      '#cols' => 60,
      '#attributes' => array('class' => array('set_responsible_mail_body')),
    );
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
  );

  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * new responsible/owner selector
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_content_set_responsible_form_submit($form, &$form_state) {
  global $user;

  $new_responsible = $form_state['values']['new_responsible'];
  $send_info_mail = FALSE;
  if (variable_get('archibald_send_mails_set_responsible', TRUE)) {
    $send_info_mail = ($form_state['values']['mailconfirm'] == 1) ? TRUE : FALSE;
  }

  $lom_id = $form_state['values']['lom_id'];

  if (is_numeric($new_responsible)) {
    $account = user_load($new_responsible);
  }
  else {
    $account = user_load_by_name($new_responsible);
  }

  ArchibaldLomSaveHandler::setResponsibleUid($lom_id, $account->uid);

  $lom = archibald_load($lom_id);
  $title = ArchibaldLom::determinTitle($lom);
  watchdog(
    'content',
    'Description "!title": new person responsible is: @user.',
    array(
      '!title' => $title,
      '@lom_id' => $lom->getLomId(),
      '@user' => $account->name,
    ),
    WATCHDOG_NOTICE,
    'archibald/' . $lom->getLomId()
  );

  // editor pass responsibility to another editor:
  //   mail to new repsponsible editor
  if ($send_info_mail == TRUE) {
    archibald_send_mail($lom_id, 'set_responsible', $account->uid, $form_state['values']['mailsubject'], $form_state['values']['mailtext']);
  }

  $form_state['new_responsible'] = theme(
    'archibald_admin_content_overview_reponsible',
    array(
      'uid' => $account->uid,
      'name' => $account->name,
      'lom_id' => $lom->getLomId(),
    )
  );

  $form_state['was_saved'] = 'TRUE';
}

/**
 * display a vCard from lom object
 * menu callback
 *
 * @param string $lom_id
 * @param string $version_id
 * @param string $contributor_id
 * @param string $js nojs or ajax
 *
 * @return string html
 */
function archibald_vcard($lom_id, $version_id = '', $contributor_id = 0, $js = 'nojs') {
  $lom = archibald_load($lom_id, $version_id);
  $contribute = $lom->getLifeCycle()->getContribute($contributor_id);

  if (!($contribute instanceof ArchibaldLomDataContribute) || empty($contribute->entity[0])) {

    if ($js == 'ajax') {
      echo t('Error');
      die();
    }
    else {
      return t('Error');
    }
  }

  $parser = new ArchibaldAppDataVcard_Parser($contribute->entity[0]);
  $vcard  = $parser->parse();
  $vcard  = $vcard[0];

  $html = theme('archibald_choose_contributor_preview', array('vcard' => $vcard));

  if ($js == 'ajax') {
    print $html;
    die();
  }
  else {
    return $html;
  }
}

/**
 *
 * This is an ajax menu callback which informs user in case a resource with status ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE is edited.
 *
 * @param string $lom_id
 * @param string $is_published
 */
function archibald_content_edit_final($lom_id, $is_published) {

  // Include the CTools tools that we need.
  ctools_include('ajax');
  ctools_include('modal');

  $form_state = array(
    'title' => t('You are editing a description by the status final.'),
    'ajax' => TRUE,
    'lom_id' => $lom_id,
    'is_published' => $is_published
  );

  $commands = ctools_modal_form_wrapper('archibald_content_edit_final_form', $form_state);

  drupal_add_http_header('Content-Type', 'application/json');
  print ajax_render($commands);
}


/**
 * This function generates a simple form which informs user that resource status is set to draft
 * in case user edits the resource.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array a drupal form
 */
function archibald_content_edit_final_form($form, &$form_state) {

  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Change status'),
    '#collapsible' => FALSE,
  );

  $form['status']['hint'] = array(
    '#markup' => t('The status of this description will change back to draft as soon as you edit and save it.')
  );

  if ($form_state['is_published'] == 'published') {
    $form['publication'] = array(
      '#type' => 'fieldset',
      '#title' => t('Republish'),
      '#collapsible' => FALSE,
    );

    $form['publication']['hint'] = array(
      '#markup' => t('The description must be republished after editing.')
    );
  }

  $form['edit'] = array(
    '#markup' => '<button class="archibald_close_dialog">' . t('Proceed with editing') . '</button>'
  );

  return $form;
}