<?php

/**
 * @file
 * Variable module integration.
 */

/**
 * Implements hook_variable_info().
 */
function step_variable_info($options) {
  return array(
    'step_button_next' => array(
      'type' => 'string',
      'title' => t('Next button', array(), $options),
      'default' => 'Next',
      'description' => t("The 'Next' button label.", array(), $options),
      'required' => TRUE,
      'group' => 'step_step',
    ),
    'step_button_back' => array(
      'type' => 'string',
      'title' => t('Back button', array(), $options),
      'default' => 'Back',
      'description' => t("The 'Back' button label.", array(), $options),
      'required' => TRUE,
      'group' => 'step_step',
    ),
    'step_button_finish' => array(
      'type' => 'string',
      'title' => t('Finish button', array(), $options),
      'default' => 'Finish',
      'description' => t("The 'Finish' button label.", array(), $options),
      'required' => TRUE,
      'group' => 'step_step',
    ),
    'step_title_[step]' => array(
      'localize' => TRUE,
      'type' => 'multiple',
      'title' => t('Title', array(), $options),
      'repeat' => array(
        'type' => 'select',
        'default' => STEP_REGISTER,
        'options' => 'step_steps',
      ),
      'description' => t("The wizard step title.", array(), $options),
      'group' => 'step_step',
    ),
    'step_description_[step]' => array(
      'localize' => TRUE,
      'type' => 'multiple',
      'title' => t('Description', array(), $options),
      'repeat' => array(
        'type' => 'select',
        'default' => STEP_REGISTER,
        'options' => 'step_steps',
      ),
      'description' => t("The wizard step description.", array(), $options),
      'group' => 'step_step',
    ),
  );
}

/**
 * Implements hook_variable_type_info()
 */
function step_variable_type_info() {
  return array(
    'step' => array(
      'title' => t('Step'),
      'options callback' => 'step_steps',
      'type' => 'select',
    ),
  );
}

/**
 * Implements hook_variable_group_info().
 */
function step_variable_group_info() {
  return array(
    'step_step' => array(
      'title' => t('Multi-Step Registration'),
      'description' => t('Multi-Step Registration variables.'),
      'access' => 'administer site configuration',
      'path' => 'admin/config/people/step/%step/edit',
    ),
  );
}
