<?php

/**
 * Implements hook_rules_action_info().
 */
function tmgmt_server_mailer_rules_action_info() {
  $info['tmgmt_server_mailer_rules_new_job_notify'] = array(
    'label' => t('Notify translator of a new job created at TMGMT Server'),
    'group' => t('Translation Management'),
    'parameter' => array(
      'tmgmt_job' => array(
        'type' => 'tmgmt_job',
        'label' => t('TMGMT Job'),
        'description' => t('The translation job just created.'),
      ),
      'tmgmt_server_client' => array(
        'type' => 'tmgmt_server_client',
        'label' => t('TMGMT Client'),
        'description' => t('The client that has requested the job.'),
      ),
    ),
  );

  return $info;
}

/**
 * Rules action callback to send new job notification.
 *
 * @param TMGMTJob $job
 *   New job to send notification about.
 * @param TMGMTClient $client
 *   Client that has created the job.
 */
function tmgmt_server_mailer_rules_new_job_notify(TMGMTJob $job, TMGMTClient $client) {
  drupal_mail('tmgmt_server_mailer', 'tmgmt_server_mailer_rules_new_job_notify',
    tmgmt_server_mailer_notification_email($job->getTranslator()), $job->source_language, array('job' => $job));
}
