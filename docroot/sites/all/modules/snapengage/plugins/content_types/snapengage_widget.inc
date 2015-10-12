<?php
/**
 * @file
 * Plugin to handle the 'snapengage_widget' content type.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('SnapEngage Widget'),
  'single' => TRUE,
  'description' => t('Add a SnapEngage widget.'),
  'category' => t('Widgets'),
  'defaults' => array(
    'snapengage_widget_id' => NULL,
    'snapengage_widget_language' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Output function.
 */
function snapengage_snapengage_widget_content_type_render($subtype, $conf, $panel_args, $pane_context, $incoming_content) {
  $block = new stdClass();

  // Add widget code if there's a widget id.
  $allowed_tags = array();
    if (!empty($conf['snapengage_widget_id'])) {
      _snapengage_add_snapengage_script(
        filter_xss($conf['snapengage_widget_id'], $allowed_tags),
        $conf['snapengage_widget_language']
      );
    }
  return $block;
}

/**
 * Define the settings form.
 */
function snapengage_snapengage_widget_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['snapengage_widget_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Widget ID'),
    '#default_value' => $conf['snapengage_widget_id'],
    '#required' => TRUE,
    '#description' => t('Your widget ID. It will look somewhat like this: "abcdefgh-1234-ijkl-5678-mnopqrstuvwx". You can find it at <a href="!url">!url</a>.', array('!url' => 'https://secure.snapengage.com/widget')),
  );
  $form['snapengage_widget_language'] = array(
  '#type' => 'select',
    '#title' => t('Widget language'),
    '#default_value' => variable_get('snapengage_widget_language', 'default'),
    '#required' => TRUE,
    '#options' => array(
      'default' => t('Default language of site. With URL detection.'),
      'user' => t('User\'s default language'),
    ),
    '#description' => t('Select how to choose language.'),
);

  unset($form['override_title'], $form['override_title_text'], $form['override_title_markup']);
  return $form;
}


/**
 * Store the new configuration settings.
 */
function snapengage_snapengage_widget_content_type_edit_form_submit($form, &$form_state) {
  $elements = array(
    'snapengage_widget_id',
    'snapengage_widget_language',
  );
  foreach ($elements as $element) {
    if (isset($form_state['values'][$element])) {
      $form_state['conf'][$element] = $form_state['values'][$element];
    }
  }
}
