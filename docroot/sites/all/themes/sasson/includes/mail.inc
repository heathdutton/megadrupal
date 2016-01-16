<?php

/**
 * @file
 * Tools for HTML mail messages development.
 */


/**
 * Implements hook_mail_alter()
 * Send mails to a file or log to database for easy debugging
 */
function sasson_mail_alter(&$message) {
  
  if (theme_get_setting('sasson_mail_to_file') || theme_get_setting('sasson_mail_to_log')) {
  
    $debug_dir = file_directory_temp() . '/mail_debug';
    $debug_file = $debug_dir . '/mail_debug.html';

    if (!is_dir($debug_dir)) {
      if (!mkdir($debug_dir)) {
        drupal_set_message(t('Mail message directory could not be created. Please consult your !watchdog for a detailed error description.', array('!watchdog' => l('log messages', 'admin/reports/dblog'))), 'error');
      }
    } 

    $output = '<!DOCTYPE html><meta charset="utf-8">';
    $output .= '<p>';
    $output .= '</br><strong>From:</strong> ' . $message['from'];
    $output .= '</br><strong>To:</strong> ' . $message['to'];
    $output .= '</br><strong>Subject:</strong> ' . $message['subject'];
    $output .= '</p>';

    if (is_array($message['body'])) {
      foreach ($message['body'] as $item) {
        $output .= $item;
      }
    } else {
      $output .= $message['body'];
    }
  }

  // Write debug information to a file
  if (theme_get_setting('sasson_mail_to_file')) {
    if (!file_put_contents($debug_file, $output)) {
      drupal_set_message(t('Mail message could not be logged. Please consult your !watchdog for a detailed error description.', array('!watchdog' => l('log messages', 'admin/reports/dblog'))), 'error');
    } else {
      drupal_set_message(t('Mail message was logged to !debug_file', array('!debug_file' => $_SERVER['DOCUMENT_ROOT'] . $debug_file)), 'status');
    }
  }

  // Write debug message to log
  if (theme_get_setting('sasson_mail_to_log')) {
    watchdog('mail_debugger', $output, NULL, WATCHDOG_DEBUG, NULL);
    drupal_set_message(t('Mail message was logged. Check out your !watchdog.', array('!watchdog' => l('log messages', 'admin/reports/dblog'))), 'status');
  }
  
  // display devel message
  if (function_exists('kpr') && theme_get_setting('sasson_mail_devel')) {
    kpr($message);
  }

  // prevent drupal_mail() from calling drupal_mail_system()->mail() and 
  // delivering the message.
  if (theme_get_setting('sasson_mail_prevent')) {
    $message['send'] = FALSE;
  }
}
