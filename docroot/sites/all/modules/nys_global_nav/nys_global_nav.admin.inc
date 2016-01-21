<?php

/**
 * @file
 * New York State Global Navigation module configuration page.
 *
 * Functions that are only called on the admin pages.  Separating
 * from .module file keeps them from being loaded with every page.
 */

/**
 * NYS Global Navigation module settings form.
 */
function nys_global_nav_admin_settings() {
  $form['nys_global_nav_agency_fieldset'] = _nys_global_nav_agency_fieldset_field();
  $form['nys_global_nav_agency_fieldset']['nys_global_nav_agency_name'] = _nys_global_nav_agency_name_field();
  $form['nys_global_nav_agency_fieldset']['nys_global_nav_agency_color'] = _nys_global_nav_agency_color_field();
  $form['nys_global_nav_header_fieldset'] = _nys_global_nav_header_fieldset_field();
  $form['nys_global_nav_header_fieldset']['nys_global_nav_header_auto'] = _nys_global_nav_header_auto_field();
  $form['nys_global_nav_header_fieldset']['nys_global_nav_header_format'] = _nys_global_nav_header_format_field();
  $form['nys_global_nav_header_fieldset']['nys_global_nav_header_menu'] = _nys_global_nav_header_menu_field();
  $form['nys_global_nav_footer_fieldset'] = _nys_global_nav_footer_fieldset_field();
  $form['nys_global_nav_footer_fieldset']['nys_global_nav_footer_auto'] = _nys_global_nav_footer_auto_field();
  $form['nys_global_nav_footer_fieldset']['nys_global_nav_footer_menu'] = _nys_global_nav_footer_menu_field();
  $form['nys_global_nav_footer_fieldset']['nys_global_nav_social_media_fieldset'] = _nys_global_nav_social_media_fieldset_field();
  $form['nys_global_nav_footer_fieldset']['nys_global_nav_social_media_fieldset']['nys_global_nav_social_media'] = _nys_global_nav_social_media_field();
  return system_settings_form($form);
}

/**
 * NYS Global Navigation configuration form validation.
 */
function nys_global_nav_admin_settings_validate($form, &$form_state) {
  // Check if Special Menu Items is configured correctly.
  if (module_exists('special_menu_items') && (variable_get('special_menu_items_nolink_tag', '<span>') !== '<a href="#">')) {
    drupal_set_message(htmlspecialchars('Special Menu Items module needs to have its <nolink> HTML set to <a href="#"> ') . l('Configure now', '/admin/config/system/special_menu_items'), 'warning');
  }
  // Check the number of footer menu items.
  $header_menu_top_count = _nys_global_nav_menu_count_top($form_state['values']['nys_global_nav_header_menu']);
  if (($header_menu_top_count < 5) ||($header_menu_top_count > 7)) {
    drupal_set_message(t('The header menu should have between 5 and 7 top level menu items.  Your menu has  @count.', array('@count' => $header_menu_top_count)), 'warning');
  }

  // Check the number of footer menu items.
  $footer_menu_top_count = _nys_global_nav_menu_count_top($form_state['values']['nys_global_nav_footer_menu']);
  if (($footer_menu_top_count < 1) ||($footer_menu_top_count > 5)) {
    drupal_set_message(t('The footer menu should have between 1 and 5 top level menu items.  Your menu has @count.', array('@count' => $footer_menu_top_count)), 'warning');
  }

}

/**
 * NYS Global Navigation agency fieldset field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_agency_fieldset_field() {
  return array(
    '#type' => 'fieldset',
    '#title' => t('Agency navigation options'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
}

/**
 * NYS Global Navigation header fieldset field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_header_fieldset_field() {
  return array(
    '#type' => 'fieldset',
    '#title' => t('Agency header options'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
}

/**
 * NYS Global Navigation footer fieldset field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_footer_fieldset_field() {
  return array(
    '#type' => 'fieldset',
    '#title' => t('Agency footer options'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
}
/**
 * NYS Global Navigation social media fieldset field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_social_media_fieldset_field() {
  return array(
    '#type' => 'fieldset',
    '#title' => t('Social media options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
}

/**
 * NYS Global Navigation agency name field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_agency_name_field() {
  return array(
    '#type' => 'textfield',
    '#title' => t('Agency name'),
    '#default_value' => variable_get('nys_global_nav_agency_name', ''),
    '#maxlength' => 128,
    '#size' => '60',
    '#description' => t('Enter the agency name for the global navigation.  You can use &#60;br&#47;&#62; to cause the name to split.'),
  );
}

/**
 * NYS Global Navigation agency grouping color field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_agency_color_field() {
  $agency_groupings = array(
    'administration' => 'Administration',
    'business' => 'Business',
    'education' => 'Education',
    'health-and-human-services' => 'Health and Human Services',
    'local-and-regional-authorities' => 'Local and Regional authorities',
    'public-safety' => 'Public Safety',
    'recreation-and-environment' => 'Recreation and Environment',
    'statewide-elected-officials' => 'Statewide Elected Officials',
    'transportation-and-utilities' => 'Transportation and Utilities',
  );
  return array(
    '#type' => 'select',
    '#title' => t('Agency grouping color'),
    '#options' => $agency_groupings,
    '#default_value' => variable_get('nys_global_nav_agency_color', ''),
    '#multiple' => FALSE,
    '#empty_option' => 'None',
    '#empty_value' => '',
    '#description' => t('Select which agency grouping color to use for the global navigation header/footer.'),
  );
}

/**
 * NYS Global Navigation agency grouping color field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_header_format_field() {
  $format_options = array(
    'horizontal stacked' => 'Horizontal - stacked',
    'horizontal unstacked' => 'Horizontal - unstacked',
    'vertical stacked' => 'Vertical',
  );
  return array(
    '#type' => 'select',
    '#title' => t('Agency navigation header format'),
    '#options' => $format_options,
    '#default_value' => variable_get('nys_global_nav_header_format', ''),
    '#multiple' => FALSE,
    '#empty_option' => 'None',
    '#empty_value' => '',
    '#description' => t('Select which header format to use.'),
  );
}

/**
 * NYS Global Navigation header menu selection field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_header_menu_field() {
  $menu_list = menu_get_menus(TRUE);
  return array(
    '#type' => 'select',
    '#title' => t('Global navigation header menu'),
    '#options' => $menu_list,
    '#default_value' => variable_get('nys_global_nav_header_menu', ''),
    '#multiple' => FALSE,
    '#empty_option' => 'None',
    '#empty_value' => '',
    '#description' => t('Select which menu to use in the global header.  If the menu has more than 7 first level item, all are output which might cause formatting issues.'),
  );
}

/**
 * NYS Global Navigation header automatic insertion field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_header_auto_field() {
  return array(
    '#type' => 'checkbox',
    '#title' => t('Global navigation header automatic insertion'),
    '#default_value' => variable_get('nys_global_nav_header_auto', ''),
    '#multiple' => FALSE,
    '#description' => t('Select if the global header is to be automatically inserted into the page.  If not selected, make sure to use the NYS Global Navigation Header block'),
  );
}

/**
 * NYS Global Navigation footer automatic insertion field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_footer_auto_field() {
  return array(
    '#type' => 'checkbox',
    '#title' => t('Global navigation footer automatic insertion'),
    '#default_value' => variable_get('nys_global_nav_footer_auto', ''),
    '#multiple' => FALSE,
    '#description' => t('Select if the global footer is to be automatically inserted into the page.  If not selected, make sure to use the NYS Global Navigation Footer block'),
  );
}

/**
 * NYS Global Navigation footer menu selection field.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_footer_menu_field() {
  $menu_list = menu_get_menus(TRUE);
  return array(
    '#type' => 'select',
    '#title' => t('Global navigation footer menu'),
    '#options' => $menu_list,
    '#default_value' => variable_get('nys_global_nav_footer_menu', ''),
    '#multiple' => FALSE,
    '#empty_option' => 'None',
    '#empty_value' => '',
    '#description' => t('Select which menu to use in the global footer.  The first level menu items will be the column headers.  If the menu has more than 5 first level item, all are output which might cause formatting issues.'),
  );
}

/**
 * NYS Global Navigation social media fields.
 *
 * @return array
 *   Form API element for field.
 */
function _nys_global_nav_social_media_field() {
  $social_media_list = _nys_global_nav_setup_social_names();
  $social_media_names = array();
  foreach ($social_media_list as $key => $social_media_name) {
    $social_media_index_name = 'nys_global_nav_social_media_' . check_plain($key);
    $social_media_names[$social_media_index_name] = array(
      '#type' => 'textfield',
      '#title' => t('@social URL', array('@social' => $social_media_name)),
      '#default_value' => variable_get($social_media_index_name, ''),
      '#maxlength' => 128,
      '#size' => 60,
      '#description' => t('Enter the URL for @social.  If left blank, the social media icon will not display.  If no social media icons display, the section will not display', array('@social' => $social_media_name)),
    );
  }
  return $social_media_names;
}
