<?php
/**
 * @file
/**
 * General helper methods for Availability Calendar Booking formlet to make the
 * .module file smaller:
 */

/**
 * Returns a customizable text.
 *
 * @param string $variable_name
 *
 * @return string
 */
function availability_calendar_booking_formlet_get_customizable_text($variable_name) {
  // 0 passes isset() and will differ from the empty string which is the actual
  // default in a number of cases.
  $text = 0;
  if (variable_get('availability_calendar_configurable_texts_enabled', 0) != 0
      && function_exists('variable_get_value')) {
    // Get from variable module, but do not get the default as that may have
    // been translated into another language then the current one.
    $text = variable_get_value($variable_name, array('default' => 0));
  }
  if ($text === 0) {
    // Get the *translated* default. By calling the hook_variable_info
    // function here, the defaults will be translated into the current language.
    module_load_include('inc', 'availability_calendar_booking_formlet', 'availability_calendar_booking_formlet.variable');
    $variable_info = availability_calendar_booking_formlet_variable_info();
    $text = $variable_info[$variable_name]['default'];
  }
  return $text;
}

function availability_calendar_booking_formlet_get_customizable_title($variable_name) {
  $title = availability_calendar_booking_formlet_get_customizable_text($variable_name);
  if ($title != '') {
    $title = " title='$title'";
  }
  return $title;
}

/***************************************************************
 * Field Type API hooks
 ***************************************************************/

/**
 * Implements hook_field_info().
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_info/7
 *
 * Provides the descriptions of the fields offered by this module.
 */
function availability_calendar_booking_formlet_field_info_inc() {
  module_load_include('inc', 'availability_calendar');
  $states = availability_calendar_get_states();
  if (!empty($states[4]) && $states[4]['css_class'] == 'cal-opt') {
    // State "Provisionally booked" exists, make this the default for the
    // "booked_state".
    $booked_state = 4;
  }
  else {
    $booked_state = NULL;
  }

  // Set default for "use get" to true if webform version 4 is used.
  $use_get = FALSE;
  if (module_exists('webform')) {
    $info = system_get_info('module', 'webform');
    $use_get = version_compare($info['version'], '7.x-4', '>=');
  }
  return array(
    'availability_calendar_booking_formlet' => array(
      'label' => t('Availability Calendar Booking formlet'),
      'description' => t('Allows to show a booking formlet for entities with an availability calendar.'),
      'settings' => array(
        'define_per_language' => 0,
        'post_url' => array(LANGUAGE_NONE => ''),
        'booked_state' => $booked_state,
        'use_get' => $use_get,
        //'allow_url_override' => FALSE,
      ),
      'instance_settings' => array(),
      'default_widget' => 'availability_calendar_booking_formlet',
      'default_formatter' => 'availability_calendar_booking_formlet',
  ),);
}

/**
 * Implements hook_field_settings_form().
 * @link http://api.drupal.org/api/drupal/modules--field_ui--field_ui.api.php/function/hook_field_settings_form/7
 */
function availability_calendar_booking_formlet_field_settings_form_inc($field/*, $instance, $has_data*/) {
  module_load_include('inc', 'availability_calendar');
  $settings = $field['settings'];
  $form = array();

  $form['url_2_continue'] = array(
    '#type' => 'markup',
    '#markup' => '<fieldset class="form-wrapper"><legend><span class="fieldset-legend">' . t("URL to continue the booking") . '</span></legend><div class="fieldset-wrapper">',
    '#weight' => 10,
  );

  $language_list = function_exists('locale_language_list') ? locale_language_list() : array();
  if (count($language_list) > 1) {
    $form['define_per_language'] = array(
      '#type' => 'checkbox',
      '#title' => t('Define the URL to continue the booking per language'),
      '#description' => t("Webforms are not language aware. Check this option if you want to continue the booking with a different URL (webform) per language."),
      '#default_value' => $settings['define_per_language'],
      '#required' => FALSE,
      '#weight' => 11,
    );
  }
  $form['post_url'] = array(
    '#type' => 'item',
    '#description' => t("These URLs typically point to a webform.") . ' ' . t("Do not start internal URLs with a '/'. You can use the system URL, it wil be replaced by its alias. See the help for more instructions about setting up the target page or webform."),
    '#suffix' => '</div></fieldset>',
    '#weight' => 12,
  );
  $form['post_url'][LANGUAGE_NONE] = array(
    '#type' => 'textfield',
    '#title' => t('All languages') . '/' . t('Language neutral'),
    '#default_value' => isset($settings['post_url'][LANGUAGE_NONE]) ? $settings['post_url'][LANGUAGE_NONE] : (is_array($settings['post_url']) ? reset($settings['post_url']) : ''),
    '#required' => FALSE,
    '#states' => array(
      // Use invisible as this field must also be visible when the checkbox does not exist.
      'invisible' => array(':input[name="field[settings][define_per_language]"]' => array('checked' => TRUE)),
    ),
  );
  if (count($language_list) > 1) {
    foreach ($language_list as $language_code => $language_name) {
      $form['post_url'][$language_code] = array(array(
        '#type' => 'textfield',
        '#title' => $language_name,
        '#default_value' => isset($settings['post_url'][$language_code]) ? $settings['post_url'][$language_code] : $settings['post_url'][LANGUAGE_NONE],
        '#required' => FALSE,
        // Use visible as these fields may only be visible when the the checkbox does exist.
        '#states' => array(
          'visible' => array(':input[name="field[settings][define_per_language]"]' => array('checked' => TRUE)),
        ),
      ));
    }
  }

  $form['booked_state'] = array(
    '#type' => 'select',
    '#title' => t('Change state to'),
    '#description' => t('The state to change the calendar to after the user has selected the arrival and departure dates. Note that the state is only visually changed in the browser, not stored.'),
    '#default_value' => $settings['booked_state'],
    '#options' => availability_calendar_get_states('label'),
    '#required' => TRUE,
    '#weight' => 13,
  );

  $form['use_get'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use GET (instead of POST)'),
    '#description' => t('Webform 7.x-4.x does not support %post tokens anymore, so check this option if the URL above is pointing to a webform and you have installed a 7.x-4.x version.'),
    '#default_value' => $settings['use_get'],
    '#required' => FALSE,
    '#weight' => 14,
  );

  return $form;
}

/**
 * Helper function to prevent duplication of these strings.
 */
function availability_calendar_booking_formlet_field_formatter_settings_form_get_options() {
  return array(
    '' => t('Do not display the duration'),
    'nights' => t('Display duration as number of nights'),
    'days' => t('Display duration as number of days'),
    'nights_and_days' => t('Display duration as number of nights / number + 1 days'),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 * @link http://api.drupal.org/api/drupal/modules--field_ui--field_ui.api.php/function/hook_field_formatter_settings_form/7
 */
function availability_calendar_booking_formlet_field_formatter_settings_form_inc($field, $instance, $view_mode/*, $form, &$form_state*/) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $name_prefix = 'fields[' . $field['field_name'] . '][settings_edit_form][settings]';

  $element = array();
  $element['submit_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only show the "Book now" button'),
    '#default_value' => $settings['submit_only'],
    '#description' => t('This will hide the date fields and their reset buttons, effectively disabling any interaction between the calendar, the user and this booking formlet. You should only check this option if the dates will be filled as per the settings below and you do not want the user to change the values anymore.'),
  );
  $element['single_day_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only allow single day bookings'),
    '#default_value' => $settings['single_day_only'],
    '#description' => t('This will hide the departure date field, turning the form into a single day booking formlet. You should check this option if you only accept single day bookings and want to optimize the UI for that.'),
  );
  $element['display_duration'] = array(
    '#type' => 'select',
    '#title' => t('Display number of days'),
    '#default_value' => $settings['display_duration'],
    '#options' => availability_calendar_booking_formlet_field_formatter_settings_form_get_options(),
    '#required' => FALSE,
  );
  $element['description'] = array(
    '#type' => 'item',
    '#description' => t('Below you can specify default values for the begin and end date. These can come from dynamic sources like $_GET or $_POST and may contain angle brackets ("[" and "]") to access an array value. Note that these values will only be set if an accompanying calendar is indeed "available". This option will typically be used in combination with a "view" that filters on availability.'),
  );
  $element['preset_begin_date_source'] = array(
    '#type' => 'select',
    '#title' => t('Source for the begin date'),
    '#default_value' => $settings['preset_begin_date_source'],
    '#options' => array(
      'none' => t('None'),
      'today' => t('Today'),
      'get' => t('GET value'),
      'post' => t('POST value'),
    ),
    '#required' => FALSE,
  );
  $name_begin = $name_prefix . '[preset_begin_date_source]';
  $element['preset_begin_date_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key for the begin date value'),
    '#default_value' => $settings['preset_begin_date_key'],
    '#required' => $settings['preset_begin_date_source'] === 'get' || $settings['preset_begin_date_source'] === 'post',
    '#size' => 36,
    '#states' => array(
      'visible'  => array(":input[name='$name_begin']" => array(array('value' => 'get'), array('value' => 'post'))),
      'required'  => array(":input[name='$name_begin']" => array(array('value' => 'get'), array('value' => 'post'))),
    ),
  );
  $element['preset_end_date_source'] = array(
    '#type' => 'select',
    '#title' => t('Source for the end date'),
    '#default_value' => isset($settings['preset_end_date_source']) ? $settings['preset_end_date_source'] : 'none',
    '#options' => array(
      'none' => t('None'),
      'today' => t('Today'),
      'fixed_duration' => t('Fixed duration'),
      'get' => t('GET value as end date'),
      'post' => t('POST value as end date'),
      'get1' => t('GET value as departure date'),
      'post1' => t('POST value as departure date'),
      'get_duration' => t('GET value as duration'),
      'post_duration' => t('POST value as duration'),
    ),
   '#states' => array(
      'invisible' => array(":input[name='$name_begin']" => array('value' => 'none')),
    ),
  );
  $name_end = $name_prefix . '[preset_end_date_source]';
  $element['preset_end_date_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key for the end date value or number of days'),
    '#default_value' => isset($settings['preset_end_date_key']) ? $settings['preset_end_date_key'] : '',
    '#required' => $settings['preset_end_date_source'] !== 'none' && $settings['preset_end_date_source'] !== 'today',
    '#size' => 36,
    '#states' => array(
      'invisible' => array(
        array(":input[name='$name_begin']" => array('value' => 'none')),
        array(":input[name='$name_end']" => array(array('value' => 'none'), array('value' => 'today'))),
      ),
      'optional' => array(":input[name='$name_end']" => array(array('value' => 'none'), array('value' => 'today'))),
    ),
  );

  $element['#attached']['css'] = array(
    drupal_get_path('module', 'availability_calendar_booking_formlet') . '/availability_calendar_booking_formlet.admin.css',
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 * @link http://api.drupal.org/api/drupal/modules--field_ui--field_ui.api.php/function/hook_field_formatter_settings_summary/7
 */
function availability_calendar_booking_formlet_field_formatter_settings_summary_inc($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = array();

  // Summary info about showing whole form or submit button only.
  $summary[] = $settings['submit_only'] ? t('Only show the submit button') : t('Show the complete booking formlet');

  // Summary info about the source of the begin date.
  switch ($settings['preset_begin_date_source']) {
    case 'today':
      $source = t('today');
      break;
    case 'get':
    case 'post':
      $key = availability_calendar_booking_formlet_format_key($settings['preset_begin_date_key']);
      $source = '$_' . strtoupper($settings['preset_begin_date_source']) . $key;
      break;
    case 'none':
    default:
      $source = t('blank');
      break;
  }
  $summary[] = t('Default begin date: @value', array('@value' => $source));

  // Summary info about the source of the end date.
  switch ($settings['preset_end_date_source']) {
    case 'get':
    case 'post':
      $key = availability_calendar_booking_formlet_format_key($settings['preset_end_date_key']);
      $source = '$_' . strtoupper($settings['preset_end_date_source']) . $key;
      $line = t('Default end date: @value', array('@value' => $source));
      break;
    case 'get1':
    case 'post1':
      $key = availability_calendar_booking_formlet_format_key($settings['preset_end_date_key']);
      $source = substr($settings['preset_end_date_source'], 0, strpos($settings['preset_end_date_source'], '1'));
      $source = '$_' . strtoupper($source) . $key;
      $line = t('Default departure date: @value', array('@value' => $source));
      break;
    case 'get_duration':
    case 'post_duration':
      $key = availability_calendar_booking_formlet_format_key($settings['preset_end_date_key']);
      $global = $settings['preset_end_date_source'] === 'get_duration' ? 'GET' : 'POST';
      $source = '$_' . $global . $key;
      $line = t('Default end date: begin + @value days', array('@value' => $source));
      break;
    case 'fixed_duration':
      $source = $settings['preset_end_date_key'];
      $line = t('Default end date: begin + @value days', array('@value' => $source));
      break;
    case 'none':
    default:
      $source = t('blank');
      $line = t('Default end date: @value', array('@value' => $source));
      break;
  }
  $summary[] = $line;

  $options = availability_calendar_booking_formlet_field_formatter_settings_form_get_options();
  $summary[] = $options[!empty($settings['display_duration']) ? $settings['display_duration'] : ''];

  return implode('<br/>', $summary);
}

/**
 * Helper function for the formatter summary to display what preset value will
 * be used.
 *
 * @param string $key
 *   The key, something like 'key1' or key1[subkey1][subkey2].
 *
 * @return string
 *   The key but with the first part also enclosed in angle brackets, thus
 *   something like [key1] or [key1][subkey1][subkey2].
 */
function availability_calendar_booking_formlet_format_key($key) {
  $key = check_plain($key);
  // Put angle brackets around (first part of) the key.
  $pos = strpos($key, '[');
  if ($pos === FALSE) {
    $key = "[$key]";
  }
  else {
    $key = '[' . substr($key, 0, $pos) . ']' . substr($key, $pos);
  }
  return $key;
}

/**
 * Implements hook_field_formatter_view().
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_formatter_view/7
 */
function availability_calendar_booking_formlet_field_formatter_view_inc($entity_type, $entity, $field, $instance, $langcode, &$items, $display) {
  module_load_include('inc', 'availability_calendar');

  $settings = $display['settings'] + $instance['settings'] + $field['settings'];

  // Get values for the begin and end date based on the field settings and
  // the context.
  $from = availability_calendar_booking_formlet_extract_date($settings, 'preset_begin_date');
  $to = $from !== NULL ? availability_calendar_booking_formlet_extract_date($settings, 'preset_end_date', $from) : NULL;
  // We need this function here only when from is set.
  if ($from !== NULL && !function_exists('availability_calendar_is_available')) {
    module_load_include('inc', 'availability_calendar');
  }

  // Find all calendars in this entity that are enabled. If there is no enabled
  // calendar, we do not show the booking form.
  //
  // We also look for the first calendar that is "available" during the from-to
  // period (or just on the from date if no to date is set). This calendar will
  // be used as default selection, together with the from and to date.
  $cids = array();
  $calendar = NULL;
  $calendar_settings = NULL;
  $field_instance_info = field_info_instances($entity_type, $instance['bundle']);
  foreach ($field_instance_info as $field_name => $field_instance) {
    $field_info = field_info_field_by_id($field_instance['field_id']);
    if ($field_info['type'] === 'availability_calendar') {
      $calendar_field_items = field_get_items($entity_type, $entity, $field_name, $langcode);
      if ($calendar_field_items !== FALSE) {
        foreach ($calendar_field_items as /*$delta =>*/ $calendar_field_item) {
          if (isset($calendar_field_item['enabled']) && $calendar_field_item['enabled']) {
            // An enabled calendar: add it to the list of calendars we serve.
            $cids[] = $calendar_field_item['cid'];
            // If it is the 1st calendar that is available in a preset period
            // ($from and $to) we select it as default.
            if ($from !== NULL && $calendar === NULL) {
              if (availability_calendar_is_available($calendar_field_item['cid'], $from, $to !== NULL ? $to : $from, $field_info['settings']['default_state'])) {
                $calendar = $calendar_field_item;
                $calendar_settings = $field_info['settings'];
              }
            }
          }
        }
      }
    }
  }

  $element = array();
  if (count($cids) > 0) {
    // $items will be empty because this is a display only field: fake 1 item.
    $delta = 0;
    $items[$delta] = array();

    // If we didn't find an available calendar we have to set the from and to
    // dates to null.
    if ($calendar === NULL) {
      $from = NULL;
      $to = NULL;
    }
    $element[$delta] = drupal_get_form('availability_calendar_booking_formlet_form', $entity_type, $entity, $settings, $cids, $calendar, $calendar_settings, $from, $to);
  }
  return $element;
}

/**
 * Callback for drupal_get_form() to create the booking formlet view.
 *
 * drupal_get_form() is called by hook_field_formatter_view (in our
 * case: availability_calendar_booking_formlet_field_formatter_view).
 *
 * @param string $form_id
 * @param array $form_state
 *
 * @return array
 */
function availability_calendar_booking_formlet_form_inc($form_id, $form_state) {
  static $formlet_count = 0;

  $formlet_count++;
  $entity_type = $form_state['build_info']['args'][0];
  $entity = $form_state['build_info']['args'][1];
  $entity_id = entity_extract_ids($entity_type, $entity);
  $entity_id = $entity_id[0];
  $entity_label = entity_label($entity_type, $entity);
  $settings = $form_state['build_info']['args'][2];
  $cids = $form_state['build_info']['args'][3];
  $calendar = $form_state['build_info']['args'][4];
  $calendar_settings = $form_state['build_info']['args'][5];
  /** @var DateTime */
  $from = $form_state['build_info']['args'][6];
  /** @var DateTime */
  $to = $form_state['build_info']['args'][7];
  /** @var integer */
  $duration = 0;
  if ($from instanceof DateTime && $to instanceof DateTime) {
    //PHP5.3: use getTimestamp() and/or diff().
    $timestamp_from = (int) $from->format('U');
    $timestamp_to = (int) $to->format('U');
    // To is inclusive, so add 1 day to the difference.
    $duration = (int) round(($timestamp_to - $timestamp_from)/(60*60*24)) + 1;
  }

  $form = array();
  $form['#id'] = "availability-calendar-booking-formlet-form-$formlet_count";
  $form['cid'] = array(
    '#type' => 'hidden',
    '#value' => $calendar !== NULL ? $calendar['cid'] : '',
  );
  $form['calendar_label'] = array(
    '#type' => 'hidden',
    '#value' => $calendar !== NULL ? $calendar['name'] : '',
  );
  $form['entity_type'] = array(
    '#type' => 'hidden',
    '#value' => $entity_type,
  );
  $form['entity_id'] = array(
    '#type' => 'hidden',
    '#value' => $entity_id,
  );
  $form['entity_label'] = array(
    '#type' => 'hidden',
    '#value' => $entity_label,
  );

  $form['from_iso'] = array(
    '#type' => 'hidden',
    '#value' => !empty($from) ? $from->format(AC_ISODATE) : '',
  );
  $form['to_iso'] = array(
    '#type' => 'hidden',
    '#value' => !empty($to) ? $to->format(AC_ISODATE) : '',
  );
  $form['duration'] = array(
    '#type' => 'hidden',
    '#value' => $duration,
  );
  if ($settings['submit_only']) {
    $form['from_display'] = array(
      '#type' => 'hidden',
      '#value' => $from instanceof DateTime ? availability_calendar_format_display_date($from) : '',
    );
    $form['to_display'] = array(
      '#type' => 'hidden',
      '#value' => $to instanceof DateTime ? availability_calendar_format_display_date($to) : '',
    );
  }
  else {
    $title = availability_calendar_booking_formlet_get_customizable_text('availability_calendar_booking_formlet_date_1');
    $button_text = availability_calendar_booking_formlet_get_customizable_text('availability_calendar_booking_formlet_clear_date_1_text');
    $button_title = availability_calendar_booking_formlet_get_customizable_title('availability_calendar_booking_formlet_clear_date_1_title');
    $form['from_display'] = array(
      '#type' => 'textfield',
      '#title' => $title,
      '#default_value' => $from instanceof DateTime ? availability_calendar_format_display_date($from) : '',
      '#required' => TRUE,
      '#disabled' => TRUE,
      '#attributes' => array('readonly' => 'readonly'),
      '#prefix' => '<div class="acbf-date">',
      '#suffix' => '<input class="acbf-reset-from form-reset"' . $button_title . ' type="reset" value="' . $button_text . '" /></div>',
    );
    if ($settings['single_day_only']) {
      $form['to_display'] = array(
        '#type' => 'hidden',
        '#value' => $to instanceof DateTime ? availability_calendar_format_display_date($to) : '',
      );
      $title = availability_calendar_booking_formlet_get_customizable_text('availability_calendar_booking_formlet_date');
      $button_text = availability_calendar_booking_formlet_get_customizable_text('availability_calendar_booking_formlet_clear_date_text');
      $button_title = availability_calendar_booking_formlet_get_customizable_title('availability_calendar_booking_formlet_clear_date_title');
      $form['from_display']['#title'] = $title;
      $form['from_display']['#suffix'] = '<input class="acbf-reset-from form-reset"' . $button_title . ' type="reset" value="' . $button_text . '" /></div>';
    }
    else {
      $title = availability_calendar_booking_formlet_get_customizable_text('availability_calendar_booking_formlet_date_2');
      $button_text = availability_calendar_booking_formlet_get_customizable_text('availability_calendar_booking_formlet_clear_date_2_text');
      $button_title = availability_calendar_booking_formlet_get_customizable_title('availability_calendar_booking_formlet_clear_date_2_title');
      $form['to_display'] = array(
        '#type' => 'textfield',
        '#title' => $title,
        '#default_value' => $to instanceof DateTime ? availability_calendar_format_display_date($to) : '',
        '#required' => TRUE,
        '#disabled' => TRUE,
        '#attributes' => array('readonly' => 'readonly'),
        '#prefix' => '<div class="acbf-date">',
        '#suffix' => '<input class="acbf-reset-both form-reset"' . $button_title . ' type="reset" value="' . $button_text . '" /></div>',
      );
      // Display the duration if the settings indicate so.
      if (!empty($settings['display_duration'])) {
        $duration_display = '';
        switch ($settings['display_duration']) {
          case 'days':
            $duration_display = format_plural($duration, '1 day', '@count days');
            break;
          case 'nights':
            $duration_display = format_plural($duration, '1 night', '@count nights');
            break;
          case 'nights_and_days':
            $duration_display = format_plural($duration, '1 night', '@count nights');
            $duration_display .= ' / ';
            $duration_display .= format_plural($duration + 1, '1 day', '@count days');
            break;
        }
        $title = availability_calendar_booking_formlet_get_customizable_text('availability_calendar_booking_formlet_duration');
        $form['duration_display'] = array(
          '#type' => 'textfield',
          '#title' => $title,
          '#default_value' => $duration_display,
          '#required' => TRUE,
          '#disabled' => TRUE,
          '#attributes' => array('readonly' => 'readonly'),
          '#prefix' => '<div class="acbf-date">',
          '#suffix' => '</div>',
        );
      }
    }
  }

  // Add form submit button.
  $button_text = availability_calendar_booking_formlet_get_customizable_text('availability_calendar_booking_formlet_submit_text');
  $button_title = availability_calendar_booking_formlet_get_customizable_text('availability_calendar_booking_formlet_submit_title');
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => $button_text,
  		'#disabled' => TRUE,
    ),
  );
  if ($button_title != '') {
    $form['actions']['submit']['#attributes']['title'] = $button_title;

  }

  global $language;
  $post_url = '';
  if ($settings['define_per_language'] && isset($settings['post_url'][$language->language])) {
    $post_url = $settings['post_url'][$language->language];
  }
  else if (isset($settings['post_url'][LANGUAGE_NONE])) {
    $post_url = $settings['post_url'][LANGUAGE_NONE];
  }
  else if (is_array($settings['post_url'])) {
    $post_url = reset($settings['post_url']);
  }
  if (is_array($post_url)) {
    $post_url = reset($post_url);
  }
  $form['#action'] = url($post_url);
  $form['#method'] = !empty($settings['use_get']) ? 'GET' : 'POST';

  // Add js and css.
  // If only the book now button is shown, the calendar might not be shown.
  // Make sure there is a javascript instance that represents the selected
  // calendar (and that can be queried by our javascript).
  if ($calendar !== NULL) {
    availability_calendar_add_calendar_js($calendar['cid'], $calendar_settings['allocation_type']);
  }
  availability_calendar_booking_formlet_add_js($form['#id'], $cids, $settings);

  $form['#attached']['css'] = array(drupal_get_path('module', 'availability_calendar_booking_formlet') . '/availability_calendar_booking_formlet.css');

  return $form;
}

/**
 * Returns a date based on the settings and global context.
 *
 * @param array $settings
 * @param string $date
 *   Either 'preset_begin_date' or 'preset_end_date'
 * @param null|DateTime $offset
 *   null or a date to use as offset if the source indicates a duration rather
 *   than a date on its own.
 *
 * @return null|DateTime
 */
function availability_calendar_booking_formlet_extract_date($settings, $date, $offset = NULL) {
  $source = $settings["{$date}_source"];
  $global = substr($source, 0, strlen('get')) === 'get' ? $_GET : $_POST;
  $key = $settings["{$date}_key"];
  $result = NULL;
  switch ($source) {
    case 'today':
      $result = new DateTime();
      break;
    case 'fixed_duration':
      if ($offset instanceof DateTime) {
        $result = clone ($offset);
        $duration = (int) $key;
        // Adjust duration as to date is inclusive.
        $duration--;
        $result->modify("+$duration days");
      }
      break;
    case 'get':
    case 'post':
    case 'get1':
    case 'post1':
      $date = availability_calendar_booking_formlet_get_nested_array_value($global, $key);
      if (is_string($date)) {
        $date = availability_calendar_parse_entry_date($date);
        if ($date instanceof DateTime) {
          $result = $date;
          // get1 and post1 are not inclusive: modify by 1 day.
          if (substr($source, -strlen('1')) === '1') {
            $result->modify('-1 day');
          }
        }
      }
      break;
    case 'get_duration':
    case 'post_duration':
      if ($offset instanceof DateTime) {
        $duration = availability_calendar_booking_formlet_get_nested_array_value($global, $key);
        if (is_int($duration) || ctype_digit($duration)) {
          $result = clone ($offset);
          $duration = (int) $duration;
          // Adjust duration as to date is inclusive.
          $duration--;
          $result->modify("+$duration days");
        }
      }
      break;
    case 'none':
    default:
      break;
  }

  // Set time to noon to avoid timezone conversion rounding errors later.
  if ($result instanceof DateTime) {
    $result->setTime(12, 0, 0);
  }
  return $result;
}


/**
 * Extracts a possibly multi-level key from an array.
 *
 * @param array $array
 *   The array to extract the value from.
 * @param string $key
 *   The key, something like 'key1' or key1[subkey1][subkey2].
 *
 * @return null|mixed
 *   Null when the key does not exist, the value of the array entry otherwise.
 */
function availability_calendar_booking_formlet_get_nested_array_value($array, $key) {
  $keys = explode('[', str_replace(']', '', $key));
  return drupal_array_get_nested_value($array, $keys);
}

/**
 * Adds the javascript for the booking formlet to the page.
 *
 * @param string $form_id
 * @param array $cids
 * @param array $settings
 */
function availability_calendar_booking_formlet_add_js($form_id, $cids, $settings) {
  // Ensure that the base client side API loads as well including, optionally,
  // the date formatting functionality of date picker.
  availability_calendar_add_base_js();
  drupal_add_js(drupal_get_path('module', 'availability_calendar_booking_formlet') . '/availability_calendar_booking_formlet.js',
    array('type' => 'file'));
  drupal_add_js(array(
    'availabilityCalendar' => array(
      'bookingFormlets' => array(
        $form_id => array(
          'formId' => "#$form_id",
          'cids' => $cids,
          'bookedState' => $settings['booked_state'],
          'singleDay' => (bool) $settings['single_day_only'],
          'displayDuration' => $settings['display_duration'],
    )))), array('type' => 'setting'));
}
