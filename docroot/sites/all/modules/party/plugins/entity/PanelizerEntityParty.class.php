<?php
/**
 * @file
 * Class for the Panelizer commerce_booking_ticket entity plugin.
 */

/**
 * Panelizer Entity commerce_booking_ticket plugin class.
 *
 * Handles commerce_booking_ticket specific functionality for Panelizer.
 */
class PanelizerEntityParty extends PanelizerEntityDefault {
  /**
   * True if the entity supports revisions.
   */
  public $supports_revisions = FALSE;
  public $entity_admin_root = 'admin/community/party';
  // No bundle support so we hardcode the default bundle.
  public $entity_admin_bundle = 'party';
  public $views_table = 'party';
  public $uses_page_manager = FALSE;

  /**
   * Determine if the entity allows revisions.
   */
  public function entity_allows_revisions($entity) {
    $retval[0] = $this->supports_revisions;
    $retval[1] = user_access('administer crm settings');

    return $retval;
  }

  /**
   * Implements PanelizerEntityDefault::entity_access().
   */
  public function entity_access($op, $entity) {
    return entity_access($op, 'party', $entity);
  }

  /**
   * Implements PanelizerEntityDefault::entity_save().
   */
  public function entity_save($entity) {
    return entity_save('party', $entity);
  }

  /**
   * Overrides PanelizerEntityDefault::preprocess_panelizer_view_mode().
   */
  public function preprocess_panelizer_view_mode(&$vars, $entity, $element, $panelizer, $info) {
    $panelizer->link_to_entity = FALSE;

    parent::preprocess_panelizer_view_mode($vars, $entity, $element, $panelizer, $info);
  }

  /**
   * Overrides PanelizerEntityDefault::hook_entity_insert().
   *
   * Do nothing!
   */
  public function hook_entity_insert($entity) {}

  /**
   * Overrides PanelizerEntityDefault::hook_entity_insert().
   *
   * Do nothing!
   */
  public function hook_entity_update($entity) {}

}
