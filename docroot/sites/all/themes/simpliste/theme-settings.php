<?php

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function simpliste_form_system_theme_settings_alter(&$form, $form_state) {

  require_once 'template.php';

  $form['simpliste'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -10,
    '#default_tab' => 'defaults',
  );
 
  //-- Skins.
  $form['simpliste']['skins'] = array(
    '#type' => 'fieldset',
    '#title' => t('Skins'),
  );
  $skins = simpliste_get_skins();
  $form['simpliste']['skins']['active_skin'] = array(
    '#type' => 'select',
    '#title' => t('Select your skin'),
    '#default_value' => theme_get_setting('active_skin'),
    '#options' => drupal_map_assoc($skins),
  );

  //-- Layout.
  $form['simpliste']['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Layout'),
  );
  $form['simpliste']['layout']['sidebar_position'] = array(
    '#type' => 'select',
    '#title' => t('Sidebar position'),
    '#default_value' => theme_get_setting('sidebar_position'),
    '#options' => array(
      'left' => t('Left'),
      'right' => t('Right'),
    ),
  );
  $form['simpliste']['layout']['sidebar_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show sidebar on specific pages'),
    '#options' => array(
      t('All pages except those listed'),
      t('Only the listed pages'),
    ),
    '#default_value' => theme_get_setting('sidebar_visibility'),
  );
  $form['simpliste']['layout']['sidebar_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => theme_get_setting('sidebar_pages'),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  //-- Breadcrumbs.
  $breadcrumb = theme_get_setting('breadcrumb');
  $breadcrumb_states = array(
    'visible' => array('#edit-breadcrumb-display' => array('checked' => TRUE)),
  );
  $form['simpliste']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#weight' => '1',
    '#title' => t('Breadcrumb'),
    '#tree' => TRUE,
  );
  $form['simpliste']['breadcrumb']['display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => $breadcrumb['display'],
  );
  $form['simpliste']['breadcrumb']['separator'] = array(
    '#type'  => 'textfield',
    '#title' => t('Breadcrumb separator'),
    '#description' => t('Text only. Don\'t forget to include spaces.'),
    '#default_value' => $breadcrumb['separator'],
    '#size' => 8,
    '#maxlength' => 8,
    '#states' => $breadcrumb_states,
  );
  $form['simpliste']['breadcrumb']['home'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the homepage link in breadcrumbs'),
    '#default_value' => $breadcrumb['home'],
    '#states' => $breadcrumb_states,
  );
  $form['simpliste']['breadcrumb']['title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append the content title to the end of the breadcrumb'),
    '#default_value' => $breadcrumb['title'],
    '#states' => $breadcrumb_states,
  );

  //-- Forms.
  $form['simpliste']['forms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Forms'),
  );
  $form['simpliste']['forms']['hide_format_node_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide format selection for nodes'),
    '#default_value' => theme_get_setting('hide_format_node_form'),
  );
  $form['simpliste']['forms']['hide_format_comment_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide format selection for comments'),
    '#default_value' => theme_get_setting('hide_format_comment_form'),
  );

  //-- Development.
  $form['simpliste']['development'] = array(
    '#type' => 'fieldset',
    '#title' => t('Development'),
  );
  $form['simpliste']['development']['wireframes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wireframe Mode - Display borders around main layout elements.'),
    '#default_value' => theme_get_setting('wireframes'),
  );
  $form['simpliste']['development']['rebuild_registry'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild theme registry on every page'),
    '#default_value' => theme_get_setting('rebuild_registry'),
  );
  $form['simpliste']['development']['settings_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display settings form (for administrators only)'),
    '#default_value' => theme_get_setting('settings_form'),
  );
  $form['simpliste']['development']['timer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display page execution time'),
    '#default_value' => theme_get_setting('timer'),
  );

  //-- Miscellaneous.
  $form['simpliste']['miscellaneous'] = array(
    '#type' => 'fieldset',
    '#title' => t('Miscellaneous'),
  );
  $form['simpliste']['miscellaneous']['head_code'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML Header'),
    '#description' => t('This HTML code will be located in HEAD tag'),
    '#default_value' => theme_get_setting('head_code'),
    '#rows' => 1,
  );
  $form['simpliste']['miscellaneous']['featured'] = array(
    '#type' => 'textarea',
    '#title' => t('Featured content'),
    '#description' => t('This HTML code will be located in featured region'),
    '#default_value' => theme_get_setting('featured'),
    '#rows' => 1,
  );
  $form['simpliste']['miscellaneous']['footer_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer message'),
    '#description' => t('This HTML code will be located in footer region'),
    '#default_value' => theme_get_setting('footer_message'),
    '#rows' => 1,
  );
}
