<?php
/**
 * Availability Calendar code to edit the calendar using a month based widget.
 */

module_load_include('inc', 'availability_calendar');

/**
 * Defines the form elements to edit this field.
 *
 * Called by our implementation of hook_field_widget_form(). Parameters are as
 * passed to hook_field_widget_form(). Return is what hook_field_widget_form()
 * should return.
 *
 * @param array $form
 * @param array $form_state
 * @param array $field
 * @param array $instance
 * @param string $langcode
 * @param array $items
 * @param integer $delta
 * @param array $element
 *
 * @return array
 *   Form elements to edit this field on an entity edit form.
 */
function availability_calendar_field_widget_month_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  static $new_cid_count = 0;
  $item = isset($items[$delta]) ? $items[$delta] : NULL;
  $cid = !empty($item['cid']) ? $item['cid'] : ('new' . ++$new_cid_count);
  $cvid = availability_calendar_get_cvid();
  $name = !empty($item['name']) ? $item['name'] : '';
  $settings = $instance['widget']['settings'] + $instance['settings'] + $field['settings'];

  // Make sure this file gets loaded on submit
  $form_state['build_info']['files'][] = array('type' => 'inc', 'module' => 'availability_calendar', 'name' => 'availability_calendar.widget');

  $element = array_merge($element, array(
    '#type' => 'fieldset',
    '#element_validate' => array('availability_calendar_field_widget_month_form_validate'),
    '#attached' => availability_calendar_field_widget_month_attach_js_css($cvid),
  ));
  $element['enabled'] = array(
    '#type' => 'checkbox',
    // Don't show the checkbox if the user may not disable the calendar.
    '#access' => $settings['allow_disable'],
    '#title' => t('Enable the availability calendar'),
    '#description' => t("Uncheck the checkbox if you don't want a calendar at all for this @entity", array('@entity' => $element['#entity_type'])),
    '#default_value' => isset($item['enabled']) ? $item['enabled'] : 1,
    '#attributes' => array('class' => array('availability-enable')),
  );

  $element['calendar_details_div'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="availability-details">',
  );

  $element['name'] = array(
    '#type' => 'textfield',
    // Don't show this textbox if the user may not give the calendar a name.
    '#access' => $settings['add_name'],
    '#title' => t('Name'),
    '#default_value' => isset($item['name']) ? $item['name'] : '',
  );
  $element['description'] = array(
    '#type' => 'item',
    '#title' => t('Availability'),
    '#description' => t('<p>To update the calendar: select the new state and the date range to apply it to. Repeat if needed. When finished, click on <em>@button</em>. You can select a date range by clicking on either:</p>
     <ul>
     <li>The begin and end date of the period you want to change.</li>
     <li>A week number to select that whole week at once.</li>
     <li>The name of the month to select that whole month at once.</li>
     <li>The name of a day of the week to select all those days of the week in that month.</li>
     </ul>', array('@button' => t('Save'))),
  );
  $allowed_states = availability_calendar_get_states(array_filter($settings['allowed_states']), 'label');
  $default_state = array_key_exists($settings['default_state'], $allowed_states) ? $settings['default_state'] : key($allowed_states);
  $element['availability_states'] = array(
    '#type' => 'radios',
    '#title' => t('Select new state'),
    '#default_value' => $default_state,
    '#options' => $allowed_states,
    '#attributes' => array('class' => array('availability-states')),
  );
  $element['availability_calendar'] = array(
    '#type' => 'markup',
    '#theme' => $instance['widget']['type'],
    '#markup' => '',
    '#cid' => $cid,
    '#cvid' => $cvid,
    '#name' => $name,
    '#settings' => $settings,
  );
  $element['calendar_details_enddiv'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );
  $element['availability_changes'] = array(
    '#type' => 'hidden',
    '#title' => t('Changes in availability'),
    '#default_value' => '',
    '#attributes' => array('class' => array('availability-changes')),
  );
  // Add element cid. It does not have to be sent to the client, but is used on
  // submit. Store 0 for new calendars.
  $element['cid'] = array(
    '#type' => 'hidden',
    '#access' => FALSE,
    '#default_value' => (int) $cid,
  );
  // Add the unique cid, used to match changes, fields, and elements in the
  // processing phase.
  $element['cid_unique'] = array(
    '#type' => 'hidden',
    '#access' => FALSE,
    '#default_value' => $cid,
  );
  return $element;
}

/**
 * Attaches the necessary javascript files and settings as well as the
 * necessary css files. It does not add the base javascript, that will be added
 * by the theme function that renders the calendar view.
 *
 * @param int $cvid
 *   Id of the calendar view this editor is attached to.
 *
 * @return array
 */
function availability_calendar_field_widget_month_attach_js_css($cvid) {
  $result = array(
    'js' => array(
      drupal_get_path('module', 'availability_calendar') . '/availability_calendar.edit.js',
      array(
        'data' => array(
          'availabilityCalendar' => array(
            'editors' => array(
              $cvid => array(
                'cvid' => $cvid,
         )))),
        'type' => 'setting',
      ),
    ),
    'css' => array(drupal_get_path('module', 'availability_calendar') . '/availability_calendar.edit.css'),
  );
  return $result;
}

/**
 * Callback to validate the calendar changes.
 * @link http://api.drupal.org/api/drupal/developer--topics--forms_api_reference.html/7#element_validate
 */
function availability_calendar_field_widget_month_form_validate($element, &$form_state/*, $form*/) {
  // Do not update on preview, only on a real submit.
  // Drupal might have added additional characters to the #id to make it unique,
  // so only check on the start of the #id.
  $op = isset($form_state['clicked_button']) ? $form_state['clicked_button']['#id'] : '';
  if (substr($op, 0, strlen('edit-submit')) === 'edit-submit' && $element['enabled']['#value'] == '1') {
    $changes = array();
    $is_valid = TRUE;
    $lines = explode("\n", $element['availability_changes']['#value']);
    // Remove the last incomplete line.
    array_pop($lines);
    foreach ($lines as $line) {
      if (!empty($line)) {
        $change = availability_calendar_field_widget_month_form_validate_line($line, $element);
        if ($change !== FALSE) {
          $changes[] = $change;
        }
        else {
          $is_valid = FALSE;
          form_error($element, t('The requested calendar changes contain an invalid request.'));
          break;
        }
      }
    }
    // If the field changes validated, store them for processing in the submit
    // phase. In the submit phase our hook_field_attach_submit implementation
    // will process the changes. As that hook operates on the entity, we need
    // to be able to match these changes with the entity, field, language, and
    // delta. We do so using the value of the cid_unique field.
    if ($is_valid) {
      $form_state['availability_calendar_updates'][] = array($element['#field_name'], $element['#language'], $element['#delta'], $element['cid_unique']['#value'], $changes);
    }
  }
}

/**
 * Validates a single command line.
 *
 * @param string $line
 *   Command line: "{sid},yyyy-mm-dd,yyyy-mm-dd".
 * @param array $element
 *
 * @return array|false
 *   A keyed  array with keys 'from', 'to' and 'state' representing the parsed
 *   and validated command line, or FALSE on validation errors.
 */
function availability_calendar_field_widget_month_form_validate_line($line, $element) {
  // Basic syntax checking.
  $parts = explode(',', trim($line));
  if (count($parts) !== 3
   || !ctype_digit($parts[0])
   || preg_match('/^[1-2][0-9]{3}-[0-1][0-9]-[0-3][0-9]$/', $parts[1]) !== 1
   || preg_match('/^[1-2][0-9]{3}-[0-1][0-9]-[0-3][0-9]$/', $parts[2]) !== 1) {
    return FALSE;
  }
  // Check state: allowed state.
  $state = $parts[0];
  if (!array_key_exists($state, $element['availability_states']['#options'])) {
    return FALSE;
  }
  // Check dates: valid dates, in between ranges of the calendar and from <= to.
  $year = date('Y');
  $month = date('m');
  if (!checkdate(substr($parts[1], 5, 2), substr($parts[1], 8, 2), substr($parts[1], 0, 4))
    || $parts[1] < date(AC_ISODATE, mktime(0, 0, 0, $month, 1, $year))) {
    return FALSE;
  }
  $months = $element['availability_calendar']['#settings']['show_number_of_months'];
  if (!checkdate(substr($parts[2], 5, 2), substr($parts[2], 8, 2), substr($parts[2], 0, 4))
   || $parts[2] > date(AC_ISODATE, mktime(0, 0, 0, $month + $months + 1, 0, $year))) {
    return FALSE;
  }
  $from = new DateTime($parts[1]);
  $to = new DateTime($parts[2]);
  if ($from > $to) {
    return FALSE;
  }
  return array('state' => $state, 'from' => $from, 'to' => $to);
}

/**
 * Called by implementation of hook_field_attach_submit.
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_attach_submit/7
 */
function availability_calendar_field_attach_submit_inc($entity_type, $entity, $form, &$form_state) {
  if (!empty($form_state['availability_calendar_updates'])) {
    foreach ($form_state['availability_calendar_updates'] as $update_info) {
      // A form can contain multiple entities with multiple calendar fields:
      // match the changes with specific fields using the cid_unique value.
      list($field_name, $language_code, $delta, $cid_unique, $changes) = $update_info;
      if (isset($entity->{$field_name}[$language_code][$delta]['cid_unique'])
        && $entity->{$field_name}[$language_code][$delta]['cid_unique'] === $cid_unique) {
        // If cid is not yet set (i.e. it is a new calendar), cid will get its
        // value from availability_calendar_update_multiple_availability().
        $cid = availability_calendar_update_multiple_availability((int) $cid_unique, $changes);
        if ($cid != $entity->{$field_name}[$language_code][$delta]['cid']) {
          // New calendar: update the field.
          $entity->{$field_name}[$language_code][$delta]['cid'] = $cid;
        }
      }
    }
  }
}
