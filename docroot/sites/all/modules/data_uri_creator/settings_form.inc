<?php
/**
 * @file
 * Contains the administration form of the Data URI Creator module.
 */

/**
 * Creates an administrative form to configure the module's general settings.
 *
 * @return array
 *   An array representing the form definition.
 *
 * @ingroup forms
 * @see data_uri_creator_settings_form_submit()
 * @see system_settings_form_submit()
 */
function data_uri_creator_settings_form() {
  $form = array();
  $form['creator_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data URI Creator page'),
    '#description' => t(
      'Configure the options for the !link page.',
      array(
        '!link' => l(
          t('Data URI Creator'),
          DATA_URI_CREATOR_PAGE_PATH,
          array('attributes' => array('target' => '_blank'))
        ),
      )
    ),
  );

  $form['creator_page']['data_uri_creator_max_file_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum data size'),
    '#description' => t('Specify the maximum allowed byte-size for data uploads. To impose no limit, leave the value empty.'),
    '#default_value' => DataUriCreatorSettings::getMaximumFileSize(),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $url_options = array(
    'fragment' => 'module-data_uri_creator',
  );
  $form['creator_page']['data_uri_creator_page_public'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make the page publicly available.'),
    '#default_value' => (DataUriCreatorSettings::getPagePublicFlag() ? 1 : 0),
    '#description' => t(
      'To grant access more granularly to specific users, set the !permissions for the Data URI Creator instead.',
      array(
        '!permissions' => l(t('permissions'), 'admin/people/permissions', $url_options),
      )
    ),
  );

  // Register an extra submission handler; plus system_settings_form_submit().
  $form['#submit'][] = 'data_uri_creator_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Additional form submission handler for data_uri_creator_settings_form().
 *
 * It gets called once the settings have been validated successfully, and
 * this function will not be called if any of the submitted data was invalid.
 *
 * @param array $form
 *   The form's current render-array.
 * @param array $form_state
 *   Reference to the current form state.
 *
 * @see data_uri_creator_settings_form()
 * @see system_settings_form_submit()
 */
function data_uri_creator_settings_form_submit($form, &$form_state) {
  // Save the public-access flag as a permission setting.  Note that the
  // default system_settings_form_submit() handler will also save its
  // corresponding data_uri_creator_page_public variable value, such
  // that the stray variable still needs to be deleted during uninstall.
  // Furthermore, DataUriCreatorSettings::setMaximumFileSize() is not called
  // here, since the default submission call to system_settings_form_submit()
  // will persist the data_uri_creator_max_file_size value sufficiently.
  DataUriCreatorSettings::setPagePublicFlag(
    $form_state['values']['data_uri_creator_page_public']);
}
