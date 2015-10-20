<?php

/**
 * @file
 * Administration pages provided by dquarks module.
 */

/**
 * Menu callback for admin/config/content/dquarks.
 */
function dquarks_admin_settings() {
  require_once  drupal_get_path('module', 'dquarks') . '/includes/dquarks.export.inc';

  // Add each component to the form:
  $form['components'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available components'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('These are the available field types for your installation of quiz. You may disable any of these components by unchecking its corresponding box. Only checked components will be available in existing or new quiz.'),
    '#tree' => TRUE,
  );

  $component_types = dquarks_components(TRUE);
  foreach ($component_types as $key => $component) {
    $form['components'][$key] = array(
      '#title' => $component['label'],
      '#description' => $component['description'],
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => $component['enabled'],
    );
  }

  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default e-mail values sended to administrator after submission of quiz'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['email']['dquarks_default_from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From address'),
    '#default_value' => variable_get('dquarks_default_from_address', variable_get('site_mail', ini_get('sendmail_from'))),
    '#description' => t('The default sender address for emailed dquarks results; often the e-mail address of the maintainer of your forms.'),
  );

  $form['email']['dquarks_default_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#default_value' => variable_get('dquarks_default_from_name', variable_get('site_name', '')),
    '#description' => t('The default sender name which is used along with the default from address.'),
  );

  $form['email']['dquarks_default_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Default subject'),
    '#default_value' => variable_get('dquarks_default_subject', t('Form submission from: %title')),
    '#description' => t('The default subject line of any e-mailed results.'),
  );

  $form['email']['dquarks_default_format'] = array(
    '#type' => 'radios',
    '#title' => t('Format'),
    '#options' => array(
      0 => t('Plain text'),
      1 => t('HTML'),
    ),
    '#default_value' => variable_get('dquarks_default_format', 0),
    '#description' => t('The default format for new e-mail settings. dquarks e-mail options take precedence over the settings for system-wide e-mails configured in MIME mail.'),
    '#access' => dquarks_email_html_capable(),
  );

  $form['email']['dquarks_format_override'] = array(
    '#type' => 'radios',
    '#title' => t('Format override'),
    '#options' => array(
      0 => t('Per-dquarks configuration of e-mail format'),
      1 => t('Send all e-mails in the default format'),
    ),
    '#default_value' => variable_get('dquarks_format_override', 0),
    '#description' => t('Force all dquarks e-mails to be sent in the default format.'),
    '#access' => dquarks_email_html_capable(),
  );
  $form['Notification'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Notification process settings'),
    '#description' => t('You can customize default notifications settings.'),
  );
  $form['Notification']['quiz_notify_message_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Default message subject of notification mail'),
    '#default_value' => variable_get('quiz_notify_message_subject', t('Survey notification from:') . ' ' . variable_get('site_mail', ini_get('sendmail_from'))),
  );

  $form['Notification']['quiz_notify_message_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Default notification message content'),
    '#description' => theme('dquarks_token_help'),
    '#default_value' => variable_get('quiz_notify_message_content', t('Quiz notification message from %site. This quiz is available on %url.')),
    '#cols' => 40,
    '#rows' => 10,
    '#wysiwyg' => TRUE,
  );
  $form['Notification']['quiz_notify_mails_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Thank you specify the list of persons to be notified and the details of the message.'),
    '#description' => t("The list of emails must be separated by ';'.malformed mails address will be ignored."),
    '#default_value' => variable_get('quiz_notify_mails_list'),
    '#cols' => 40,
    '#rows' => 10,
  );
  $form['Notification']['dquarks_xls_mails_colone'] = array(
    '#type' => 'textfield',
    '#title' => t('Find mails on colone number'),
    '#default_value' => variable_get('dquarks_xls_mails_colone', 0),
  );
  $form['Notification']['dquarks_xls_mails_ligne'] = array(
    '#type' => 'textfield',
    '#title' => t('Find mails on ligne number'),
    '#default_value' => variable_get('dquarks_xls_mails_ligne', 1),
  );

  $form['Notification']['xls_file'] = array(
    '#type' => 'file',
    '#title' => t('Import from XLS FILE'),
    '#description' => t('Import mails list from Microsoft Excel file.'),
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['dquarks_use_cookies'] = array(
    '#type' => 'checkbox',
    '#checked_value' => 1,
    '#title' => t('Allow cookies for tracking submissions'),
    '#default_value' => variable_get('dquarks_use_cookies', 0),
    '#description' => t('<a href="http://www.wikipedia.org/wiki/HTTP_cookie">Cookies</a> can be used to help prevent the same user from repeatedly submitting a dquarks. This feature is not needed for limiting submissions per user, though it can increase accuracy in some situations. Besides cookies, dquarks also uses IP addresses and site usernames to prevent repeated submissions.'),
  );

  $form['advanced']['dquarks_submission_access_control'] = array(
    '#type' => 'radios',
    '#title' => t('Submission access control'),
    '#options' => array(
      '1' => t('Select the user roles that may submit each individual dquarks'),
      '0' => t('Disable dquarks submission access control'),
    ),
    '#default_value' => variable_get('dquarks_submission_access_control', 1),
    '#description' => t('By default, the configuration form for each dquarks allows the administrator to choose which roles may submit the form. You may want to allow users to always submit the form if you are using a separate node access module to control access to dquarks nodes themselves.'),
  );
  $form['advanced']['dquarks_debug'] = array(
    '#type' => 'select',
    '#title' => t('Quizs debug'),
    '#default_value' => variable_get('dquarks_debug', 0),
    '#options' => array(
      0 => t('Off'),
      1 => t('Log submissions'),
      2 => t('Full debug'),
    ),
    '#description' => t('Set to "Log submissions" to log all submissions in the watchdog. Set to "Full debug" to print debug info on submission.'),
  );
  $form['mail'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Component configuration of email notification'),
  );

  $form['mail']['dquarksz_submited_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Submission date'),
    '#description' => t('Available values are: @date'),
    '#default_value' => variable_get("dquarksz_submited_date", 'Submitted on @date'),
  );
  $form['mail']['dquarksz_submited_authentified_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Information about the user who submitted the quiz (logged user)'),
    '#description' => t('Available values are: @username et @ip_address'),
    '#default_value' => variable_get("dquarksz_submited_authentified_user", 'Submitted by user: @username [@ip_address]'),
  );
  $form['mail']['dquarksz_submited_anony_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Information about the user who submitted the quiz (Anonymous user)'),
    '#description' => t('Available values are: @ip_address'),
    '#default_value' => variable_get("dquarksz_submited_anony_user", 'Submitted by anonymous user: [@ip_address]'),
  );
  $form['mail']['dquarksz_mail_header'] = array(
    '#type' => 'textfield',
    '#title' => t('Header message  of the mail'),
    '#default_value' => variable_get("dquarksz_mail_header", 'Submitted values are'),
  );
  $form['mail']['dquarksz_score'] = array(
    '#type' => 'textfield',
    '#title' => t('Message displayed before the score'),
    '#default_value' => variable_get("dquarksz_score", 'Your score : !score points'),
    '#description' => t('Available values are: :!score'),
  );
  $form['mail']['dquarksz_mail_result'] = array(
    '#type' => 'textfield',
    '#title' => t('Message of visualization of the result'),
    '#default_value' => variable_get("dquarksz_mail_result", t('The results of this submission may be viewed at:')),
  );

  $form = system_settings_form($form);
  $form['#theme'] = 'dquarks_admin_settings';
  array_unshift($form['#submit'], 'dquarks_admin_settings_submit');

  return $form;
}

/**
 * Submit handler for the dquarks_admin_settings() form.
 */
function dquarks_admin_settings_submit($form, &$form_state) {

  $validators = array(
    'file_validate_extensions' => array('xls xlsx'),
  );
  $file_directory = variable_get('file_directory_path', conf_path() . '/files');
  $file_handle = file_save_upload('xls_file', $validators, "public://", FILE_EXISTS_RENAME);

  $mails = explode(";", $form_state['values']['quiz_notify_mails_list']);
  foreach ($mails as $key => $mail) {
    if (!valid_email_address($mail)) {
      unset($mails[$key]);
    }
  }
  if ($file_handle) {
    include libraries_get_path('PHPExcel') . '/PHPExcel.php';
    require_once libraries_get_path('PHPExcel') . '/PHPExcel/IOFactory.php';
    include libraries_get_path('PHPExcel') . '/PHPExcel/Reader/Excel2007.php';

    $fichier_xls_reader = new PHPExcel_Reader_Excel2007();

    $filename = drupal_substr($file_handle->destination, 9);
    $filename = $file_directory . "/" . $filename;
    $fichier_excel = $fichier_xls_reader->load($filename);

    $colone = (int) variable_get('dquarks_xls_mails_colone', 1);

    $row = (int) variable_get('dquarks_xls_mails_ligne', 1);
    if ($colone > 0) {
      $colone--;
    }
    else {
      $colone = 0;
    }

    while ($fichier_excel->getActiveSheet()->cellExistsByColumnAndRow($colone, $row)) {
      $mail = $fichier_excel->getActiveSheet()->getCellByColumnAndRow($colone, $row)->getValue();
      $row++;
      if (!valid_email_address($mail)) {
        continue;
      }
      $mails[] = $mail;
    }
  }
  variable_set('quiz_notify_mails_list', implode(";", $mails));

  $disabled_components = array();
  foreach ($form_state['values']['components'] as $name => $enabled) {
    if (!$enabled) {
      $disabled_components[] = $name;
    }
  }
  // Update $form_state and let system_settings_form_submit() handle saving.
  $form_state['values']['dquarks_disabled_components'] = $disabled_components;
  unset($form_state['values']['components']);

  // Change the name of the node type variable and clean it up.
  unset($form_state['values']['node_types']);
}

/**
 * Theme the output of the dquarks_admin_settings() form.
 */
function theme_dquarks_admin_settings($variables) {
  $form = $variables['form'];

  // Format the components into a table.
  foreach (element_children($form['components']) as $key) {
    $row = array();
    $row[] = $form['components'][$key]['#title'];
    $row[] = $form['components'][$key]['#description'];
    $form['components'][$key]['#title'] = NULL;
    $form['components'][$key]['#description'] = NULL;
    $row[] = array('data' => drupal_render($form['components'][$key]), 'align' => 'center');
    $rows[] = $row;
  }
  $header = array(t('Name'), t('Description'), array(
      'data' => t('Enabled'),
      'class' => array('checkbox',
      ),
    ),
  );

  // Create the table inside the form.
  $form['components']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return drupal_render_children($form);
}

/**
 * Menu callback for admin/content/dquarks. Displays all dquarkss on the site.
 */
function dquarks_admin_content() {

  $nodes = array();
  $nodes = db_select('node', 'n')
  ->fields('n')
  ->condition('n.type', 'dquarks')
  ->execute()
  ->fetchAllAssoc('nid');

  return theme('dquarks_admin_content', array('nodes' => $nodes));
}

/**
 * Create a comma-separate list of content types that are dquarks enabled.
 */
function dquarks_admin_type_list() {
  $dquarks_types = dquarks_variable_get('dquarks_node_types');
  $dquarks_type_list = '';
  $dquarks_type_count = count($dquarks_types);
  foreach ($dquarks_types as $n => $type) {
    $dquarks_type_list .= l(node_type_get_name($type), 'node/add/' . $type);
    if ($n + 1 < $dquarks_type_count) {
      $dquarks_type_list .= $dquarks_type_count == 2 ? ' ' : ', ';
    }
    if ($n + 2 == $dquarks_type_count) {
      $dquarks_type_list .= t('or') . ' ';
    }
  }

  return $dquarks_type_list;
}

/**
 * Generate a list of all dquarkss avaliable on this site.
 */
function theme_dquarks_admin_content($variables) {
  $nodes = $variables['nodes'];
  $header = array(
    t('Title'),
    array('data' => t('View'), 'colspan' => '4'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );

  $rows = array();
  foreach ($nodes as $node) {
    $rows[] = array(
      l($node->title, 'node/' . $node->nid),
      l(t('Submissions'), 'node/' . $node->nid . '/dquarks-results'),
      l(t('Analysis'), 'node/' . $node->nid . '/dquarks-results/analysis'),
      l(t('Table'), 'node/' . $node->nid . '/dquarks-results/table'),
      l(t('Download'), 'node/' . $node->nid . '/dquarks-results/download'),
      node_access('update', $node) ? l(t('Edit'), 'node/' . $node->nid . '/edit') : '',
      user_access('delete all dquarks submissions') ? l(t('Clear'), 'node/' . $node->nid . '/dquarks-results/clear') : '',
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
