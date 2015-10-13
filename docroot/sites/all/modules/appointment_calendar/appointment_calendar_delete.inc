<?php

/**
 * @file
 * Provides Delete page for selected date if there is no booked slot(s).
 */

/**
 * Implements hook_form().
 */
function appointment_calendar_list_delete_form($form, $form_state) {
  // Checking for Booked slots.
  // If slots booked in particular date disable option for delete.
  if (!empty($_GET['date'])) {
    $date = date('Y-m-d', $_GET['date']);
    $delete_query = db_select('field_data_appointment_date', 'ad');
    $delete_query->fields('ad', array('appointment_date_value'));
    $delete_query->condition('appointment_date_value', '%' . db_like($date) . '%', 'LIKE');
    $delete_result = $delete_query->execute()->fetchAll();
    if (count($delete_result) >= 1) {
      $form['date'] = array(
        '#markup' => 'Unable to delete ' . $date .
        '. Appointment already booked in selected date<br>If you still want to delete the selected date, delete timeslots booked and retry<br> ',
      );
      $form['return'] = array(
        '#type' => 'submit',
        '#value' => t('Return'),
      );
    }
    else {
      $form['date_markup'] = array(
        '#markup' => t('Are you sure to delete <b>:date</b>?<br>Note:All filled timeslots also will be deleted.<br>', array(':date' => $date)),
      );
      $form['date'] = array(
        '#type' => 'hidden',
        '#value' => $_GET['date'],
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Yes'),
      );
      $form['no'] = array(
        '#type' => 'submit',
        '#value' => t('No'),
      );
    }
    return $form;
  }
}

/**
 * Implements hook_form_submit().
 */
function appointment_calendar_list_delete_form_submit($form, &$form_state) {
  // Delete Slot.
  if ($form_state['values']['op'] == 'Yes') {
    db_delete('appointment_date')
        ->condition('date', $form_state['values']['date'])
        ->execute();
    drupal_set_message(t('Selected Date deleted successfully'));
    drupal_goto(t('admin/config/appointment-calendar/settings/list-date'));
  }
  // Go-to Listing Page.
  if (($form_state['values']['op'] == 'No') || ($form_state['values']['op'] == 'Return')) {
    drupal_goto(t('admin/config/appointment-calendar/settings/list-date'));
  }
}
