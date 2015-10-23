<?php

/**
 * @file
 * Provide a dispatcher that send emails on operation completion.
 */

namespace Drupal\maps_import\Log\Observer;

use Drupal\maps_suite\Log;

/**
 * MaPS Import Log Overver Mail class.
 *
 * This class allow to dispatch log messages through Drupal mail system.
 */
class Mail extends Log\Observer\Mail {

  /**
   * @inheritdoc
   */
  public function dispatchLog(Log\LogInterface $log) {
  	global $language;

  	$relatedTokens = $log->getRelatedTokens();

    $parameters = array(
      'subject' => variable_get('maps_import_log_subject', 'Log report'),
      'message' => variable_get('maps_import_log_message', 'The log report may be found there: [maps-log-url].'),
      'data' => array(
        'maps_import_log' => $log,
        'maps_import_profile' => $relatedTokens['maps_import_profile'],
      ),
    );

    if ($recipients = variable_get('maps_import_log_emails', '')) {
      drupal_mail('maps_import', 'log_dispatch', $recipients, $language, $parameters);
    }
  }

}
