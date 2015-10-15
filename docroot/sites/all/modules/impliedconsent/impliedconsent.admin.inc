<?php
/**
 * @file
 * Administrative page callbacks for the Implied Consent module.
 */

/**
 * Implements hook_admin_settings().
 */
function impliedconsent_admin_settings_form($form_state) {

  // Page specific visibility configurations.
  $visibility = variable_get('impliedconsent_visibility_pages', 0);
  $pages = variable_get('impliedconsent_pages', IMPLIEDCONSENT_PAGES);

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notice settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -1,
  );

  $message = variable_get('impliedconsent_message', array('value' => IMPLIEDCONSENT_MESSAGE, 'format' => NULL));
  $form['content']['impliedconsent_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Message'),
    '#default_value' => $message['value'],
    '#format' => $message['format'],
  );
  $form['content']['impliedconsent_button_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Close button label'),
    '#default_value' => variable_get('impliedconsent_button_label', IMPLIEDCONSENT_BUTTON_LABEL),
  );
  $form['content']['impliedconsent_validate_by_click'] = array(
    '#type' => 'checkbox',
    '#title' => t('Agree automatically by using the site'),
    '#description' => t('If checked, consent is implied by clicking any form button or non-external link.'),
    '#default_value' => variable_get('impliedconsent_validate_by_click', IMPLIEDCONSENT_VALIDATE_BY_CLICK),
  );

  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'impliedconsent') . '/impliedconsent.admin.js',
      ),
    ),
  );

  $options = array(
    t('Every page except the listed pages'),
    t('The listed pages only'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
    array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
    )
  );

  $form['page_vis_settings']['impliedconsent_visibility_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Add the notice to specific pages'),
    '#options' => $options,
    '#default_value' => $visibility,
  );
  $form['page_vis_settings']['impliedconsent_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#title_display' => 'invisible',
    '#default_value' => $pages,
    '#description' => $description,
    '#rows' => 10,
  );

  // Render the role overview.
  $form['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#group' => 'settings',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['role_vis_settings']['impliedconsent_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add the notice for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('impliedconsent_visibility_roles', 0),
  );

  $role_options = array_map('check_plain', user_roles());
  $form['role_vis_settings']['impliedconsent_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('impliedconsent_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be notified. If a user has any of the roles checked, that user will be notified (or excluded, depending on the setting above).'),
  );

  // Visibility settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
  );
  $form['advanced']['impliedconsent_variant'] = array(
    '#type' => 'radios',
    '#title' => t('Plugin variant'),
    '#default_value' => variable_get('impliedconsent_variant', IMPLIEDCONSENT_VARIANT),
    '#options' => array(
      'minified' => t('Production (minified code)'),
      'source' => t('Development (uncompressed code)'),
    ),
    '#description' => t('Choose the plugin variant to include on pages. Uncompressed code may help with debugging.'),
  );
  $form['advanced']['impliedconsent_method'] = array(
    '#type' => 'radios',
    '#title' => t('Plugin loading method'),
    '#default_value' => variable_get('impliedconsent_method', IMPLIEDCONSENT_METHOD),
    '#options' => array(
      'drupal' => t('Standard Drupal'),
      'snippet' => t('Asynchronous snippet'),
    ),
    '#description' => t('Choose the method of loading the script. Standard Drupal uses libraries_load() while the asynchronous snippet is non-blocking by default.'),
  );
  $form['advanced']['impliedconsent_async'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skip autoloading'),
    '#default_value' => variable_get('impliedconsent_async', IMPLIEDCONSENT_ASYNC),
    '#description' => t('If checked, loading and initializing the library is omitted and needs to done manually. This option is for advanced implementations.'),
  );

  return system_settings_form($form);
}

/**
 * Implements _form_validate().
 */
function impliedconsent_admin_settings_form_validate($form, &$form_state) {
  // Trim text values.
  $form_state['values']['impliedconsent_pages'] = trim($form_state['values']['impliedconsent_pages']);
}
