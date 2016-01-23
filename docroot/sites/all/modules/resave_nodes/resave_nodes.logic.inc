<?php
/**
 * @file
 * Login include for the Resave Nodes module.
 *
 * This file has the breakout of the main function that does the node
 * resaving.  This was moved out of the resave_nodes.module file for better
 * organization.
 */

/**
 * Submit handler for the 'run now' button on the config page.
 */
function resave_nodes_run_now_submit() {
  $results = resave_nodes_do_that_thing();
  drupal_set_message(t('Resaving nodes complete. @nodes nodes updated in @sec seconds.',
    array(
      '@nodes' => $results['number_updated'],
      '@sec'   => $results['run_time'],
    )
  ));
}


/**
 * The 'business logic' for resaving the nodes.  This resided in
 * hook_cron() in the past.  It's been pulled out into its own file to
 * allow the creation of more refined run options.
 */
function resave_nodes_do_that_thing() {
  $resave_nodes_last_run = variable_get('resave_nodes_last_run');
  $resave_nodes_selected_types = variable_get('resave_nodes_selected_types', array());
  $resave_nodes_all_nodes = variable_get('resave_nodes_all_nodes');

  $start_time = time();

  $total_num_nodes_saved = 0;
  if (isset($resave_nodes_selected_types)) {
    foreach ($resave_nodes_selected_types as $key => $value) {
      if ($value != '0') {

        // Default action: resave only recently created/changed.
        if ($resave_nodes_all_nodes['resave_all_nodes'] == '0') {
          $query = db_query('SELECT nid FROM {node} WHERE changed >= :last_cron_run AND type = :content_type',
            array(
              ":last_cron_run" => $resave_nodes_last_run,
              ":content_type"  => $value,
            )
          );

        // Resave everything for specified content type.
        } else {

          $query = db_query('SELECT nid FROM {node} WHERE type = :content_type',
            array(
              ":content_type"  => $value,
            )
          );
        }

        $num_nodes_saved = 0;
        foreach ($query as $result) {
          $node = node_load($result->nid);
          node_save($node);
          watchdog(
            'resave_nodes',
            'Resaving node "@res" of content type "@ctype"',
            array(
              '@res'   => print_r($node->title, TRUE),
              '@ctype' => print_r($value, TRUE),
            ),
            WATCHDOG_INFO
          );
          $num_nodes_saved += 1;
        }
        $total_num_nodes_saved += $num_nodes_saved;
      }
    }
  }

  $time_delta = time() - $start_time;
  watchdog(
    'resave_nodes',
    'Finished updating @num new/changed nodes. Runtime was @time seconds',
    array(
      '@num'  => print_r($total_num_nodes_saved, TRUE),
      '@time' => print_r($time_delta, TRUE),
    ),
    WATCHDOG_INFO
  );
  variable_set('resave_nodes_last_run', $start_time + $time_delta);

  return array(
    'number_updated' => $total_num_nodes_saved,
    'run_time'       => $time_delta,
  );
}
