<?php

/**
 * @file
 * Provides the names and attributes of the target jobfaucet fields.
 */

/**
 * Provides an array of accepted job board fields.
 */
function jobfaucet_get_target_fields() {
  return array(
    'jobfaucet_title' => array(
      'label' => t('Job Title'),
      'description' => t(''),
      'default_mapping' => '[node:title]',
      'allow_fixed_value' => FALSE,
      'required' => TRUE,
    ),
    'jobfaucet_summary' => array(
      'label' => t('Job Summary'),
      'description' => t('Consider entering multiple tokens (e.g. "[node:body] [node:field_required_skills]").'),
      'default_mapping' => '[node:body]',
      'allow_fixed_value' => FALSE,
      'required' => TRUE,
    ),
    'jobfaucet_how_to_apply' => array(
      'label' => t('How to Apply'),
      'description' => t('Required, but may be included in the summary field instead if you prefer.  If so, enter <em>!placeholder</em> in this field to avoid validation errors.', array('!placeholder' => '&lt;none&gt;')),
      'default_mapping' => '[node:jobfaucet_how_to_apply]',
      'allow_fixed_value' => TRUE,
      'required' => TRUE,
    ),
    'jobfaucet_state' => array(
      'label' => t('State'),
      'description' => t('Consider using a static value (e.g. "Massachusetts").'),
      'default_mapping' => '[node:jobfaucet_state]',
      'allow_fixed_value' => TRUE,
      'required' => TRUE,
    ),
    'jobfaucet_country' => array(
      'label' => t('Country'),
      'description' => t('Consider using a static value (e.g. "United States").'),
      'default_mapping' => '[node:jobfaucet_country]',
      'allow_fixed_value' => TRUE,
      'required' => TRUE,
    ),
    'jobfaucet_city' => array(
      'label' => t('City'),
      'description' => t('Consider using a static value (e.g. "Boston").'),
      'default_mapping' => '',
      'allow_fixed_value' => TRUE,
      'required' => FALSE,
    ),
    'jobfaucet_zip_code' => array(
      'label' => t('Zip Code'),
      'description' => t(''),
      'default_mapping' => '',
      'allow_fixed_value' => TRUE,
      'required' => FALSE,
    ),
    'jobfaucet_company' => array(
      'label' => t('Company Name'),
      'description' => t('Consider using a static value (e.g. "My Company, Inc.").'),
      'default_mapping' => variable_get('site_name'),
      'allow_fixed_value' => TRUE,
      'required' => FALSE,
    ),
    'jobfaucet_salary' => array(
      'label' => t('Salary'),
      'description' => t(''),
      'default_mapping' => '[node:jobfaucet_salary]',
      'allow_fixed_value' => TRUE,
      'required' => FALSE,
    ),
    'jobfaucet_education' => array(
      'label' => t('Education'),
      'description' => t(''),
      'default_mapping' => '[node:jobfaucet_education]',
      'allow_fixed_value' => TRUE,
      'required' => FALSE,
    ),
    'jobfaucet_experience' => array(
      'label' => t('Experience'),
      'description' => t(''),
      'default_mapping' => '[node:jobfaucet_experience]',
      'allow_fixed_value' => TRUE,
      'required' => FALSE,
    ),
  );
}
