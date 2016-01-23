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
class PanelizerEntityCommerceBookingTicket extends PanelizerEntityDefault {
  /**
   * True if the entity supports revisions.
   */
  public $supports_revisions = FALSE;
  public $entity_admin_root = 'admin/structure/tickets/manage/%commerce_boooking_ticket_type';
  public $entity_admin_bundle = 4;
  public $views_table = 'commerce_booking_ticket';
  public $uses_page_manager = FALSE;

  /**
   * Determine if the entity allows revisions.
   */
  public function entity_allows_revisions($entity) {
    $retval[0] = $this->supports_revisions;
    $retval[1] = user_access('administer tickets');

    return $retval;
  }

  /**
   * Implements a delegated hook_form_alter.
   *
   * We want to add Panelizer settings for the bundle to the commerce_order_type form.
   */
  public function hook_form_alter(&$form, &$form_state, $form_id) {
    if ($form_id == 'commerce_booking_ticket_type_form') {
      if (isset($form['type'])) {
        $bundle = $form['type']['#default_value'];
        $this->add_bundle_setting_form($form, $form_state, $bundle, array('type'));
      }
    }
  }

  /**
   * Implements PanelizerEntityDefault::entity_access().
   */
  public function entity_access($op, $entity) {
    return entity_access($op, 'commerce_booking_ticket', $entity);
  }

  /**
   * Implements PanelizerEntityDefault::entity_save().
   */
  public function entity_save($entity) {
    return entity_save('commerce_booking_ticket', $entity);
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
