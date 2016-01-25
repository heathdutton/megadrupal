<?php

$plugin = array(
    'title' => t('Wizard Steps'),
    'render callback' => 'wizardsteps_wizard_steps_content_type_render',
    'edit form' => 'wizardsteps_wizard_steps_content_type_edit_form',
    'description' => t('Displays the wizard steps for a CTools wizard form'),
    'category' => t('CTools'),
    'no title override' => FALSE,
    'single' => TRUE,
    'required context' => new ctools_context_required(t('Wizard Info'), 'wizard_info'),
);


function wizardsteps_wizard_steps_content_type_render($subtype, $conf, $args, $context) {
  global $wizardsteps_wizard_info;

  $content = new stdClass();
  $content->title = t("Steps");

  $output = '';
  $links = array();
  if (!empty($wizardsteps_wizard_info)) {
    foreach ($wizardsteps_wizard_info['form_info']['order'] as $key => $title) {
      $path = $wizardsteps_wizard_info['form_info']['path'];
      $path = str_replace('%step', $key, $path);
      
      $classes = array();
      $classes[] = 'wizard-step-link';
      $classes[] = 'step-' . $key;

      if ($key == $wizardsteps_wizard_info['step']) {
        $classes[] = 'active';
      }
      else {
        $classes[] = 'not-active';
      }

      $link = l($title, $path, array('attributes' => array('class' => $classes)));
      $links[] = $link;
    }

    $output .= theme('item_list', array('items' => $links));
  }

  $content->content = $output;
  return $content;
}

function wizardsteps_wizard_steps_content_type_edit_form($form, &$form_state) {
  return $form;
}
