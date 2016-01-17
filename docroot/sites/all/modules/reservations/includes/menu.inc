<?php
/**
 * @file
 * Reservations menu components.
 */

/**
 * Menu callback for AHAH additions.
 */
function reservations_choice_js($form, $form_state) {

  return $form['choice_wrapper'];
  list($form, $form_state) = reservations_ahah_get_form();

  $selected_portion = $form['choice_wrapper'];

  reservations_ahah_output($selected_portion);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function reservations_taxonomy_json($node, $vid, $did) {

  reservations_taxonomy_node_delete_vocabulary($node, $vid);

  $tids = $_POST['reservations_reservation_items']
    ? $_POST['reservations_reservation_items'][$did]['accessories']['choices'][$vid] : $_POST[$did]['accessories']['choices'][$vid];

  reservations_taxonomy_node_save_terms($node, $tids);

  reservations_ahah_output('', TRUE);
}



