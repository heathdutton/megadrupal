<?php

/**
 * indic_script Admin Settings Form.
 */
function indic_script_admin_settings_form() {


  $form_fields = _indic_script_languages();
  //$options['en'] = t('English');
  foreach ($form_fields as $field) {
    $options[$field['key']] = $field['title'];
  }

  $form['indic_script_langs'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Languages and typing metods'),
    '#default_value' => variable_get('indic_script_langs', array('ta_en' => t('Tamil Thanglish'))),
    '#options' => $options,
    '#multiple' => TRUE,
    '#description' => t('Choose the Languages and typing metods you need to available to Site Users.'),
  );

  $form['indic_script_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $access = user_access('use PHP for block visibility');
  $options = array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.'));
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
  if ($access) {
      $options[] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  $form['indic_script_settings']['indic_script_page_excl_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Page inclusion or exclusion mode'),
    '#default_value' => variable_get('indic_script_page_excl_mode', 0),
    '#options' => array('0' => t('All pages expect the following pages'), '1' => t('Only the following pages')),
    '#description' => t('Choose the way of disabling/enabling Indic Script on selected paths'),
  );
  /**
   * get excluded paths - so we can have normal textareas too
   * split the phrase by any number of commas or space characters,
   * which include " ", \r, \t, \n and \f
   */
  $form['indic_script_settings']['indic_script_excl_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths to exclude/include'),
    '#cols' => 60,
    '#rows' => 5,
    '#default_value' =>variable_get('indic_script_excl_paths', ''),
    '#description' => $description,
  );


  $form['indic_script_settings']['indic_script_excl_mode'] = array(
    '#type' => 'select',
    '#title' => t('Use inclusion or exclusion mode'),
    '#default_value' => variable_get('indic_script_excl_mode', 0),
    '#options' => array('0' => t('exclude'), '1' => t('include')),
    '#description' => t('Choose the way of disabling/enabling Indic Script on selected fields/paths (see below). Use exclude to disable Indic Script on selected fields/paths. Use include if you want to load Indic Script only on selected paths/fields.'),
  );

  /**
   * get excluded fields - so we can have normal textareas too
   * split the phrase by any number of commas or space characters,
   * which include " ", \r, \t, \n and \f
   */
  $form['indic_script_settings']['indic_script_excl_fields'] = array(
    '#type' => 'textarea',
    '#title' => t('Fields to exclude/include'),
    '#cols' => 60,
    '#rows' => 5,
    '#default_value' => variable_get('indic_script_excl_fields', ''),
    '#description' => t("Enter names (HTML ID's) of fields that may or may not have an Indic Script, depending on the chosen option for the inclusion/exclusion mode.<br />You may separate the different entries by commas, spaces or newlines."),
  );

  // demo page switch
  $form['indic_script_settings']['indic_script_enable_demo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Tamil Typing Demo Page?'),
    '#options' => array(0 => t('Disable'), 1 => t('Enable')),
    '#default_value' => variable_get('indic_script_enable_demo', 1),
    '#description' => t("If checked, the new page will be available at path tamil_type."),
  );
  return system_settings_form($form);
}
