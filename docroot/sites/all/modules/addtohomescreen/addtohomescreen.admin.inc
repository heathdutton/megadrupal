<?php
/**
 * @file
 * Settings form for Add to homescreen
 */

/**
 * Form constructor for admin/config/user-interface/addtohomescreen.
 */
function addtohomescreen_settings($form, $form_state) {
  $form['library'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#description' => t('For more information about these options, visit <a href="@addtohomescreen_url">Add to homescreen on Github</a>.',
      array('@addtohomescreen_url' => 'https://github.com/cubiq/add-to-homescreen')
    ),
  );
  $form['library']['addtohomescreen_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('Some of the preliminary checks are skipped and the message is shown on desktop browsers and unsupported devices as well.'),
    '#default_value' => variable_get('addtohomescreen_debug', FALSE),
  );
  $form['library']['addtohomescreen_modal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Modal'),
    '#description' => t('Prevents further actions on the website until the message is closed.'),
    '#default_value' => variable_get('addtohomescreen_modal', FALSE),
  );
  $form['library']['addtohomescreen_mandatory'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mandatory'),
    '#description' => t('The website is not accessible until the user adds the website to the homescreen.'),
    '#default_value' => variable_get('addtohomescreen_mandatory', FALSE),
  );
  $form['library']['addtohomescreen_skipfirstvisit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skip first visit'),
    '#description' => t('Prevent the message from appearing the first time the user visits your website. It is highly recommended to enable this option!'),
    '#default_value' => variable_get('addtohomescreen_skipfirstvisit', FALSE),
  );
  $form['library']['addtohomescreen_autostart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autostart'),
    '#description' => t('The message is not shown automatically and you have to trigger it programmatically.'),
    '#default_value' => variable_get('addtohomescreen_autostart', TRUE),
  );
  $form['library']['addtohomescreen_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Icon'),
    '#description' => t('Display the touch icon in the pop up message.'),
    '#default_value' => variable_get('addtohomescreen_icon', TRUE),
  );
  $form['library']['addtohomescreen_startdelay'] = array(
    '#type' => 'textfield',
    '#title' => t('Start delay'),
    '#description' => t('Seconds to wait from page load before showing the message.'),
    '#default_value' => variable_get('addtohomescreen_startdelay', 1),
    '#size' => 10,
  );
  $form['library']['addtohomescreen_lifespan'] = array(
    '#type' => 'textfield',
    '#title' => t('Lifespan'),
    '#description' => t('Seconds to wait before automatically closing the message. Set to 0 to disable automatic removal.'),
    '#default_value' => variable_get('addtohomescreen_lifespan', 15),
    '#size' => 10,
  );
  $form['library']['addtohomescreen_displaypace'] = array(
    '#type' => 'textfield',
    '#title' => t('Display pace'),
    '#description' => t('Minutes before the message is shown again. By default it is set to 1440, meaning the message is shown once per day.'),
    '#default_value' => variable_get('addtohomescreen_displaypace', 1440),
    '#size' => 10,
  );
  $form['library']['addtohomescreen_maxdisplaycount'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum display count'),
    '#description' => t('Absolute maximum number of times the call out will be shown. Set to 0 for no maximum.'),
    '#default_value' => variable_get('addtohomescreen_maxdisplaycount', 1),
    '#size' => 10,
  );
  $form['library']['addtohomescreen_use_custom_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a custom message'),
    '#description' => t('Add to homescreen comes with a localized and device specific message. You can override this message with your own.'),
    '#default_value' => variable_get('addtohomescreen_use_custom_message', FALSE),
  );
  $form['library']['addtohomescreen_message'] = array(
    '#title' => t('Message'),
    '#type' => 'textarea',
    '#default_value' => variable_get('addtohomescreen_message', t('To add this web app to the home screen: tap %icon and then <strong>Add to homescreen</strong>.')),
    '#description' => t('Available replacements: %icon'),
    '#states' => array(
      'disabled' => array(
        ':input[name="addtohomescreen_use_custom_message"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['compression_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Library compression settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['compression_settings']['addtohomescreen_compression_type'] = array(
    '#type' => 'radios',
    '#title' => t('Choose compression level'),
    '#options' => array(
      'minified' => t('Production (Minified)'),
      'source' => t('Development (Uncompressed Code)'),
    ),
    '#default_value' => variable_get('addtohomescreen_compression_type', 'minified'),
  );

  return system_settings_form($form);
}
