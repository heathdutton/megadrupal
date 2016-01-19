<?php

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function millennium_cron_do() {

  // Clear import cache tables
  $session = (REQUEST_TIME - 3600) * 100;
  db_delete('millennium_import_cache')
    ->condition('session', $session, '<')
    ->execute();

  module_load_include('inc', 'millennium', 'millennium.import');
  $elapsed_crawl = 0;
  $elapsed_node = 0;

  // Is URL set?
  if (($base_url = variable_get('millennium_crawl_baseurl', FALSE)) === FALSE) {
    return;
  }
  if (!millennium_ping($base_url)) {
    watchdog("Millennium", "Cron run aborted because WebOPAC at @url not responding", array("@url" => $base_url), WATCHDOG_ERROR);
    return;
  }

  // Did user indicate crawl should be reset?
  if (variable_get('millennium_webopac_reset', FALSE)) {
    _millennium_crawl_restart();
    millennium_variable_set('millennium_webopac_reset', FALSE);
  }

  $default_start = variable_get('millennium_webopac_start_itemrecord', 10000);
  $default_end = variable_get('millennium_webopac_end_itemrecord', 20000);
  $last_succesful_rec_num = variable_get('millennium_webopac_latest_successful_itemrecord', $default_start);
  $current_rec_num = variable_get('millennium_webopac_current_itemrecord', $last_succesful_rec_num);
  $beginning_rec_num = $current_rec_num;

  // How many successfull imports before quitting the crawl
  $max_to_import = intval(variable_get("millennium_webopac_maxrecords", 50));

  // How many back-to-back "not found" records before quitting the crawl
  #$max_notfound = $max_to_import;
  $sequential_tot_notfound = 0;
  $tot_attempted = 0;
  $tot_notfound = 0;
  $tot_imported = 0;
  $tot_fail = 0;
  $will_auto_crawl = variable_get('millennium_crawl_flag', "0") != "0";

  // MANUAL IMPORT QUEUE =======================================================
  if ($will_auto_crawl) {
    $max_to_import_from_queue = $max_to_import * 0.5;
  }
  else {
    $max_to_import_from_queue = $max_to_import;
  }
  /* Use the queue table first, to import up to $max_to_import items.
   * import up to 50% hi-priority
   * import up to 25% low priority
   */
  $queue_jobs = array(
    // pri => limit
    1 => $max_to_import_from_queue * 0.8,
    0 => $max_to_import_from_queue,
  );
  foreach ($queue_jobs as $priority => $limit) {
    // Get items to import from DB
    $handle = db_query_range("SELECT id, recnum, force_update, base_url FROM {millennium_import_queue} WHERE priority = :priority", 0, $limit, array(':priority' => $priority));
    $stop = $tot_imported + $limit;
    $recnums = array();
    foreach ($handle as $queued_item) {
      $recnums[$queued_item->base_url][] = $queued_item->recnum;
      $force_update[$queued_item->base_url][$queued_item->recnum] = $queued_item->force_update;
      $queue_id[$queued_item->base_url][$queued_item->recnum] = $queued_item->id;
    }
    $tot_attempted += sizeof($recnums);
    foreach ($recnums as $base_url => $recs) {
      // Chunk array into groups of 50 (maximum number allowed by bookcart)
      $recnums_chunks = array_chunk($recs, 50);
      foreach ($recnums_chunks as $recnums_chunk) {
        $fetched = millennium_fetch_records_via_bookcart($recnums_chunk, FALSE, $base_url);
        $elapsed_crawl += $fetched['elapsed'];

        // Remove not-found items from queue
        foreach ($fetched['not_found'] as $recnum) {
          db_delete('millennium_import_queue')
            ->condition('id', $queue_id[$base_url][$recnum], '=')
            ->execute();
          $tot_notfound++;
          $import_errors[] = array("recnum" => $recnum, "error" => 'Not found');
        }

        // Import successful fetches only
        // Get author name for new nodes
        $user = array_shift(user_load_multiple(array(), array('name' => variable_get('millennium_newitems_author_name', millennium_default_author()))));

        timer_start("millennium_node");
        foreach ($fetched['found'] as $recnum => $data) {
          db_delete('millennium_import_queue')
            ->condition('id', $queue_id[$base_url][$recnum], '=')
            ->execute();
          $data['user'] = $user;
          $result = millennium_process_record($data, $force_update[$base_url][$recnum]);

          if ($result["success"] !== FALSE) {
            $tot_imported++;
          }
          else {
            $tot_fail++;
            watchdog("Millennium",
              "Queue #@id: Failed to import node: URL @url, item #@recnum, error: @error",
              array("@id" => $ids[$recnum], "@url" => $base_url, "@recnum" => $recnum, "@error" => $result["error"])
            );
            // Build a list of errors
            $import_errors[] = array("recnum" => $recnum, "error" => $result["error"]);
          }

          if ($tot_imported > $stop || $tot_imported > $max_to_import) {
            break;
          }

        }
        $timer = timer_stop("millennium_node");
        $elapsed_node += $timer['time'] / 1000;
      } // foreach ($recnums_chunks as $recnums_chunk)
    } // foreach ($recnums as $base_url => $recs) {
  }

  // Send errors via email
  /*
  if (is_array($import_errors)) {
    $subject = "Millennium module import errors";
    $body =  "** This is an automatic email; DO NOT REPLY **\r\n\r\n";
    $body .= "$subject\r\n";
    $body .= "These items could not be imported, due to the following errors:\r\n\r\n";
    foreach ($import_errors as $error) {
      $body .= $error["recnum"] ."|". $error["error"] ."\r\n";
    }
    $to = variable_get('site_mail', ini_get('sendmail_from'));
    $account = array($to);
    $language = user_preferred_language($account);
    $object = array();
    $context['subject'] = $subject;
    $context['body'] = $body;
    $params = array('account' => $account, 'object' => $object, 'context' => $context);
    drupal_mail('millennium', 'millennium', $to, $language, $params);
  }
  */

  if ($will_auto_crawl) {

    // AUTO CRAWL ================================================================
    $record_type = variable_get('millennium_crawl_type', "b");
    $rollover_limit = intval($default_end + ($default_end - $default_start) * .05);
    $recnums = array();
    for ($num = $beginning_rec_num; $num < $beginning_rec_num + $max_to_import; $num++) {
      $recnums["{$record_type}{$num}"] = "{$record_type}{$num}";
    }
    $tot_attempted += sizeof($recnums);

    // Get author name for new nodes
    $user = array_shift(user_load_multiple(array(), array('name' => variable_get('millennium_newitems_author_name', millennium_default_author()))));

    // Chunk array into groups of 50 (maximum number allowed by bookcart)
    $recnums_chunks = array_chunk($recnums, 50);
    foreach ($recnums_chunks as $recnums_chunk) {
      $fetched = millennium_fetch_records_via_bookcart($recnums_chunk, FALSE, $base_url);
      $elapsed_crawl += $fetched['elapsed'];
      #dpm($fetched);
      timer_start("millennium_node");
      foreach ($fetched['found'] as $data) {
        $data['user'] = $user;
        $result = millennium_process_record($data);
        if ($result["success"] !== FALSE) {
          $tot_imported++;
        }
        else {
          $tot_fail++;
          watchdog("Millennium",
            "Crawl: Failed to import node: record #@recnum, error: @error",
            array("@recnum" => $data['item_recnum'], "@error" => $result["error"])
          );
        }
      }
      $timer = timer_stop("millennium_node");
      $elapsed_node += $timer['time'] / 1000;

      // If any records found, reset the back-to-back not-found record count
      if (sizeof($fetched["found"] > 0 )) {
        $sequential_tot_notfound = 0;

        //Store last successful bibrec imported
        $tmp = array_pop($fetched['found']);
        $current_rec_num = substr($tmp[($record_type == 'i') ? 'item_recnum' : 'bib_recnum'], 1) + 0; // Remove the "i" or "b" from X100000
        #drupal_set_message("current_rec_num = $current_rec_num");
        if ($current_rec_num > $last_succesful_rec_num) {
          millennium_variable_set('millennium_webopac_latest_successful_itemrecord', $current_rec_num);
          $last_succesful_rec_num = $current_rec_num;
        }

        // Adjust rollover limit by 5% the size of the database
        #if ($current_rec_num > $rollover_limit) {
        #  $rollover_limit = intval($current_rec_num + ($current_rec_num - $default_start) * .05);
        #}
      }

      #if ($tot_imported >= $max_to_import) break;

      // Increase not-found counter by number of items not found
      $sequential_tot_notfound += sizeof($fetched['not_found']);
      $tot_notfound += sizeof($fetched['not_found']);

      // Store last tried item
      $last_tried_itemnum = substr(array_pop($recnums_chunk), 1) + 0;
      millennium_variable_set('millennium_webopac_current_itemrecord', $last_tried_itemnum);
    }

    // Do rollover when no records were found in this cron run after 5% the estimated database size
    if ($last_tried_itemnum > $rollover_limit && $sequential_tot_notfound >= $max_to_import) {
      watchdog("Millennium", "Crawl will start over, as it has detected no records after #@recnum", array("@recnum" => $rollover_limit));
      _millennium_crawl_restart();
    }

    // Adjust ending record if records are being found past that number
    if ($current_rec_num > $default_end) {
      millennium_variable_set('millennium_webopac_end_itemrecord', $current_rec_num);
      $default_end = $current_rec_num;
    }
  }

  if ($tot_attempted) {
    millennium_time_history($elapsed_node, $elapsed_crawl, $tot_attempted, $tot_notfound, $tot_imported, $tot_fail);

    watchdog("Millennium", "Cron import finished: @url: @attempted attempted, @notfound not found on WebOpac, @imported imported ok, @failed could not import, in @time seconds (@time2 items per second)",
      array(
        "@url" => $base_url,
        "@attempted" => $tot_attempted,
        "@notfound" => $tot_notfound,
        "@imported" => $tot_imported,
        "@failed" => $tot_fail + 0,
        "@time" => sprintf("%2.1f", $elapsed_node + $elapsed_crawl),
        "@time2" => sprintf("%2.1f", $tot_attempted / ($elapsed_node + $elapsed_crawl)),
      )
    );
  }
}

/**
 * Helper function that resets values for restarting the automatic crawl process.
 */
function _millennium_crawl_restart() {
  $default_start = variable_get('millennium_webopac_start_itemrecord', 10000);
  watchdog("Millennium", "Restarting automatic crawl from record @start", array("@start" => $default_start));
  millennium_variable_set('millennium_webopac_latest_successful_itemrecord', $default_start);
  millennium_variable_set('millennium_webopac_current_itemrecord', $default_start);
  millennium_variable_set('millennium_webopac_sequential_failed_imports', 0);
}
