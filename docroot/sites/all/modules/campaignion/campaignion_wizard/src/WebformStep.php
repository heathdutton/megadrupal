<?php

namespace Drupal\campaignion_wizard;

/**
 * Wizard step for configuring webforms using form_builder.
 *
 * NOTE: Needs form_builder_webform to work.
 */
class WebformStep extends WizardStep {
  protected $step = 'form';
  protected $title = 'Form';
  protected function loadIncludes() {
    module_load_include('inc', 'form_builder', 'includes/form_builder.admin');
    module_load_include('inc', 'form_builder', 'includes/form_builder.api');
    module_load_include('inc', 'form_builder', 'includes/form_builder.cache');
  }

  public function pageCallback() {
    $build = parent::pageCallback();
    $form =& $build[0];
    $form['actions']['#access'] = FALSE;
    $form['#weight'] = -100;

    $path = drupal_get_path('module', 'webform');
    $build['#attached']['css'][] = $path . '/css/webform.css';
    $build['#attached']['css'][] = $path . '/css/webform-admin.css';
    $build['#attached']['js'][] = $path . '/js/webform.js';
    $build['#attached']['js'][] = $path . '/js/webform-admin.js';
    $build['#attached']['js'][] = $path . '/js/select-admin.js';
    $build['#attached']['js'][] = drupal_get_path('module', 'campaignion_wizard') . '/js/form-builder-submit.js';
    $build['#attached']['library'][] = array('system', 'ui.datepicker');

    // Build form for webform_template select box.
    if (module_exists('campaignion_action_template')) {
      $build[] = drupal_get_form('campaignion_action_template_selector_form', $this->wizard->node)
        + array('#weight' => -99);
    }

    // Load all components.
    $components = webform_components();
    foreach ($components as $component_type => $component) {
      webform_component_include($component_type);
    }
    module_load_include('inc', 'form_builder', 'includes/form_builder.admin');
    foreach (form_builder_interface('webform', $form['nid']['#value']) as $k => $f) {
      $build[$k + 2] = $f;
    }

    return $build;
  }

  public function stepForm($form, &$form_state) {
    $form = parent::stepForm($form, $form_state);
    $form = \form_builder_webform_save_form($form, $form_state, $this->wizard->node->nid);
    $form_state['build_info']['base_form_id'] = 'form_builder_webform_save_form';
    return $form;
  }

  public function checkDependencies() {
    return isset($this->wizard->node->nid);
  }

  public function validateStep($form, &$form_state) {
    // form_builder seems to use it's ajax callbacks for validation.
  }

  public function submitStep($form, &$form_state) {
    form_builder_webform_save_form_submit($form, $form_state);
  }

  public function status() {
    return NULL;
  }
}
