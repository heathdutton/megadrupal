<?php
/**
 * @file
 * Contains the ProjectIssueAlterParticipants class.
 */

/**
 * Add field for node and comment authors.
 */
class ProjectIssueAlterParticipants extends SearchApiAbstractAlterCallback {
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
      'project_issue_participants' => array(
        'label' => t('Participants'),
        'description' => t('Node and comment authors.'),
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
      $items[$nid]->project_issue_participants = array_unique(array_merge(
        array($item->uid),
        db_query('SELECT DISTINCT uid FROM {comment} WHERE nid = :nid', array(':nid' => $nid))->fetchCol()
      ));
    }
  }
}
