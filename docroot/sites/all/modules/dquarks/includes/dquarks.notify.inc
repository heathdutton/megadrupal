<?php

/**
 * @file
 * @category Advertizing.
 * @package dquarks
 * @subpackage QuizzNotification
 * @name QuizzNotification
 *
 * @author IP-TECH
 */

/**
 * Implements hook_form().
 *
 * @param array $form
 *   An array of form item of notification parameter.
 * @param array $form_state
 *   An array of form item values of notification parameter.
 * @param object $node
 *   The node id.
 */
function dquarks_notify_form($form, &$form_state, $node) {
  $form = array();
  $form = array(
    '#tree' => TRUE,
    '#node' => $node,
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $form['Notification'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Notification process settings'),
    '#description' => t('You can customize default notifications settings.'),
    '#node' => $node,
  );
  $form['Notification']['quiz_notify_message_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Message subject of notification mail'),
    '#default_value' => (!empty($node->dquarks['quiz_notify_message_subject'])) ? $node->dquarks['quiz_notify_message_subject'] : variable_get('quiz_notify_message_subject', t('Survey notification from:') . ' ' . variable_get('site_mail', ini_get('sendmail_from'))),
  );

  $form['Notification']['quiz_notify_message_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Notification message content'),
    '#description' => theme('dquarks_token_help'),
    '#default_value' => (!empty($node->dquarks['quiz_notify_message_content'])) ? $node->dquarks['quiz_notify_message_content'] : variable_get('quiz_notify_message_content', t('Quiz notification message from %site. This quiz is available on %url.')),
    '#cols' => 40,
    '#rows' => 10,
    '#wysiwyg' => TRUE,
  );

  $form['Notification']['quiz_notify_mails_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Thank you specify the list of persons to be notified and the details of the message.'),
    '#description' => t("The list of emails must be separated by ';'.malformed mails address will be ignored."),
    '#default_value' => (!empty($node->dquarks['quiz_notify_mails_list'])) ? $node->dquarks['quiz_notify_mails_list'] : variable_get('quiz_notify_mails_list', t('')),
    '#cols' => 40,
    '#rows' => 10,
  );

  $form['Notification']['excel'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Import mails list from Microsoft Excel file.'),
    '#node' => $node,
  );

  $form['Notification']['excel']['dquarks_xls_mails_colone'] = array(
    '#type' => 'textfield',
    '#title' => t('Find mails on colone number'),
    '#default_value' => variable_get('dquarks_xls_mails_colone', t('1')),
  );

  $form['Notification']['excel']['dquarks_xls_mails_ligne'] = array(
    '#type' => 'textfield',
    '#title' => t('Start from ligne number'),
    '#default_value' => variable_get('dquarks_xls_mails_ligne', t('1')),
  );

  $form['Notification']['excel']['xls_file'] = array(
    '#type' => 'file',
    '#title' => t('Import from XLS FILE'),
    '#description' => t('Import mails list from Microsoft Excel file.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Notify'),
    '#weight' => 1,
  );
  $form['#attributes'] = array('enctype' => "multipart/form-data");

  return $form;
}

/**
 * Implements hook_submit().
 *
 * @param array $form
 *   The current form of notification parameter.
 * @param array $form_state
 *   The current form values of notification parameter.
 */
function dquarks_notify_form_submit($form, &$form_state) {
  global $user;
  $node = node_load($form_state['values']['nid']);
  $validators = array(
    'file_validate_extensions' => array('xlsx xls'),
  );
  $file_handle = file_save_upload('Notification', $validators, "public://", FILE_EXISTS_RENAME);
  $file_directory = variable_get('file_directory_path', conf_path() . '/files');
  $mails = explode(";", $form_state['values']['Notification']['quiz_notify_mails_list']);
  foreach ($mails as $key => $mail) {
    if (!valid_email_address($mail)) {
      unset($mails[$key]);
    }
  }
  if ($file_handle) {
    include libraries_get_path('PHPExcel') . '/PHPExcel.php';
    require_once libraries_get_path('PHPExcel') . '/PHPExcel/IOFactory.php';
    include libraries_get_path('PHPExcel') . '/PHPExcel/Reader/Excel2007.php';
    // Open Excel File.
    $fichier_xls_reader = new PHPExcel_Reader_Excel2007();
    $filename = drupal_substr($file_handle->destination, 9);
    $filename = $file_directory . "/" . $filename;
    $fichier_excel = $fichier_xls_reader->load($filename);
    $colone = (int) $form_state['values']['Notification']['excel']['dquarks_xls_mails_colone'];
    $row = (int) $form_state['values']['Notification']['excel']['dquarks_xls_mails_ligne'];
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
  $values = array();
  $values['quiz_notify_message_subject'] = $form_state['values']['Notification']['quiz_notify_message_subject'];
  $values['quiz_notify_message_content'] = $form_state['values']['Notification']['quiz_notify_message_content'];
  $values['quiz_notify_mails_list'] = (is_array($mails)) ? implode(';', $mails) : $mails;
  dquarks_notify_update($node->nid, $values);
  if (!empty($mails)) {
    foreach ($mails as $mail) {
      if (!valid_email_address($mail)) {
        continue;
      }
      $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';
      $message['headers']['X-Mailer'] = 'Drupal dquarks (PHP/' . phpversion() . ')';
      $message['subject'] = $values['quiz_notify_message_subject'];
      $message['body'] = _dquarks_filter_values(t($values['quiz_notify_message_content']), $node);
      drupal_mail('dquarks', 'submission', $mail, user_preferred_language($user), $message, dquarks_variable_get('dquarks_default_from_address'), TRUE);
    }
    drupal_set_message(t('@number Mails was sent.', array('@number' => count($mails))));
  }
}

/**
 * Update quizz configuration settings.
 *
 * @param int $nid
 *   The node id.
 * @param array $values
 *   The content of the message to send.
 */
function dquarks_notify_update($nid, &$values) {
  db_update('dquarks')
          ->fields(array(
            'quiz_notify_message_subject' => $values['quiz_notify_message_subject'],
            'quiz_notify_message_content' => $values['quiz_notify_message_content'],
            'quiz_notify_mails_list' => $values['quiz_notify_mails_list'],
          ))
          ->condition('nid', $nid)
          ->execute();
}
