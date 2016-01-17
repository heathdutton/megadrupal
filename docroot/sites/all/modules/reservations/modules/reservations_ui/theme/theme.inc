<?php

/**
 * @file
 * Reservations - Managed Equipment Reservation Checkout and Inventory
 */


/**
 * returns the number of items currently checked out for each content type
 * TODO: change this to a views view.
 */
function theme_reservations_ui_current_inventory() {
  
  // admin/reservations/manage/current_inventory/inventory/$tid
  // check to see if inventory is being passed
  $inventory = (arg(4) == 'inventory') ? arg(5) : NULL;
  
  if ($inventory) {
    $term = taxonomy_term_load($inventory);
    
    drupal_set_title('Current Inventory of ' . $term->name . ' Items');
  }
  
  $header = array(
    t('Item'),
    t('On Hand'),
    t('Unavailable'),
    t('Checked Out'),
    t('Total'),
    t('Operations'),
  );

  $reservable_types = reservations_content_types();

  $rows = array();

  foreach ($reservable_types as $reservable_type) {
    $include = FALSE;  // default to skipping the type unless 
    
    if ($inventory) {
      foreach ($reservable_type['inventory_tids'] as $tid) {
        
        if ($tid == $inventory) {
          $include = TRUE;
        }
      }
    } else {
      // if there is no inventory, include everything
      $include = TRUE;
    }
    
    if ($include) {

    $operations = l(t('Edit'), 'admin/structure/types/manage/' . $reservable_type['type'], array('query' => array('destination' => 'admin/reservations/manage/current_inventory/' . $inventory)));
    $operations .= " | ";
    
    $typelink = str_replace('_', '-', $reservable_type['type']);
    $operations .= l(t('Add Item'), 'node/add/' . $typelink, array('query' => array('destination' => 'admin/reservations/manage/current_inventory/' . $inventory)));

    $checked_out = reservations_get_reservation_count($reservable_type['type']);

    $available = reservations_get_count($reservable_type, RESERVATIONS_AVA_F);
    $onhand = $available - $checked_out;

    $unavailable = reservations_get_count($reservable_type, RESERVATIONS_UNA_F);
    $total = $onhand + $unavailable + $checked_out;
    $available = l($onhand, 'admin/reservations/manage/current_inventory/type/' . $reservable_type['type'] . '/Available');
    $checked_out = l($checked_out, 'admin/reservations/manage/current_inventory/type/' . $reservable_type['type'] . '/checked-out');
    $unavailable = l($unavailable, 'admin/reservations/manage/current_inventory/type/' . $reservable_type['type'] . '/Unavailable');

    $title = views_get_view('reservations_inventory_view')
        ? l($reservable_type['type_name'], 'admin/reservations/manage/inventory/type', array('html' => TRUE, 'query' => 'type[0]=' . $reservable_type['type']))
        : $reservable_type['type_name'];
  
  
    $rows[] = array(
      $title,
      $available,
      $unavailable,
      $checked_out,
      $total,
      $operations,
    );
  }
  } //include
  
  $output = theme('status_messages');
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  $output .= '&raquo; ' . l(t('Add a new content type'), 'admin/structure/types/add', array('query' => array('destination' => 'admin/reservations/manage/current_inventory')));
  return $output;
}
