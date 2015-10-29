<?php
/**
 * @file
 * Contains the ProjectIssueAlterFollowers class.
 */

/**
 * Add field for node and comment authors.
 */
class ProjectIssueAlterFollowers extends SearchApiAbstractAlterCallback {
  /**
   * Check whether this data-alter callback is applicable for a certain index.
   *
   * Returns TRUE only for indexes on nodes.
   *
   * @param SearchApiIndex $index
   *   The index to check for.
   *
   * @return boolean
   *   TRUE if the callback can run on the given index; FALSE otherwise.
   */
  public function supportsIndex(SearchApiIndex $index) {
    // Currently only node access is supported.
    return $index->getEntityType() === 'node';
  }

  /**
   * Declare the properties that are (or can be) added to items with this callback.
   *
   * @see hook_entity_property_info()
   *
   * @return array
   *   Information about all additional properties, as specified by
   *   hook_entity_property_info() (only the inner "properties" array).
   */
  public function propertyInfo() {
    return array(
      'project_issue_followers' => array(
        'label' => t('Followers'),
        'description' => t('Issue followers.'),
        'type' => 'list<user>',
      ),
    );
  }

  /**
   * Alter items before indexing. Adds the node and comment authors.
   *
   * @param array $items
   *   An array of items to be altered, keyed by item IDs.
   */
  public function alterItems(array &$items) {
    foreach ($items as $nid => $item) {
      $follow_flag = flag_tracker_get_tracker_flag($item->type);
      if (!empty($follow_flag)) {
        $items[$nid]->project_issue_followers = array_keys(flag_get_content_flags('node', $nid, $follow_flag));
      }
    }
  }
}
