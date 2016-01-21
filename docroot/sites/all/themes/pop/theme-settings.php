<?php
/**
 * @file template.php
 * Pop base theme settings.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function pop_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['pop'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Theme settings'),
    '#description'  => t('Customize the appearance of your Pop base theme or sub-theme if you are on sub-theme configuration page.'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  // Developer settings
  if (theme_get_setting('enable_dev_settings')) {
    $form['pop']['dev_settings'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Developer settings'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE,
    );
    $form['pop']['dev_settings']['grid_overlay'] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Enable grid overlay.'),
      '#default_value'  => theme_get_setting('grid_overlay'),
      '#description'    => t('Enables a "Show Grid" button in the bottom left corner of each page to toggle a grid overlay, which can help easily align, match and/or lay out websites.'),
    );
  }

  // Page layout settings
  if (theme_get_setting('enable_layout_settings')) {
    $form['pop']['layout'] = array(
      '#type'         => 'fieldset',
      '#title'        => t('Page layout settings'),
      '#collapsible'  => TRUE,
      '#collapsed'    => TRUE,
    );
    // Sidebars layout
    $form['pop']['layout']['sidebars_layout'] = array(
      '#type'           => 'radios',
      '#title'          => t('Sidebars layout'),
      '#description'    => t('The sidebar layout descriptions are for LTR (left to right languages), these are reversed in RTL mode.'),
      '#default_value'  => theme_get_setting('sidebars_layout'),
      '#options'        => array(
        'pop-sidebars-split' => t('Standard three column layout — First sidebar | Content | Second sidebar</span>'),
        'pop-sidebars-last'  => t('Two columns on the right — Content | First sidebar | Second sidebar</span>'),
        'pop-sidebars-first' => t('Two columns on the left — First sidebar | Second sidebar | Content</span>'),
      ),
    );
    // Equal height sidebars
    $form['pop']['layout']['equal_heights_sidebars'] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Equal Height Sidebars'),
      '#default_value'  => theme_get_setting('equal_heights_sidebars'),
      '#description'    => t('This setting will make the sidebars and the main content column equal to the height of the tallest column.'),
    );
  }

  // Search box settings
  if (theme_get_setting('enable_search_box_settings')) {
    $form['pop']['search_box'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Search box'),
      '#description' => t('The search box customization.'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['pop']['search_box']['search_box_label_inline'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Display search box label inline'),
      '#description'   => t('Label is shown as placeholder in the search text field.'),
      '#default_value' => theme_get_setting('search_box_label_inline'),
    );
    $form['pop']['search_box']['search_box_label_text'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Search box label'),
      '#description'   => t('Text only. Default is "Search".'),
      '#default_value' => theme_get_setting('search_box_label_text'),
      '#size'          => 30,
      '#maxlength'     => 255,
    );
    $form['pop']['search_box']['search_box_tooltip_text'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Search box tooltip, i.e. input field\'s title value'),
      '#description'   => t('Text only, <em>&lt;none&gt;</em> to disable. Default is "Enter the terms you wish to search for.".'),
      '#default_value' => theme_get_setting('search_box_tooltip_text'),
      '#size'          => 30,
      '#maxlength'     => 255,
    );
    $form['pop']['search_box']['search_box_button'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display the search button'),
      '#default_value' => theme_get_setting('search_box_button'),
    );
    $form['pop']['search_box']['search_box_button_text'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Search box button text'),
      '#description'   => t('Text only. Default is "Search".'),
      '#default_value' => theme_get_setting('search_box_button_text'),
      '#size'          => 20,
      '#maxlength'     => 30,
    );
  }
}
