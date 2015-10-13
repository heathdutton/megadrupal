<?php

/**
 * @file
 * lom resource add/edit form part: for technical tab
 */

function archibald_content_form_technical_zero_pad( &$digit ) {
  $digit = sprintf("%02d", $digit);
}


/**
 * part of lom resource add/edit form
 *  tab: technical
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $language
 */
function archibald_content_form_technical(&$form, &$form_state, &$lom, $language) {
  // Include the CTools tools that we need.
  ctools_include('ajax');

  $lom_technical = $lom->getTechnical();
  if (!($lom_technical instanceof ArchibaldLomDataTechnical)) {
    $lom_technical = new ArchibaldLomDataTechnical();
  }

  if (
    archibald_check_field_permission('technical location') ||
    archibald_check_field_permission('technical format') ||
    archibald_check_field_permission('technical size') ||
    archibald_check_field_permission('technical otherPlattformRequirements')
  ) {
    $form['technical'] = array(
      '#type' => 'fieldset',
      '#title' => archibald_get_field_label('technical'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#group' => 'vt_main',
      '#tree' => TRUE,
      '#theme' => 'archibald_technical'
    );

    // Locations
    if (archibald_check_field_permission('technical location')) {
      $form['technical']['location'] = array(
        '#type' => 'fieldset',
        '#title' => archibald_get_field_label('technical.location') . archibald_get_require_type_value('technical.location') . archibald_get_field_help('technical.location'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
        '#attributes' => array('archibald_help_ident' => 'technical.location'),
        '#prefix' => '<div class="clearfix" id="archibald_technical_location-wrapper">',
        '#suffix' => '</div>',
        //'#theme' => 'archibald_technical_location'
      );

      $lindex = 0;
      if (!isset($form_state['lindex']) || empty($form_state['lindex'])) {
        $form_state['lindex'] = 0;
      }

      $locations = $lom_technical->getLocation();
      if (!empty($locations)) {
        foreach ($locations as $location) {
          if (!($location instanceof ArchibaldLomDataLocation)) {
            continue;
          }

          $form['technical']['location']['location_' . $lindex] = array(
            '#type' => 'fieldset',
            '#collapsible' => FALSE,
            '#tree' => TRUE,
            '#weight' => $lindex,
          );

          $form['technical']['location']['location_' . $lindex]['type'] = array(
            '#type' => 'radios',
            '#title' => '',
            '#options' => array('url' => t('URL'), 'text' => t('Text')),
            '#default_value' => $location->getType(),
            '#attributes' => array('class' => array('location_type')),
          );

          $default_value = $location->getValue();
          $form['technical']['location']['location_' . $lindex]['value'] = array(
            '#type' => 'textfield',
            '#title' => '',
            '#default_value' => $default_value,
            '#size' => 80,
            '#attributes' => array('class' => array('location_value')),
          );

          if (!empty($form_state['values']['technical']['location']['add'])) {
            $form_state['lindex']++;
          }
          $lindex++;
        }
      }

      if ($lindex == 0) archibald_content_form_technical_add_empty_location($form, $form_state);
      if ($form_state['lindex'] > 0) {
        for ($i=0; $i < ($form_state['lindex'] - $lindex) ; $i++) {
          archibald_content_form_technical_add_empty_location($form, $form_state);
        }
      }

      $form['technical']['location']['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add location'),
        '#weight' => 9999,
        '#limit_validation_errors' => array(),
        // #submit is required to use #limit_validation_errors
        '#submit' => array(
          'archibald_content_form_add_technical_location_submit',
        ),
        '#ajax' => array(
          'callback' => 'archibald_ajax_content_form_technical_add_empty_location',
          'wrapper' => 'archibald_technical_location-wrapper',
          'method' => 'replace',
        ),
      );
    }

    if (archibald_check_field_permission('technical format')) {
      $form['technical']['format'] = array(
        '#type' => 'select',
        '#title' => archibald_get_field_label('technical.format') . archibald_get_require_type_value('technical.format') . archibald_get_field_help('technical.format'),
        '#default_value' => $lom_technical->getFormat(),
        '#options' => archibald_get_technical_format_options(),
        '#attributes' => array('class' => array('technical_format')),
      );
    }

    if (archibald_check_field_permission('technical size')) {
      $size = $lom_technical->getSizeHuman(TRUE);
      $form['technical']['size'] = array(
        '#type' => 'textfield',
        '#size' => 6,
        '#maxlength' => 5,
        '#default_value' => $size[0],
        '#title' => archibald_get_field_label('technical.size') . archibald_get_require_type_value('technical.size') . archibald_get_field_help('technical.size'),
        '#attributes' => array('class' => array('technical_size diego')),
      );

      $ddm_opts = array(
        'b' => 'Bytes',
        'kb' => 'KB (KiloBytes)',
        'mb' => 'MB (MegaBytes)',
        'gb' => 'GB (GigaBytes)',
        'tb' => 'TB (TeraBytes)'
      );
      $form['technical']['sizeunit'] = array(
        '#type' => 'select',
        '#default_value' => $size[1],
        '#options' => $ddm_opts,
        '#attributes' => array('class' => array('technical_size')),
      );
    }

    if (archibald_check_field_permission('technical otherPlattformRequirements')) {
      $default_value = '';
      $lang_string = $lom_technical->getOtherPlattformRequirements();
      if ($lang_string instanceof ArchibaldLomDataLangString) {
        $default_value = $lang_string->getString($language);
      }
      $form['technical']['otherPlattformRequirements'] = array(
        '#type' => 'textarea',
        '#title' => archibald_get_field_label('technical.otherPlattformRequirements') .
          archibald_get_require_type_value('technical.otherPlattformRequirements') .
          archibald_get_field_help('technical.otherPlattformRequirements'),
        '#default_value' => $default_value,
        '#rows' => 5,
        '#attributes' => array(
          'class' => array(
            'lang_string_autocomplete',
            'lang_string',
          ),
          'archibald_help_ident' => 'technical.otherPlattformRequirements',
        ),
      );
    }
  }
}

/**
 * add new empty location
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_technical_add_empty_location(&$form, &$form_state) {

  $i = 0;
  foreach (array_keys($form['technical']['location']) as $k) {
    if (preg_match('/^location_/', $k)) {
      $i++;
    }
  }

  $form['technical']['location']['location_' . $i] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#weight' => $i,
  );

  $form['technical']['location']['location_' . $i]['type'] = array(
    '#type' => 'radios',
    '#title' => '',
    '#options' => array('url' => t('URL'), 'text' => t('Text')),
    '#default_value' => 'url',
    '#attributes' => array('class' => array('location_type')),
  );

  $form['technical']['location']['location_' . $i]['value'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => '',
    '#size' => 80,
    '#attributes' => array('class' => array('location_value')),
  );

}

/**
 * ajax menu callback
 * place new empty location fields on bottom of location list
 *
 * @return string
 */
function archibald_ajax_content_form_technical_add_empty_location($form, $form_state) {
  return $form['technical']['location'];
}

/**
 * Submit handler for the "Add more" button.
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_add_technical_location_submit($form, &$form_state) {
  $form_state['lindex']++;
  $form_state['rebuild'] = TRUE;
}


/**
 * Get the options for the format DDM
 *
 */
function archibald_get_technical_format_options() {
  $ret = array(
    '' => ''
  );
  $formats = archibald_get_taxonomy_options('technical_format', NULL, TRUE, TRUE);

  $prepend = '';
  foreach ($formats as $k => $v) {
    $a = explode('|', $k);
    $depth = array_pop( $a );
    $k = array_pop( $a );
    if( $k == 'non_digital' ) {
      $ret['non digital'] = $v;
      continue;
    }

    if ($depth == 0) {
      $prepend_k = $k . '/';
      $prepend_v = $v . '/';
      continue;
    }

    $k = $prepend_k . $k;
    $ret[$k] = $prepend_v . $v;
  }
  return $ret;
}


/**
 * theme form block 'technical'
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_technical($form) {
  $form = reset($form);

  $output = '';
  $output .= '<div>';
  $output .= '<label class="htab_title">' . archibald_get_field_label('technical') . '</label>';
  $output .= '<table class="archibald-technical"><tr>';
  $output .= '<td colspan="2">';
  $output .= drupal_render($form['location']);
  $output .= '</td>';
  $output .= '</tr><tr>';
  $output .= '<td>';
  $output .= drupal_render($form['format']);
  $output .= '</td>';
  $output .= '<td width="100%">';
  $output .= drupal_render($form['size']);
  $output .= drupal_render($form['sizeunit']);
  $output .= '</td>';
  $output .= '</tr><tr>';
  $output .= '<td colspan="2">';
  $output .= drupal_render($form['otherPlattformRequirements']);
  $output .= '</td>';
  $output .= '</tr></table>';
  $output .= '</div>';

  return $output;
}

/*
  ..######..##.....##.########..##.....##.####.########
  .##....##.##.....##.##.....##.###...###..##.....##...
  .##.......##.....##.##.....##.####.####..##.....##...
  ..######..##.....##.########..##.###.##..##.....##...
  .......##.##.....##.##.....##.##.....##..##.....##...
  .##....##.##.....##.##.....##.##.....##..##.....##...
  ..######...#######..########..##.....##.####....##...
*/
/**
 * save handler for lom technical form part
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 */
function archibald_content_form_technical_submit($form, &$form_state, &$lom) {
  $lom_technical = $lom->getTechnical();
  if (!($lom_technical instanceof ArchibaldLomDataTechnical)) {
    $lom_technical = new ArchibaldLomDataTechnical();
  }

  ##### handle format
  if (archibald_check_field_permission('technical format')) {
    $lom_technical->setFormat( $form_state['values']['technical']['format'] );
  }

  ##### handle size
  if (archibald_check_field_permission('technical size')) {
    $size = $form_state['values']['technical']['size'];

    $multiplier = 1024;
    switch( $form_state['values']['technical']['sizeunit'] ) {
      case 'kb':
        $size *= $multiplier;
      break;
      case 'mb':
        $size *= $multiplier * $multiplier;
      break;
      case 'gb':
        $size *= $multiplier * $multiplier * $multiplier;
      break;
      case 'tb':
        $size *= $multiplier * $multiplier *$multiplier * $multiplier;
      break;
    }
    $lom_technical->setSize( round($size) );
  }

  ##### handle location(s)
  if (archibald_check_field_permission('technical location')) {
    $locations = array();
    foreach ($form_state['values']['technical']['location'] as $i => $location) {

      if (!is_array($location) || empty($location['value'])) {
        continue;
      }

      $new_location = new ArchibaldLomDataLocation();
      $new_location->setValue($location['value']);
      $locations[] = $new_location;
    }
    $lom_technical->setLocation($locations);
  }

  ##### handle other platform requirements
  if (archibald_check_field_permission('technical otherPlattformRequirements')) {
    $lom_technical->setOtherPlattformRequirements(
      archibald_langstring_submit(
        $lom_technical->getOtherPlattformRequirements(),
        $form_state['values']['language'],
        $form_state['values']['technical']['otherPlattformRequirements'],
        FALSE
      )
    );
  }

  $lom->setTechnical($lom_technical);
}

