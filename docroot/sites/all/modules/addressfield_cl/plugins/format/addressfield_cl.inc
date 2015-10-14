<?php

/**
 * @file
 * The format for Chile adresses.
 */

$plugin = array(
  'title'           => t('Address form (specific for Chile).'),
  'format callback' => 'addressfield_cl_format_address',
  'type'            => 'address',
  'weight'          => 0,
);


/**
 * The format callback.
 */
function addressfield_cl_format_address(&$format, $address, $context = array()) {
  if ($address['country'] == 'CL') {
    $use_provinces = variable_get('addressfield_cl_provinces', FALSE);

    if (!variable_get('addressfield_cl_postal_code', FALSE)) {
      unset($format['locality_block']['postal_code']);
    }

    if (!$use_provinces) {
      unset($format['locality_block']['locality']);
    }

    $options = array(
      '' => t('--'),
    );

    $options += addressfield_cl_get_options_regions();
    $format['locality_block']['administrative_area'] = array(
      '#title'              => t('Region'),
      '#options'            => $options,
      '#required'           => TRUE,
      '#render_option_value'=> TRUE,
      '#attributes'         => array('class' => array('locality')),
      '#weight'             => 1,
    );

    if ($use_provinces) {
      $format['locality_block']['administrative_area']['#element_validate'][] = 'addressfield_cl_format_address_validate_change_region_commune';
      $format['locality_block']['administrative_area']['#process'][] = 'ajax_process_form';
      $format['locality_block']['administrative_area']['#process'][] = 'addressfield_cl_format_address_process';

      $provinces_options = array(
        '' => t('--'),
      );
      $provinces_options += addressfield_cl_get_options_provinces_by_region($address['administrative_area']);

      $format['locality_block']['locality'] = array(
        '#title'              => t('Province'),
        '#size'               => 20,
        '#required'           => TRUE,
        '#render_option_value'=> TRUE,
        '#tag'                => 'div',
        '#options'            => $provinces_options,
        '#attributes'         => array('class' => array('dependent', 'province')),
        '#weight'             => 2,
      );
    }

    $communes_options = array(
      '' => t('--'),
    );
    if ($use_provinces) {
      $communes_options += addressfield_cl_get_options_communes_by_province($address['locality']);
    }
    else {
      $communes_options += addressfield_cl_get_options_communes_by_region($address['administrative_area']);
    }

    $format['locality_block']['dependent_locality'] = array(
      '#title'              => t('Commune'),
      '#size'               => 20,
      '#render_option_value'=> TRUE,
      '#tag'                => 'div',
      '#required'           => TRUE,
      '#options'            => $communes_options,
      '#attributes'         => array('class' => array('dependent', 'commune')),
      '#weight'             => 3,
    );
  }

  // Format render
  if ($context['mode'] == 'render') {
    $format['locality_block']['dependent_locality']['#weight'] = 1;
    if ($use_provinces) {
      $format['locality_block']['locality']['#weight'] = 2;
    }
    $format['locality_block']['administrative_area']['#weight'] = 3;
  }

  if ($context['mode'] == 'form' && $address['country'] == 'CL') {
    $ajax_options = array(
      'callback' => 'addressfield_standard_widget_refresh',
      'wrapper' => $format['#wrapper_id'],
    );
    $format['locality_block']['administrative_area']['#ajax'] = $ajax_options;
    if ($use_provinces) {
      $format['locality_block']['locality']['#ajax'] = $ajax_options;
    }
  }
}
