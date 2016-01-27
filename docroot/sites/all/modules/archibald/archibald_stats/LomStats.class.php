<?php

/**
 * @file
 * get view statistic from the central catalog
 * how ofter was my published resouce on all portals view`d
 */

/**
 * Require abstract parent class
 */
module_load_include('php', 'archibald', 'includes/dsb_api.class');

/**
 * This class get stats from archibald from lom objetecs
 *
 * @author Heiko Henning <h.henning@educa.ch>
 */
class ArchibaldLomStats extends ArchibaldAbstractDsbApi {

  public function cron() {}

  /**
   * Get stats of resource views
   *
   * @param string $date_from
   *   sql data YYYY-MM-DD, optional
   * @param string $date_to
   *   sql data YYYY-MM-DD, optional
   * @param string $aggregate
   *   d=day,
   *   w=week,
   *   m=month
   *   y=year
   *
   * @return array
   *   list of results
   *   array([] =>
   *     string  lom_id,
   *     int     new_quantity,
   *     string  partner_id, # if matching parameter was empty partner_id
   *     int     year,
   *     int     month,  # when  $concentrate==m or $concentrate==d
   *     int     week,   # when  $concentrate==w
   *     int     day,    # when  $concentrate==d  day of month
   */
  public function statsResources($date_from   = 0, $date_to    = 0, $aggregate = 'month', $content_partner_id = "") {

    if (!empty($content_partner_id)) {
      $this->loadContentPartnerByContentPartnerId($content_partner_id);
    }

    if ($this->lomApiAuth($auth_token) != TRUE) {
      watchdog('stats resources', 'stats resources error: unable to auth at api, check access data', array(), WATCHDOG_ERROR);
      return FALSE;
    }

    if (is_numeric($date_from) && $date_from > 900000000) {
      // Is a unix timstamp > 1998.
      $date_from = strftime('%Y-%m-%d', $date_from);
    }
    elseif (empty($date_from)) {
      $date_from = strftime('%Y-%m-%d', time() - (6400 * 24 * 31 * 2));
    }

    if (is_numeric($date_to) && $date_to > 900000000) {
      // Is a unix timstamp > 1998.
      $date_to = strftime('%Y-%m-%d', $date_to);
    }
    elseif (empty($date_from)) {
      $date_to = strftime('%Y-%m-%d', time());
    }

    $headers = array(
      'X-TOKEN-KEY' => $auth_token,
    );

    $url = variable_get('archibald_api_url', 'https://dsb-api.educa.ch/v2') . '/stats/' . $this->contentPartner->username . '/' . $date_from . '/' . $date_to . '/' . $aggregate;
    // print_r( $url );
    // die();

    list($raw_result, $info) = $this->openUrl(
      $url,
      array(),
      'GET',
      $headers
    );

    $result = json_decode($raw_result);
    if (empty($result)) {
      watchdog('stats resources', 'Stats resources error: cannot parse result: !raw_result', array(
        '!raw_result' => $raw_result,
      ), WATCHDOG_ERROR);
      return FALSE;
    }

    if ($info->status != 200) {
      watchdog('stats resources', 'Stats resources error: @code (!message)', array(
        '@code' => $info->status,
        '!message' => $raw_result
      ), WATCHDOG_ERROR);
      return FALSE;
    }
    else {
      return $result;
    }
  }
}