<?php

/**
 * @file
 * Handles the export page for Speed_form_blocks module.
 */

/**
 * Handles displaying data received and csv generation.
 */
function speed_form_blocks_export_data() {

  global $base_url;

  $form['inline_feedback_block_creation'] = array(
    '#markup' => '<h3>' . t('Speed form block data export') . '</h3>',
  );

  $result = db_query('SELECT extra from {speed_form_blocks_feedback}');
  $columns = array();

  $counter = 0;

  foreach ($result as $keys) {

    $counter++;

    $data = unserialize($keys->extra);

    if (is_array($data)) {

      foreach ($data as $key => $value) {

        if (!in_array($key, $columns)) {

          array_push($columns, $key);

        }

      }

    }

  }

  sort($columns);
  $result = db_query('SELECT page,block,extra,obtained from {speed_form_blocks_feedback}');
  $first_row = "page,block," . implode(",", $columns) . ",obtained\n";
  $data_set = new stdClass();
  $data_set->all = $first_row;

  foreach ($result as $data) {

    $extra = unserialize($data->extra);
    $curr_row = $data->page . "," . unserialize($data->block) . ",";

    for ($x = 0; $x < count($columns); $x++) {

      if (isset($extra[$columns[$x]])) {

        $curr_row .= $extra[$columns[$x]] . ",";

      }
      else {

        $curr_row .= ",";

      }

    }

    $curr_row = drupal_substr($curr_row, 0, (drupal_strlen($curr_row) - 1)) . "," . $data->obtained . "\n";

    if (!isset($data_set->{$data->page})) {

      $data_set->{$data->page} = $first_row;
      $data_set->{$data->page} .= $curr_row;

    }
    else {

      $data_set->{$data->page} .= $curr_row;

    }

    if (!isset($data_set->{$data->block})) {

      $data_set->{$data->block} = $first_row;
      $data_set->{$data->block} .= $curr_row;

    }
    else {

      $data_set->{$data->block} .= $curr_row;

    }

    $data_set->all .= $curr_row;

  }

  $result = db_query('SELECT page,block,extra from {speed_form_blocks_feedback}');

  $response_data = new stdClass();
  $data_column_headers = array();
  
  $blocks_list = array();

  foreach ($result as $block) {

    $block_name = unserialize($block->block);
    $block_data = implode(",", unserialize($block->extra));
    $data_column_headers = array_keys(unserialize($block->extra));
    $column_headers = implode(",", $data_column_headers);

    if (!isset($response_data->{$block->page . " Page "})) {

      $response_data->{$block->page . " Page "} = array($column_headers);
      array_push($response_data->{$block->page . " Page "}, $block_data);

    }
    else {

      array_push($response_data->{$block->page . " Page "}, $block_data);

    }

    if (!isset($response_data->{$block_name . " Block "})) {

      array_push($blocks_list, $block_name . " Block ");

      $response_data->{$block_name . " Block "} = array($column_headers);
      array_push($response_data->{$block_name . " Block "}, $block_data);

    }
    else {

      array_push($response_data->{$block_name . " Block "}, $block_data);

    }

  }
  
  foreach ($response_data as $form_block_name => $form_block_data) {

    $counter++;

    $form["block_" . $form_block_name] = array(

      '#type' => 'fieldset',
      '#title' => t("%page (click to expand)", array('%page' => $form_block_name)),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,

    );

    $form["block_" . $form_block_name]["csv"] = array(
      '#type' => 'textarea',
      '#default_value' => implode("\n", $form_block_data),
      '#description' => t("Please note the first row is a 'header'"),
    );
  
    if (in_array($form_block_name, $blocks_list)) {
  
      $form["block_" . $form_block_name]["csv_link"] = array(
        '#markup' => t('Access the CSV directly at @url/speed_form_blocks/csv_export/@block', array('@url' => $base_url, '@block' => str_replace(" Block", "", $form_block_name))),
      );

   }

  }

  if ( $counter != 0 ) {

    $form["ALL"] = array(
      '#type' => 'fieldset',
      '#title' => t("All Data (click to expand)"),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    $form["ALL"]["csv"] = array(
      '#type' => 'textarea',
      '#default_value' => $data_set->all,
      '#description' => t("Please note the first row is a 'header'"),
    );

  }
  else {

    $form["display"] = array(
      '#markup' => t('No responses have been submitted yet.'),
    );

  }

  return $form;

}
