<?php
/**
 * Admin settings page
 */

function dynatree_admin_settings_form($form, &$form_state) {

  $form = array();

  $menus = _dynatree_menu_menus();

  $form['dynatree_forms_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('Form CSS IDs'),
    '#rows' => 3,
    '#cols' => 40,
    '#default_value' => variable_get('dynatree_forms_ids', '#primary-menu-bar nav'),
    '#description' => t('Enter the CSS IDs/class of the menu containing the UL. One per line. SAMPLE: #block-menu-menu-texturen .content'),
  );

  $form['dynatree_id_list'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Collaps menus by default'),
    '#options' => $menus,
    '#default_value' => variable_get('dynatree_id_list', array()),
    '#description' => t('Chose your menus that should be collapsed by default. Most users want that by default. (technically it removes classes that would interfer with dynatree)'),
  );

  //Theme usage
  $form['dynatree']['dynatree_theme_select'] = array(
    '#type' => 'radios',
    '#title' => 'Chose Theme usage',
    '#default_value' => variable_get('dynatree_theme_select', 'default'),
    '#options' => array(
      'default' => t('Use themes bundled with the script'),
      'custom' => t('Specify custom theme'),
    ),
    '#description' => '',

  );

  //a custom theme
  $form['dynatree_custom_theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom theme'),
    '#collapsible' => FALSE,
    '#states' => array(
      'visible' => array (
        ':input[name="dynatree_theme_select"]' => array('value' => 'custom'),
      )
    )
  );

  $form['dynatree_custom_theme']['dynatree_custom_theme_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom theme folder'),
    '#default_value' => variable_get('dynatree_custom_theme_path', ''),
    '#description' => t("If you want to use a custom theme, set the path here. Example sites/all/themes/adaptivetheme/adaptivetheme_subtheme/MYSKIN/style.css'. <strong>NO LEADING SLASH</strong>"),

  );

  //the standard themes shipped with the script
  $form['dynatree_themes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Standard themes'),
    '#collapsible' => FALSE,
    '#states' => array(
      'visible' => array (
        ':input[name="dynatree_theme_select"]' => array('value' => 'default'),
      )
    )
  );

  $form['dynatree_themes']['dynatree_script_themes'] = array(
    '#type' => 'select',
    '#title' => t('Themes bundled with the script itself'),
    '#default_value' => variable_get('dynatree_script_themes', 'skin'),
    '#options' => array (
     'skin' => 'skin',
     'skin-vista' => 'skin-vista',
     ),
    '#description' => t('Select a default theme that shipps with dynatree'),
  );


  return system_settings_form($form);
}


/**
 * Helper function to get avaliable menus
 *
 * @return $menu_names
 *  array with avaliable menus
 *
 */

function _dynatree_menu_menus() {
  if (function_exists('menu_get_menus')) {
    // If the menu module is enabled, list custom menus.
    $menus = menu_get_menus();
  }
  else {
    // Otherwise, list only system menus.
    $menus = menu_list_system_menus();
  }

  // If the book module is enabled, also include book menus.
  if (function_exists('book_get_books')) {
    foreach (book_get_books() as $bid => $link) {
      $menus["book-toc-$bid"] = t('Book: %title', array('%title' => $link['title']));
    }
  }

  // menu_get_names() filters out empty menus, and adds any menus not found using the above calls (edge case).
  foreach (menu_get_names() as $menu) {
    $menu_names[$menu] = isset($menus[$menu]) ? $menus[$menu] : t('Other menu: %menu', array('%menu' => $menu));
  }

  return $menu_names;
}



