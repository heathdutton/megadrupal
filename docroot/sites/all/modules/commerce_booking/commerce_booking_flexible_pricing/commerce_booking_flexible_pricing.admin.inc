<?php
/**
 * @file
 * Administration pages for the Commerce Booking Flexible Pricing Module
 */

/**
 * Page callback for the ticket class admin page.
 */
function commerce_booking_flexible_pricing_manage_ticket_classes() {
  RulesPluginUI::$basePath = 'admin/commerce_booking/ticket_classes';
  $options = array('show plugin' => FALSE);

  $content['enabled']['title']['#markup'] = '<h3>' . t('Enabled ticket classes') . '</h3>';

  $conditions = array(
    'plugin' => array('or', 'and'),
    'active' => TRUE,
    'tags' => array('Ticket Class'),
  );
  $content['enabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['enabled']['rules']['#empty'] = t('There are no enabled ticket classes.');

  $content['disabled']['title']['#markup'] = '<h3>' . t('Disabled ticket classes') . '</h3>';

  $conditions['active'] = FALSE;
  $content['disabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['disabled']['rules']['#empty'] = t('There are no disabled ticket classes.');

  return $content;
}
