<?php
/**
 * @file
 * Menu callback methods for the tether_stats module.
 */

/**
 * JSON callback: Track an event in the stats tables.
 *
 * @see tether_stats_menu()
 */
function tether_stats_track() {

  if (variable_get('tether_stats_active', FALSE)) {

    $output_data = _tether_stats_track_element($_GET);

    if (isset($_GET['type']) && $_GET['type'] == 'hit') {

      // See if there were any embedded impressions in the call and
      // track those too.
      for ($i = 0; isset($_GET['imp' . $i]); $i++) {

        $params = array();
        $impression_data = rawurldecode($_GET["imp{$i}"]);
        $parts = explode(',', $impression_data);

        foreach ($parts as $part) {

          list($key, $value) = explode('=', $part);
          $params[$key] = $value;
        }
        $params['type'] = 'impression';
        $params['alid'] = $output_data['alid'];

        _tether_stats_track_element($params);
      }
    }

    drupal_json_output($output_data);
  }
  else {

    print '{"status": "' . t('Stats tracking is currently inactive.') . '"}';
  }
  exit();
}

/**
 * Page callback: Delete a derivative.
 *
 * @param object $derivative
 *   The derivative object.
 *
 * @see tether_stats_menu()
 */
function tether_stats_admin_derivative_delete_callback($derivative) {

  $build = array();

  $old_db = db_set_active(tether_stats_set_db());

  // Prevent the removal of derivatives in use.
  $query = db_select('tether_stats_element', 'e')
    ->condition('derivative_id', $derivative->id, '=')->countQuery();

  $num_elements = $query->execute()->fetchField(0);
  db_set_active($old_db);

  if ($num_elements > 0) {

    drupal_set_message(t('There are currently %num_elements elements that have been created using the %derivative derivative for entity_type %entity_type.<br /> Removing this derivative would purge these stat elements and all tracked data associated with them.<br /><br /> As a result, this action is disallowed at this interface.',
      array(
        '%num_elements' => $num_elements,
        '%derivative' => $derivative->name,
        '%entity_type' => $derivative->entity_type,
      )), 'warning');

    drupal_goto('admin/config/system/tether_stats/derivatives');
  }
  else {

    module_load_include('inc', 'tether_stats', 'tether_stats.forms');
    $build = drupal_get_form('tether_stats_admin_derivatives_delete_confirm_form', $derivative);
  }

  return $build;
}
