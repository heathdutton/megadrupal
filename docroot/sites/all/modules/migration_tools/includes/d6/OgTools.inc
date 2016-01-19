<?php
/**
 * @file
 * D6 Organic Groups tools.
 */

/**
 * OgTools.
 */
class OgTools {

  /**
   * Get groups.
   */
  public static function getGroups($nid) {
    $db_table = "og_ancestry";
    $child = "nid";
    $parent = "group_nid";

    $query = \Database::getConnection('default', 'legacy')->select($db_table, 'oga');
    $query->fields("oga", array($child, $parent));
    $query->condition("oga.{$child}", $nid, "=");

    $results = $query->execute()->fetchAll();
    $counter = 0;
    $groups = array();
    foreach ($results as $result) {
      $parent_gid = $result->{$parent};
      $groups[] = $parent_gid;
    }
    return $groups;
  }
}
