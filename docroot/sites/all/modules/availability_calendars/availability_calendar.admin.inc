<?php
/**
 * Availability Calendar admin settings form code:
 * - define form
 * - validate
 * - submit
 */

module_load_include('inc', 'availability_calendar');

/**
 * Defines form callback for the admin/config/availability-calendar/settings
 * page.
 *
 * @param array $form
 *   The form.
 *
 * @return array
 *   The form.
 */
function availability_calendar_admin_settings($form) {
  drupal_add_css(drupal_get_path('module', 'availability_calendar') . '/availability_calendar.admin.css');
  $form['#validate'][] = 'availability_calendar_admin_settings_validate';
  $form['#submit'][] = 'availability_calendar_admin_settings_submit';

  // Information about date formatting (including links).
  $form['date_formatting'] = array(
    '#type' => 'item',
    '#title' => t('Date formats'),
    '#description' => t('<p class="no-space">Availability calendar defines 3 date types:</p>
<ul>
<li>@datetype1: Used to display dates to users. This is on the booking formlet.</li>
<li>@datetype2: Used in places where users can input dates. This is in the Views exposed filter when searching on availability.</li>
<li>@datetype3: Used as caption above a month when the calendar is being displayed.</li>
  </ul>
<p>You can change the formats of these date types at !link1. If you want different date formats per language, you can localize the date types at !link2. Note that you should only use date parts, no time parts.</p>',
      array(
        '@datetype1' => t('Availability Calendar date display'),
        '@datetype2' => t('Availability Calendar date entry'),
        '@datetype3' => t('Availability Calendar month caption'),
        '!link1' => l(t('Date and time'), 'admin/config/regional/date-time'),
        '!link2' => l(t('Date and time') . ' - ' . t('Localize'), 'admin/config/regional/date-time/locale'))),
  );

  $sentence1 = t('Check this field if you want to customize user facing texts. The !variable-project (including the Variable admin sub module) must be enabled to use this feature.',
    array('!variable-project' => l(t('Variable project'), 'https://www.drupal.org/project/variable')));
  $sentence2 = t('If you enable this feature, you can customize the texts on the !variable-admin-ui.',
    array('!variable-admin-ui' => l(t('Variable admin UI page'), 'admin/config/system/variable/group/availability_calendar')));
  $sentence3 = t('To translate these texts, the i18n_variable module from the !i18n-project must be enabled.
    If that module is enabled, you can mark the variables as multilingual on the !variable-i18n and translate them on !variable-realm-language.',
    array('!i18n-project' => l(t('Internationalization project'), 'https://www.drupal.org/project/i18n'),
          '!variable-i18n' => l(t('Multilingual settings - Variable page'), 'admin/config/regional/i18n/variable'),
          '!variable-realm-language' => l(t('Variable - Realms - Language'), 'admin/config/system/variable/realm/language/edit')));

  $form['availability_calendar_configurable_texts_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable custom texts for the availability calendar'),
    '#description' => "$sentence1 $sentence2 $sentence3",
    '#disabled' => !module_exists('variable_admin'),
    '#default_value' => variable_get('availability_calendar_configurable_texts_enabled', 0),
  );

  // 'availability_calendar_months to_index'
  $form['availability_calendar_months_to_index'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of months to index'),
    '#default_value' => variable_get('availability_calendar_months_to_index', 13),
    '#description' => t('Number of months to index when indexing availability for Search API indices.'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#size' => 4,
  );

  // Add states.
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('States'),
    '#description' => t('<p>You can modify the availability states here.</p>
      <ul>
        <li>The label is what visitors will see in the legend and what editors will see when editing the calendar.</li>
        <li>The class should be unique and will be used for the css.</li>
        <li>The "Treat as available" checkbox defines whether this state should be treated as available in searches for availability.</li>
        <li>The weight defines the order in the legend.</li>
        <li>Make a label empty to remove the row.</li>
        <li>If there are no more empty lines to add new states, save the form and you will be able to add another state.</li>
        <li>If you change or define new classes, visit the styling page to define its colors.</li>
      </ul>
    '),
  );

  // Create a draggable table
  $header = array(t('Id'), t('Label'), t('CSS Class'), t('Treat as available?'), t('Weight'));
  $form['states'] = array(
    '#type' => 'markup',
    '#tree' => TRUE,
    '#theme' => 'table',
    '#pre_render' => array('availability_calendar_admin_settings_pre_render'),
    '#header' => $header,
    '#attributes' => array('id' => 'state-list'),
  );

  $i = 0;
  foreach (availability_calendar_get_states() as $state) {
    $form['states'][$i++] = availability_calendar_admin_settings_add_state($state);
  }

  // Show a minimum of 6 available states with at least one empty state.
  do {
    $form['states'][$i++] = availability_calendar_admin_settings_add_state(
        array('sid' => 0, 'label' => '', 'css_class' => '', 'is_available' => 0, 'weight' => 0));
  } while ($i < 6);

  drupal_add_tabledrag('state-list', 'order', 'sibling', 'state-weight');

  return system_settings_form($form);
}

/**
 * Helper function to add a state item to a form.
 * Only the first item gets labels, as the fields will be presented below each other.
 *
 * @param array $state
 *   Array containing a state record.
 *
 * @return array
 *   A form element with children only.
 */
function availability_calendar_admin_settings_add_state($state) {
  static $max_weight = 0;
  $element = array();
  $element['sid'] = array(
    '#type' => 'hidden',
    '#value' => $state['sid'],
  );
  $element['label'] = array(
    '#type' => 'textfield',
    '#default_value' => $state['label'],
    '#size' => 40,
  );
  $element['css_class'] = array(
    '#type' => 'textfield',
    '#default_value' => $state['css_class'],
    '#size' => 24,
  );
  $element['is_available'] = array(
    '#type' => 'checkbox',
    '#title_display' => 'before',
    '#default_value' => $state['is_available'],
  );
  $element['weight'] = array(
    '#type' => 'weight',
    '#default_value' => $state['weight'] > 0 ? $state['weight'] : ++$max_weight,
    '#attributes' => array('class' => array('state-weight')),
  );
  if ($state['weight'] > $max_weight) {
    $max_weight = $state['weight'];
  }
  return $element;
}

/**
 * Use of tables in a form.
 *
 * The children of the elements are the rows.
 * The children of the children are the cells.
 *
 * @param array $element
 *   Form element that will be themed as a table.
 *
 * @return array
 *   The changed form element.
 */
function availability_calendar_admin_settings_pre_render($element) {
  $rows = array();
  foreach (element_children($element) as $row_key) {
    $row = array();
    foreach (element_children($element[$row_key]) as $cell_key) {
      $row[$cell_key] = array('data' => $element[$row_key][$cell_key]);
    }
    $rows[$row_key] = array('data' => $row, 'class' => array('draggable'));
    unset($element[$row_key]);
  }
  $element['#rows'] = $rows;
  return $element;
}

/**
 * Form validate callback for the admin_settings form.
 * - At least one label should be filled.
 * - Entered classes should be a valid css class.
 *
 * @param array $form
 * @param array $form_state
 */
function availability_calendar_admin_settings_validate($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Save configuration')) {
    $element = $form_state['values']['states'];
    $all_empty = TRUE;
    foreach ($element as $i => $state_fields) {
      if (!empty($state_fields['label'])) {
        $all_empty = FALSE;
      }
      if (!empty($state_fields['css_class']) && $state_fields['css_class'] !== drupal_clean_css_identifier($state_fields['css_class'])) {
        form_set_error("states][$i][css_class", t('CSS class should be usable as a valid css class.'));
      }
    }

    if ($all_empty) {
      form_set_error('states][0][label', t('At least 1 state should be defined.'));
    }
  }
}

/**
 * Form submit callback for the admin_settings form.
 *
 * Processes the submitted form. The states are handled here.
 *
 * @param array $form
 * @param array $form_state
 */
function availability_calendar_admin_settings_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Save configuration')) {
    // Process the state settings here and do not pass them to the default
    // submit handler for system settings forms.
    $element = $form_state['values']['states'];
    unset($form_state['values']['states']);

    // Extract the states.
    $states = array();
    foreach ($element as $state_fields) {
      // Only add non-empty labels.
      if (!empty($state_fields['label'])) {
        if (empty($state_fields['css_class'])) {
          $state_fields['css_class'] = drupal_clean_css_identifier("cal-{$state_fields['label']}");
        }
        $states[] = array(
          'sid' => !empty($state_fields['sid']) ? (int) $state_fields['sid'] : NULL,
          'css_class' => $state_fields['css_class'],
          'label' => $state_fields['label'],
          'weight' => $state_fields['weight'],
          'is_available' => $state_fields['is_available'],
        );
      }
    }

    // Finally, save the states.
    availability_calendar_update_states($states);

    unset($form_state['values']['date_formatting']);
  }
}
