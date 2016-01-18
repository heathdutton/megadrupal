<?php

/**
* @file
* The module settings form.
*/

function bu_admin_settings() {
  $form = array();


  $form['bu_browsers']['bu_browsers_firefox'] = array(
    '#type' => 'select',
    '#title' => t('Firefox'),
    '#options' => bu_get_versions('firefox'),
    '#default_value' => variable_get('bu_browsers_firefox', '3.5'),
  );
  $form['bu_browsers']['bu_browsers_ie'] = array(
    '#type' => 'select',
    '#title' => t('Internet Explorer'),
    '#options' => bu_get_versions('ie'),
    '#default_value' => variable_get('bu_browsers_ie', '7'),
  );
  $form['bu_browsers']['bu_browsers_opera'] = array(
    '#type' => 'select',
    '#title' => t('Opera'),
    '#options' => bu_get_versions('opera'),
    '#default_value' => variable_get('bu_browsers_opera', '10.6'),
  );
  $form['bu_browsers']['bu_browsers_safari'] = array(
    '#type' => 'select',
    '#title' => t('Safari'),
    '#options' => bu_get_versions('safari'),
    '#default_value' => variable_get('bu_browsers_safari', '3.2'),
  );

  $form['bu_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Settings',
    '#description' => 'Additional settings to change behavior of browser update script.',
  );
  $form['bu_settings']['bu_settings_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom message bar text'),
    '#default_value' => variable_get('bu_settings_text', ''),
  );
  $form['bu_settings']['bu_settings_reminder'] = array(
    '#type' => 'select',
    '#title' => t('Remind every x hours'),
    '#description' => 'If 0 the message bar will show all the time.',
    '#options' => drupal_map_assoc(array(0, 1, 2, 4, 6, 12, 24, 36, 48)),
    '#default_value' => variable_get('bu_settings_reminder', 3),
  );
  $form['bu_settings']['bu_settings_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug'),
    '#description' => 'Always show the message bar.',
    '#default_value' => variable_get('bu_settings_debug', 0),
  );
  $form['bu_settings']['bu_settings_blank'] = array(
    '#type' => 'checkbox',
    '#title' => t('New window'),
    '#description' => 'Should link show up in a new windows/tab.',
    '#default_value' => variable_get('bu_settings_blank', 0),
  );

  $access = user_access('use PHP for settings');
  $options = array(
    BU_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    BU_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  if (module_exists('php') && $access) {
    $options += array(BU_VISIBILITY_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
    $title = t('Pages or PHP code');
    $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  else {
    $title = t('Pages');
  }
  $form['bu_settings']['bu_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Load Browser update script on specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('bu_visibility', BU_VISIBILITY_NOTLISTED),
  );
  $form['bu_settings']['bu_pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">' . $title . '</span>',
    '#default_value' => variable_get('bu_pages', ''),
    '#description' => $description,
  );
  return system_settings_form($form);
}
