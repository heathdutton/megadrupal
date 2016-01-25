<?php
/**
 * @file
 * The node_statistics.admin.module
 */

/**
 * Form constructor for the node_statistics_article_metrics settings form.
 *
 * @see hook_admin_settings_validate()
 *
 * @ingroup forms
 */

function advance_clock_admin_settings() {
  $form = array();
  $form['advance_clock_count'] = array(
  	'#type' => 'select',
    '#title' => t('Number of allowed clock per user'),
    '#options' => array(
	    1 => t('1'),
      2 => t('2'),
      3 => t('3'),
      4 => t('4'),
      5 => t('5'),
      6 => t('6'),
    ),
    '#default_value' => variable_get('advance_clock_count'),
    '#description' => t('Allowed clock for user.'),
    '#required' => TRUE,
  );

  $form['advance_clock_show_secs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Seconds'),
    '#default_value' => variable_get('advance_clock_show_secs', TRUE),
    '#description' => t('Show Seconds Counter with simple digital clock'),
  );

  $form['advance_clock_show_only_city'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Only City Name'),
    '#default_value' => variable_get('advance_clock_show_only_city', FALSE),
    '#description' => t('Show only City Name in clock\'s title. NOTE: Works only with Simple Digital Clock'),
  );

  $form['advance_clock_hour24'] = array(
    '#type' => 'checkbox',
    '#title' => t('24 hr\'s Clock.'),
    '#default_value' => variable_get('advance_clock_hour24', FALSE),
    '#description' => t('The Clock Type, Default is 12Hr Clock'),
  );

  $form['advance_clock_show_date'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Date'),
    '#default_value' => variable_get('advance_clock_show_date', FALSE),
  );

  $form['advance_clock_simple_advance'] = array(
    '#type' => 'radios',
    '#title' => t('Select your preference'),
    '#options' => array(
      0 => t('Advance Digital Clock (jQuery 1.5 or above (Stable).)'),
      1 => t('Advance Digital/Analog Clock (jQuery 1.9 or above)'),
    ),
    '#default_value' => variable_get('advance_clock_simple_advance', FALSE),
    '#description' => t('Set this to <em>Yes</em> if you would like this category to be selected by default.'),
  );

  // jQuery 1.5
  $form['advance_clock_digital_jq15'] = array(
    '#type' => 'checkbox',
    '#title' => t('Simple Digital Clock.'),
    '#description' => t('Uses  jQuery 1.5 or above (Stable).'),
    '#default_value' => variable_get('advance_clock_digital_jq15', FALSE),
    '#states' => array(
      'visible' => array(
        ':input[name="advance_clock_simple_advance"]' => array('checked' => TRUE),
      ),
    ),
  );

  // @TODO: Disable these options if "advance_clock_digital_jq15" is used.
  $form['advance_clock_digital'] = array(
    '#type' => 'checkbox',
    '#title' => t('Digital Clock.'),
    '#description' => t('Uses jQuery 1.10.2.'),
    '#states' => array(
      'visible' => array(
        ':input[name="advance_clock_simple_advance"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => variable_get('advance_clock_digital', FALSE),
  );

  $form['advance_clock_analog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Analog Clock.'),
    '#default_value' => variable_get('advance_clock_analog', FALSE),
    '#description' => t('Uses jQuery 1.10.2. (Not Reommended!)'),
    '#states' => array(
      'visible' => array(
        ':input[name="advance_clock_simple_advance"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['#validate'][] = 'advance_clock_admin_settings_validate';
  return system_settings_form($form);
}

/**
 * Form validation handler for hook_admin_settings().
 */
function advance_clock_admin_settings_validate($form, &$form_state) {
  // Nothing
}
