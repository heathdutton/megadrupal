<?php
/**
 * @file
 * Mailchimp campaign stats.
 */

/**
 * Function to generate table of stats for a campaign.
 *
 * @return string
 *   html table or redirect to all campaigns for list.
 */
function mailchimp_user_list_campaign_stats($lid, $cid) {
  $q = mailchimp_get_api_object();
  $results = $q->campaignStats($cid);
  if ($q->errorCode) {
    drupal_set_message(t('There was a problem retrieving the stats for this campaign.'));
    watchdog('mailchimp',
      'Could not retrieve stats for campaign @campaign_name ID number @campaign_id, error @error',
      array(
        '@campaign_name' => mailchimp_user_list_campaign_title($cid),
        '@campaign_id' => $cid,
        '@error' => $q->errorMessage,
      ),
      WATCHDOG_ERROR
    );
  }
  else {
    if ($results) {
      $rows = array();
      foreach ($results as $key => $value) {
        $rows[] = array($key, $value);
      }
      $output = array(
        'header' => array(t('Name'), t('Value')),
        'rows' => $rows,
      );
      $output = theme('table', $output);
      return $output;
    }
    else {
      mailchimp_user_list_return_to_list($lid);
    }
  }
}
