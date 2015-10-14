<?php

/**
 * @file
 * Handles the management page for Speed_form_blocks module.
 */

/**
 * Does the the URL for CSV Export
 */
function speed_form_blocks_csv_export() {

  $result = db_query('SELECT page,extra,obtained from {speed_form_blocks_feedback} WHERE block = :block_name', array(':block_name' => serialize(arg(2))));
   
  if ($result->rowCount()!=0) {

    $counter = 0;

    foreach ($result as $data) {

      header('Content-type: application/csv');

      header('Content-Disposition: attachment; filename="' . arg(2) . '.csv"');

      $extra = unserialize($data->extra);

      if ($counter==0) {

        echo "\"page\",";

        $keys = array_keys($extra);

        foreach ($keys as $key) {

          echo "\"" . addslashes($key) . "\",";

        }
 
        echo "\"obtained\"\n";

      }

      $curr_row = "\"" . addslashes($data->page) . "\",";

      foreach ($extra as $key => $value) {

        if (isset($value)) {

          $curr_row .= "\"" . addslashes($value) . "\",";

        }
        else {

          $curr_row .= ",";

        }

      }

      echo drupal_substr($curr_row, 0, (drupal_strlen($curr_row) - 1)) . ",\"" . $data->obtained . "\"\n";

      $counter++;

    }

  }
  else {

    drupal_not_found();

  }

}
