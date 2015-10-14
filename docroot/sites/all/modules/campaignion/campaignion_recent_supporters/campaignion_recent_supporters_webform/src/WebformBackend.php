<?php

namespace Drupal\campaignion_recent_supporters_webform;

use \Drupal\campaignion_recent_supporters\BackendBase;
use \Drupal\campaignion_recent_supporters\RequestParams;

/**
 * Loader for recent supporters.
 *
 * ATTENTION: This class will be used from inside a minimal bootstrap.
 * @see poll.php
 */
class WebformBackend extends BackendBase {
  public static function label() {
    return t('Webform submissions');
  }

  public function recentOnOneAction(RequestParams $params) {
      $config = $params->getParams();
    $sql = <<<SQL
SELECT n.nid, n.tnid, w.first_name, w.last_name, w.timestamp, w.country
FROM {campaignion_recent_supporters_webform} w
  INNER JOIN {node} n USING(nid)
WHERE n.status = 1
    AND (n.nid = :nid OR n.nid IN (SELECT tn.nid FROM {node} tn INNER JOIN {node} n USING(tnid) WHERE n.nid = :nid AND n.tnid != 0))
ORDER BY w.timestamp DESC
SQL;
    $result = db_query_range($sql, 0, $config['limit'], array(':nid' => $config['nid']));

    return $this->buildSupporterList($result, $config['name_display']);
  }

  public function recentOnAllActions(RequestParams $params) {
    $config = $params->getParams();
    // get translations: na - activity node, nt - any available node translated into $lang, no - the "original" node (ie. the translation source)
    $sql = <<<SQL
SELECT w.first_name, w.last_name, w.timestamp, w.country,
  na.nid, na.tnid, na.type AS action_type, na.status,
  COALESCE(nt.title, no.title, na.title) AS action_title,
  COALESCE(nt.tnid, no.tnid, na.nid) AS action_nid,
  COALESCE(nt.language, no.language, na.language) AS action_lang
FROM {campaignion_recent_supporters_webform} w
  INNER JOIN {node} na ON w.nid = na.nid
  LEFT OUTER JOIN {node} nt ON na.tnid != 0 AND nt.tnid = na.tnid AND nt.language = :lang AND nt.status>0
  LEFT OUTER JOIN {node} no ON na.tnid = no.nid AND no.status>0
WHERE na.status = 1 AND na.type IN (:types)
  ORDER BY w.timestamp DESC
SQL;
    $result = db_query_range($sql, 0, $config['limit'], array(':types' => $config['types'], ':lang' => $config['lang']));
    $rows = array();
    return $this->buildSupporterList($result, $config['name_display']);
  }
}
