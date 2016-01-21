<?php

/**
 * @file
 *
 * Plugin to provide a node context. A node context is a node wrapped in a
 * context object that can be utilized by anything that accepts contexts.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Schedule Day"),
  'description' => t('Display schedule of particular day.'),
  'context' => 'cod_schedule_context_create_day',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'edit form' => 'cod_schedule_context_day_settings_form',
  'defaults' => '',
  'no ui' => FALSE,
  'keyword' => 'cod_schedule_day',
  'context name' => 'cod_schedule_day',
  'convert' => 'cod_schedule_context_day_convert',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the schedule day machine name for this context.'),
  ),
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function cod_schedule_context_create_day($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('cod_schedule_day');
  $context->plugin = 'cod_schedule_day';

  // We don't know the current date in the argument, lets try to create one
  if ($empty) {
    return $context;
  }

  if (!empty($data)) {
    // Support the array storage from the settings form but also handle direct input from arguments.
    $context->data = is_array($data) ? $data['day'] : $data;
    // We're using a configured context, but its not set. Use todays date.
    if(isset($data['day'])) {
      $content_type = isset($data['content_type']) ? $data['content_type']: NULL;
      if ($data['day'] == '') {
        // Fetch the event via path. TODO: Get event via panels context.
        if (isset($data['existing contexts']['argument_entity_id:node_1'])) {
          $event_nid = $data['existing contexts']['argument_entity_id:node_1']->argument;
          $event_days = cod_schedule_days($event_nid, $content_type);
          $today_dt = new DateTime();
          $event_start = new DateTime(array_shift($event_days));
          $event_end = new DateTime(array_pop($event_days));
          if ($today_dt < $event_start) {
            $date = $event_start;
          }
          else {
            if ($today_dt > $event_end) {
              $date = $event_end;
            }
            else {
              $date = $today_dt;
            }
          }
          $context->data = date('Y-m-d', $date->getTimestamp());
          $context->title = date('l', $date->getTimestamp());
        }
      }
      else if ($data['day'] !== '') {
        $date = new DateTime(strtotime($data['day']));
        $context->data = date('Y-m-d', $date->getTimestamp());
        $context->title = date('l', $date->getTimestamp());
      }
    }
    else {
      $output = check_plain($data);
      $context->title = ($conf) ? check_plain($data['identifier']) : drupal_strtoupper(drupal_substr($output, 0, 1)) . drupal_substr($output, 1);
    }
    return $context;
  }
}

function cod_schedule_context_day_settings_form($form, &$form_state) {
  $conf = &$form_state['conf'];

  $form['day'] = array(
    '#title' => t('Enter the day in YYYY-MM-DD'),
    '#type' => 'textfield',
    '#maxlength' => 64,
    '#weight' => -10,
  );

  $form['content_type'] = array(
      '#title' => t('Enter an optional content type'),
      '#type' => 'textfield',
      '#maxlength' => 64,
      '#weight' => -10,
  );

  if (!empty($conf['day'])) {
    $form['day']['#description'] = t('Currently set to !link', array('!link' => $conf['day']));
  }
  if (!empty($conf['content_type'])) {
    $form['content_type']['#description'] = t('Currently set to !ct', array('!ct' => $conf['content_type']));
  }

  return $form;
}

/**
 * Validate a time format.
 */
function cod_schedule_context_day_settings_form_validate($form, &$form_state) {
  // Validate the autocomplete
  // We should do something here!
}

function cod_schedule_context_day_settings_form_submit($form, &$form_state) {
  // This will either be the value set previously or a value set by the
  // validator.
  $form_state['conf']['day'] = $form_state['values']['day'];
  $form_state['conf']['content_type'] = $form_state['values']['content_type'];
}

/**
 * Convert a context into a string.
 */
function cod_schedule_context_day_convert($context, $type) {
  switch ($type) {
    // Convert a vocabulary keyword into the data.
    case 'nid':
      return $context->data;
  }
}
