<?php

/**
 * @file
 * Admin page callbacks for the Karmacracy module.
 */

/**
 * Form builder; Configure the karmacracy widget.
 */
function karmacracy_admin_settings() {
  $form = array();

  $form['karmacracy_widget_active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the Karmacracy widget in the content.'),
    '#default_value' => variable_get('karmacracy_widget_active', 1),
    '#description' => t('Check if you want to add the Karmacracy widget on your content. Alternately you can enable the <em>Karmacracy widget</em> block on the <a href="@blocks">Blocks administration</a> page.', array('@blocks' => url('admin/structure/block'))),
  );

  $form['karmacracy_widget_location'] = array(
    '#type' => 'select',
    '#title' => t('Widget location'),
    '#description' => t('Choose where you want to put the widget.'),
    '#default_value' => variable_get('karmacracy_widget_location', 'block'),
    '#options' => array(
      'body' => t('After the body'),
      'beforebody' => t('Before the body'),
      //'block' => t('Manual - Use Karmacracy widget macro'),
    ),
    '#states' => array(
      'invisible' => array(
        ':input[name="karmacracy_widget_active"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['widget'] = array(
    '#type' => 'fieldset',
    '#title' => t('Widget settings'),
    '#collapsible' => TRUE,
    '#attributes' => array('class' => array('widget-settings')),
  );

  $options = variable_get('karmacracy_widget_settings', karmacracy_widget_default());

  $module_path = drupal_get_path('module', 'karmacracy');
  $form['widget']['karmacracy_widget_settings'] = array(
    '#tree' => TRUE,
    '#attached' => array(
      'library' => array(
        array('system', 'farbtastic'),
      ),
      'js' => array($module_path . '/karmacracy.js'),
      'css' => array($module_path . '/karmacracy.css'),
    ),
  );

  $form['widget']['karmacracy_widget_settings']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Size'),
    '#default_value' => $options['width'],
    //'#description' => t('Set a size for the widget, so it fits perfectly in your website pages.'),
    '#size' => 10,
    '#field_suffix' => t('pixels'),
  );
  $form['widget']['karmacracy_widget_settings']['colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colour'),
    '#parents' => array('karmacracy_widget_settings'),
    '#attributes' => array('class' => array('fieldset-colorpicker')),
  );
  // Colorpicket placeholder
  $form['widget']['karmacracy_widget_settings']['colors']['colorpicker'] = array(
    '#markup' => '<div class="colorpicker"></div>',
  );
  $colors = array(
    'color1' => t('Border'),
    'color2' => t('Background'),
    'color3' => t('Left background'),
    'color4' => t('Section text'),
    'color5' => t('Button background'),
    'color6' => t('Button text'),
    'color7' => t('kclicks'),
    //'color8' => 'wid.con.colors.promo',
    'color9' => t('Lower text'),
  );
  foreach ($colors as $color => $title) {
    $form['widget']['karmacracy_widget_settings']['colors'][$color] = array(
      '#type' => 'textfield',
      '#title' => $title,
      '#title_display' => 'after',
      '#default_value' => $options[$color],
      '#size' => 7,
      '#maxlength' => 7,
      '#attributes' => array('class' => array('field-colorpicker')),
    );
  }
  $form['widget']['karmacracy_widget_settings']['other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other options'),
    '#parents' => array('karmacracy_widget_settings'),
  );
  $form['widget']['karmacracy_widget_settings']['other']['sc'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show clicks'),
    '#default_value' => $options['sc'],
    '#access' => FALSE,
  );
  $form['widget']['karmacracy_widget_settings']['other']['rb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rounded borders'),
    '#default_value' => $options['rb'],
  );
  $form['widget']['karmacracy_widget_settings']['other']['np'] = array(
    '#type' => 'checkbox',
    '#title' => t("Hide 'powered by' link"),
    '#default_value' => $options['np'],
  );

  // Widget preview placeholder
  $form['widget']['karmacracy_widget_settings']['preview'] = array(
    '#markup' => '<h3>' . t('Widget preview') . '</h3><div class="karmacracy-preview"></div>',
  );

  $form['widget']['karmacracy_widget_settings']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#submit' => array('karmacracy_widget_reset_submit'),
  );

  return system_settings_form($form);
}

/**
 * Submit callback; reset widget settings to defaults.
 */
function karmacracy_widget_reset_submit($form, &$form_state) {
  variable_del('karmacracy_widget_settings');
  drupal_set_message(t('The widget settings have been reset to their default values.'));
}
