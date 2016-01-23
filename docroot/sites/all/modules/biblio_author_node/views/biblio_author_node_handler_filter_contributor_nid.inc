<?php

/**
 * @file
 * Custom handler.
 */

/**
 * Filter handler for contributors linked via drupal_nid.
 */
class biblio_author_node_handler_filter_contributor_nid extends views_handler_filter_many_to_one {

  /**
   * Custom filter for drupal_nid field.
   */
  function get_value_options() {
    $result = db_query("SELECT n.title, lastname, firstname, initials, cid, drupal_uid, drupal_nid
              FROM {biblio_contributor_data} cd
              INNER JOIN {node} n on n.nid = cd.drupal_nid
              WHERE cd.drupal_nid > 0
              ORDER by lastname, firstname");
    $this->value_options = array();
    foreach ($result as $row) {
      $this->value_options[$row->drupal_nid] = "$row->lastname, $row->firstname $row->initials ($row->title)";
    }
  }

}
