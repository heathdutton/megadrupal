<?php


// Set arctica theme directory
$arctica_theme_path = drupal_get_path('theme', 'arctica');

// Add styling to skinr form
drupal_add_css($arctica_theme_path . '/styling/css/arctica.skinr.admin.css', array('group' => CSS_THEME, 'weight' => 10));

// Implement skinr form_callback to ensure this inc file gets loaded on every edit-skin form request
function arctica_skinr_callback($form, $form_state, $skinr_theme_context) {
  $form = array(
    '#type' => 'hidden',
  );
  return $form;
}

/**
 * Implements hook_skinr_group_info().
 * @todo
 * figure out why this doesn't work
 */
function arctica_skinr_group_info() {
  $groups['arctica_layoutkit'] = array(
    'title' => t('Arctica LayoutKit'),
    'description' => t("Grid, layout and other structural related skins. Settings for smaller screens are inherited by the Big-screen setting so you don't have to copy your default settings to all the layouts"),
    'weight' => -20,
  );
  $groups['arctica_styling'] = array(
    'title' => t('Arctica block styles'),
    'description' => t('Generic styling of blocks, available in all Arctica subthemes.'),
    'weight' => -19,
  );
  return $groups;
}

/**
 * Implements hook_skinr_skin_PLUGIN_info().
 */
function arctica_skinr_skin_core_info() {
  $skins = array();
  // drupal_set_message('go arctica_skinr_skin_core_info');
  // Load Sooper Features that provide skins
  $arctica_theme_path = drupal_get_path('theme', 'arctica') . '/';
  foreach (file_scan_directory($arctica_theme_path . '/features', '/skins.inc/i') as $file) {
    require_once($file->uri);
  }

  // Use Skinr form callback to discover the theme context for the skins
  $skins['arctica_skinr_callback'] = array(
    'title' => t('Arctica Skinr Callback'),
    'form callback' => 'arctica_skinr_callback',
    'group' => 'general',
    'default status' => 1,
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
  );

  /**
   * Arctica skins
   */

  $skins['style_skins'] = array(
    'title' => t('Block style'),
    'type' => 'select',
    'description' => t('Present blocks in different styles'),
    'group' => 'general',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'default status' => TRUE,
    'options' => array(
      'no-style' => array(
        'title' => 'No styling (e.g. for ads)',
        'class' => array('no-style'),
      ),
      'squared' => array(
        'title' => 'Square corners',
        'class' => array('arctica-squared'),
      ),
      'rounded' => array(
        'title' => 'Rounded corners (5px)',
        'class' => array('arctica-rounded-medium', 'arctica-rounded'),
      ),
      'rounded-small' => array(
        'title' => 'Small rounded corners (3px)',
        'class' => array('arctica-rounded-small', 'arctica-rounded'),
      ),
      'rounded-large' => array(
        'title' => 'Large rounded corners (10px)',
        'class' => array('arctica-rounded-large', 'arctica-rounded'),
      ),
      'rounded-xlarge' => array(
        'title' => 'Extra large rounded corners (25px)',
        'class' => array('arctica-rounded-xlarge', 'arctica-rounded'),
      ),
    ),
  );

  $skins['block_icon'] = array(
    'title' => t('Block icon'),
    'type' => 'select',
    'description' => t('Searches for an image with the .block-icon class in the block content and puts it next to the title'),
    'group' => 'general',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'default status' => TRUE,
    'options' => array(
      'icon-left' => array(
        'title' => 'Icon left',
        'class' => array('arctica-icon-block'),
      ),
      'icon-right' => array(
        'title' => 'Icon right',
        'class' => array('arctica-icon-block', 'arctica-icon-block-right'),
      ),
    ),
  );

  $skins['content_alignment'] = array(
    'title' => t('Content alignment'),
    'description' => t('Default'),
    'type' => 'select',
    'group' => 'general',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'default status' => TRUE,
    'options' => array(
      'left' => array(
        'title' => 'Left align content within its container',
        'class' => array('arctica-left-content'),
      ),
      'center' => array(
        'title' => 'Center align content within its container',
        'class' => array('arctica-center-content'),
      ),
      'right' => array(
        'title' => 'Right align content within its container',
        'class' => array('arctica-right-content'),
      ),
    ),
  );

  $skins['font_size'] = array(
    'title' => t('Font size'),
    'type' => 'select',
    'description' => t('Changes the font size of content within this block'),
    'group' => 'general',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'default status' => TRUE,
    'options' => array(
      'xsmall' => array(
        'title' => 'Extra small (75%)',
        'class' => array('arctica-xsmall'),
      ),
      'small' => array(
        'title' => 'Small (90%)',
        'class' => array('arctica-small'),
      ),
      'large' => array(
        'title' => 'Large (125%)',
        'class' => array('arctica-large'),
      ),
      'xlarge' => array(
        'title' => 'Extra large (150%)',
        'class' => array('arctica-xlarge'),
      ),
      'xxlarge' => array(
        'title' => 'Extra-extra large (200%)',
        'class' => array('arctica-xxlarge'),
      ),
    ),
  );

  $skins['font_style'] = array(
    'title' => t('Font style'),
    'type' => 'checkboxes',
    'description' => t('Changes the font of content within this block. The Fontkit option will load the default/first font that is loaded in the Sooper FontKit configuration.'),
    'group' => 'general',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'default status' => TRUE,
    'options' => array(
      'bold' => array(
        'title' => 'Bold',
        'class' => array('arctica-bold'),
      ),
      'italic' => array(
        'title' => 'Italic',
        'class' => array('arctica-italic'),
      ),
      'underlined' => array(
        'title' => 'Underlined',
        'class' => array('arctica-underlined'),
      ),
    ),
  );

  return $skins;
}
