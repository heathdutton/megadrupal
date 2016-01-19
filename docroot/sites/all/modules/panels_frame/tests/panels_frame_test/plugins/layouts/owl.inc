<?php

$plugin = array(
  'title' => t('Owl'),
  'category' => t('Builders'),
  'icon' => 'owl.png',

  'theme' => 'panels_frame_owl',
  'admin theme' => 'panels_frame_owl_admin',
  'regions' => array(
    'left' => t('Left'),
    'right' => t('Right'),
  ),

  'ajax' => array(
    'hoot' => 'panels_frame_owl_edit_hoot',
  ),
);

function theme_panels_frame_owl($vars) {
  return 'Owl..';
}

function theme_panels_frame_owl_admin($vars) {
  $css_id = $vars['css_id'];
  $content = $vars['content'];
  $settings = $vars['settings'];
  $display = $vars['display'];
  $layout = $vars['layout'];
  $handler = $vars['renderer'];

  $output = '';
  $output .= l('Hoot!', $handler->get_url('layout', 'hoot'), array('attributes' => array('class' => array('ctools-use-modal'))));

  $output .= '<div class="edit-owl">';
  foreach ($content as $id => $region) {
    $output .= $region;
  }
  $output .= '</div>';

  return $output;
}


function panels_frame_owl_edit_hoot($handler) {
  $settings = &$handler->display->layout_settings;
  $form_state = array(
    'display' => &$handler->display,
    'settings' => &$settings,
    'ajax' => TRUE,
    'title' => t('This is Owl test form...'),
  );

  $output = ctools_modal_form_wrapper('panels_frame_test_form', $form_state);

  if (!empty($form_state['executed'])) {
    $output[] = ctools_modal_command_dismiss();
  }

  $handler->commands = $output;
}

function panels_frame_test_form() {
  $form['first'] = array(
    '#type' => 'textfield',
    '#title' => t('First'),
    '#size' => 2,
  );

  $form['second'] = array(
    '#type' => 'select',
    '#title' => t('Second'),
    '#options' => drupal_map_assoc(array('Hello', 'World')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit this'),
  );

  return $form;
}

function panels_frame_test_form_submit($form, &$form_state) {
  dpm(func_get_args(), 'args');
}
