<?php

/**
 * @file
 * Handles the 'admin/structure/speed_form_blocks/responses' page and
 * posted response.
 */

/**
 * Does the display of the responses each block has received.
 */
function speed_form_blocks_responses() {

  $result = db_query('SELECT fid, page, block, extra, obtained from {speed_form_blocks_feedback} order by obtained DESC');
  $counter = 0;

  foreach ($result as $page) {

    if ($counter === 0) {

      $form['inline_feedback_main'] = array(
        '#markup' => '<h3>' . t('Responses received from Speed form blocks') . '</h3>',
      );

      $form["date_range"] = array(

        '#type' => 'fieldset',
        '#title' => t("Sorted by date (click to expand)"),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,

      );

    }

    $counter++;

    if (!isset($form[$page->page])) {

      $form[$page->page] = array(

        '#type' => 'fieldset',
        '#title' => t("%page results (click to expand)", array('%page' => $page->page)),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,

      );

    }

    if (!isset($form[$page->block])) {

      $form[$page->block] = array(

        '#type' => 'fieldset',
        '#title' => t("%page results (click to expand)", array('%page' => unserialize($page->block))),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,

      );

    }

    if (isset($page->extra)) {

      $extra = unserialize($page->extra);

      if (is_array($extra) && count($extra) != 0) {

        $extra_data = "<div>";

        foreach ($extra as $key => $value) {

          $data_to_display = "";

          if (is_array($value)) {

            $data_to_display .= implode(",", $value);
            $data_to_display .= ",";

          }
          else {

            $data_to_display .= $value . ",";

          }

          $data_to_display = drupal_substr($data_to_display, 0, drupal_strlen($data_to_display) - 1);
          $extra_data .= "<p><span>" . t('Setting') . " : " . str_replace(filter_xss($page->block) . "_", "", $key) . "</span> | " . t('value') . " :" . filter_xss($data_to_display) . "</p>";

        }

        $extra_data .= "</div>";

      }

    }

    $form[$page->page]['response_' . $page->obtained] = array(

      '#markup' => '<p style="border-bottom:1px solid black"><span>' . date("D, jS F Y G:i:s", $page->obtained) . '</span></p><p>' . $extra_data . '</p>',

    );

    $form["date_range"]['response_' . $page->obtained] = array(

      '#markup' => '<p style="border-bottom:1px solid black"><span>' . date("D, jS F Y G:i:s", $page->obtained) . '</span></p><p>' . $extra_data . '</p>',

    );

    $form[$page->page]['delete_' . $page->fid] = array(
      '#type' => 'checkbox',
      '#title' => t("Delete this response"),
    );

    $form[$page->block]['response_' . $page->obtained] = array(

      '#markup' => '<p style="border-bottom:1px solid black"><span>' . date("D, jS F Y G:i:s", $page->obtained) . '</span></p><p>' . $extra_data . '</p>',

    );

    $form["date_range"]['delete_' . $page->fid] = array(
      '#type' => 'checkbox',
      '#title' => t("Delete this response"),
    );

    $form[$page->block]['delete_' . $page->fid] = array(
      '#type' => 'checkbox',
      '#title' => t("Delete this response"),
    );

  }

  if ($counter === 0) {

    $form['inline_feedback_main_no_responses'] = array(
      '#markup' => '<p>' .
      t('No responses have been received or no blocks have been set up. Please visit block creation.')
      . '</p>',
    );

  }
  else {

    $form['deletion'] = array(
      '#type' => 'submit',
      '#value' => t('Delete these responses'),
      '#submit' => array('speed_form_blocks_deletion_handler'),
    );

    $form['delete_all'] = array(
      '#type' => 'submit',
      '#value' => t('Delete all responses'),
      '#submit' => array('speed_form_blocks_delete_all_handler'),
    );

  }

  return $form;

}

/**
 * Does the process of Deleting of a speed form block.
 */
function speed_form_blocks_deletion_handler($form, &$form_state) {

  foreach ($form_state['values'] as $key => $value) {

    if (is_array($value)) {

      foreach ($value as $key => $data) {

        if ($data == 1) {

          if (strpos($key, "delete_") !== FALSE) {

            $num_deleted = db_delete('speed_form_blocks_feedback')
              ->condition('fid', str_replace("delete_", "", $key))
              ->execute();

          }

        }

      }

    }

  }

}

/**
 * Does the process of Deleting of a speed form block.
 */
function speed_form_blocks_delete_all_handler($form, &$form_state) {

    $num_deleted = db_delete('speed_form_blocks_feedback')
      ->execute();

}
