<?php

/**
 * Helper function to build the form array for areas
 */
function _regionarea_build_area_forms($area, $area_data, $available_tags) {
  $form = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'area--' . $area,
    	'class' => array(
        'element-wrapper',
        'drag-area-source',
        'drag-region-target',
        'col-' . $area_data['#column'],
        'element-wrapper-area',
      ),
    ),
  );

  if ($area_data['#clearfix'] == 1) {
    $form['#attributes']['class'][] = 'clear clearfix';
  }

  if ($area_data['#newrow'] == 1) {
    $form['#attributes']['class'][] = 'newrow';
  }

  if ($area_data['#lastrow'] == 1) {
    $form['#attributes']['class'][] = 'lastrow';
  }

  // Build area configuration forms
  $form['area-element-title--' . $area] = array(
    '#type' => 'markup',
    '#markup' => '<h5 class="region-title">' . $area_data['#name'] . '</h5>',
  );

  $form['area'] = array(
    '#tree' => TRUE,
  );

  $form['area'][$area] = array(
    '#type' => 'container',
    '#attributes' => array(
    	'class' => array(
        'region-configuration',
      ),
    ),
  );

  $form['area'][$area]['name'] = array(
  	'#type' => 'value',
    '#value' => vtcore_get_default_value('#name', $area_data, $area),
  );

  $form['area'][$area]['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => vtcore_get_default_value('#enabled', $area_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-enabled'
      ),
    ),
  );

  $form['area'][$area]['disable_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable Empty'),
    '#default_value' => vtcore_get_default_value('#disable_empty', $area_data, 0),
  );

  $form['area'][$area]['wrapper'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wrapper'),
    '#default_value' => vtcore_get_default_value('#wrapper', $area_data, 0),
  );

  $form['area'][$area]['clearfix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clearfix'),
    '#default_value' => vtcore_get_default_value('#clearfix', $area_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-clear'
      ),
    ),
  );

  $form['area'][$area]['newrow'] = array(
    '#type' => 'checkbox',
    '#title' => t('New row'),
    '#default_value' => vtcore_get_default_value('#newrow', $area_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-newrow'
      ),
    ),
  );

  $form['area'][$area]['lastrow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Last row'),
    '#default_value' => vtcore_get_default_value('#lastrow', $area_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-lastrow'
      ),
    ),
  );

  $column = range(0, 12);
  unset($column[0]);

  $form['area'][$area]['column'] = array(
    '#type' => 'select',
    '#title' => t('column'),
    '#options' => $column,
    '#default_value' => vtcore_get_default_value('#column', $area_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-column'
      ),
    ),
  );

  $form['area'][$area]['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 19,
    '#default_value' => vtcore_get_default_value('#weight', $area_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-weight',
      ),
    ),
  );

  $form['area'][$area]['tag'] = array(
    '#type' => 'select',
    '#title' => t('HTML Tag'),
    '#options' => $available_tags,
    '#default_value' => vtcore_get_default_value('#tag', $area_data, 'div'),
    '#attributes' => array(
      'class' => array(
        'element-tag',
      ),
    ),
  );

  $form['area'][$area]['tag_wrapper'] = array(
    '#type' => 'select',
    '#title' => t('Wrapper HTML Tag'),
    '#options' => $available_tags,
    '#default_value' => vtcore_get_default_value('#tag_wrapper', $area_data, 'div'),
    '#attributes' => array(
      'class' => array(
        'element-tag',
      ),
    ),
  );
  // Let other plugin connect with this form
  vtcore_alter_process('regionarea_vtcore_area_settings', $form, $area, $area_data);

  return $form;
}

/**
 * Helper function to build the regions form array
 */
function _regionarea_build_region_forms($region, $region_data, $all_areas, $available_tags) {
  $form = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'region--' . $region,
    	'class' => array(
        'element-wrapper',
        'drag-region-source',
        'drag-block-target',
        'col-' . $region_data['#column'],
  			'element-wrapper-region',
      ),
    ),
  );

  if ($region_data['#clearfix'] == 1) {
    $form['#attributes']['class'][] = 'clear clearfix';
  }

  if ($region_data['#newrow'] == 1) {
    $form['#attributes']['class'][] = 'newrow';
  }

  if ($region_data['#lastrow'] == 1) {
    $form['#attributes']['class'][] = 'lastrow';
  }

  // Build area configuration forms
  $form['region-element-title--' . $region] = array(
    '#type' => 'markup',
    '#markup' => '<h5 class="region-title">' . $region_data['#name'] . '</h5>',
  );

  $form['region'] = array(
    '#tree' => TRUE,
  );

  $form['region'][$region] = array(
    '#type' => 'container',
    '#attributes' => array(
    	'class' => array(
        'region-configuration',
      ),
    ),
  );

  $form['region'][$region]['name'] = array(
  	'#type' => 'value',
    '#value' => vtcore_get_default_value('#name', $region_data, $region),
  );

  $form['region'][$region]['parent'] = array(
    '#type' => 'select',
    '#title' => t('Parent'),
    '#options' => $all_areas,
    '#default_value' => vtcore_get_default_value('#parent', $region_data, 'disabled'),
    '#attributes' => array(
      'class' => array(
        'element-parent'
      ),
    ),
  );

  $form['region'][$region]['disable_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable Empty'),
    '#default_value' => vtcore_get_default_value('#disable_empty', $region_data, 0),
  );

  $form['region'][$region]['clearfix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clearfix'),
    '#default_value' => vtcore_get_default_value('#clearfix', $region_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-clear'
      ),
    ),
  );

  $form['region'][$region]['newrow'] = array(
    '#type' => 'checkbox',
    '#title' => t('New row'),
    '#default_value' => vtcore_get_default_value('#newrow', $region_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-newrow'
      ),
    ),
  );

  $form['region'][$region]['lastrow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Last row'),
    '#default_value' => vtcore_get_default_value('#lastrow', $region_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-lastrow'
      ),
    ),
  );
  $column = range(0, 12);
  unset($column[0]);

  $form['region'][$region]['column'] = array(
    '#type' => 'select',
    '#title' => t('column'),
    '#options' => $column,
    '#default_value' => vtcore_get_default_value('#column', $region_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-column'
      ),
    ),
  );

  $form['region'][$region]['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 19,
    '#default_value' => vtcore_get_default_value('#weight', $region_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-weight',
      ),
    ),
  );

  $form['region'][$region]['tag'] = array(
    '#type' => 'select',
    '#title' => t('HTML Tag'),
    '#options' => $available_tags,
    '#default_value' => vtcore_get_default_value('#tag', $region_data, 'div'),
    '#attributes' => array(
      'class' => array(
        'element-tag',
      ),
    ),
  );

  // Let other plugin connect with this form
  vtcore_alter_process('regionarea_vtcore_region_settings', $form, $region, $region_data, $all_areas);

  return $form;
}

/**
 * Helper function to build block form array
 */
function _regionarea_build_block_forms($block, $block_data, $all_regions, $block_type) {
  $form = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'block--' . $block,
    	'class' => array(
        'element-wrapper',
        'drag-block-source',
        'col-' . $block_data['#column'],
        'element-wrapper-block',
      ),
    ),
  );

  if ($block_data['#clearfix'] == 1) {
    $form['#attributes']['class'][] = 'clearfix';
  }

  if ($block_data['#newrow'] == 1) {
    $form['#attributes']['class'][] = 'newrow';
  }

  if ($block_data['#lastrow'] == 1) {
    $form['#attributes']['class'][] = 'lastrow';
  }

  // Build area configuration forms
  $form['block-element-title--' . $block] = array(
    '#type' => 'markup',
    '#markup' => '<h5 class="region-title">' . $block_data['#name'] . '</h5>',
  );

  $form['block'] = array(
    '#tree' => TRUE,
  );

  $form['block'][$block] = array(
    '#type' => 'container',
    '#attributes' => array(
    	'class' => array(
        'region-configuration',
      ),
    ),
  );

  $form['block'][$block]['name'] = array(
    '#type' => 'value',
    '#value' => vtcore_get_default_value('#name', $block_data, $block),
  );

  $form['block'][$block]['block_type'] = array(
  	'#type' => 'value',
    '#value' => vtcore_get_default_value('#block_type', $block_data, $block_type),
  );

  $form['block'][$block]['parent'] = array(
    '#type' => 'select',
    '#title' => t('Parent'),
    '#options' => $all_regions,
    '#default_value' => vtcore_get_default_value('#parent', $block_data, 'disabled'),
    '#attributes' => array(
      'class' => array(
        'element-parent'
      ),
    ),
  );

  $form['block'][$block]['clearfix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clearfix'),
    '#default_value' => vtcore_get_default_value('#clearfix', $block_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-clear'
      ),
    ),
  );

  $form['block'][$block]['newrow'] = array(
    '#type' => 'checkbox',
    '#title' => t('New row'),
    '#default_value' => vtcore_get_default_value('#newrow', $block_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-newrow'
      ),
    ),
  );

  $form['block'][$block]['lastrow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Last row'),
    '#default_value' => vtcore_get_default_value('#lastrow', $block_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-lastrow'
      ),
    ),
  );

  $column = range(0, 12);
  unset($column[0]);

  $form['block'][$block]['column'] = array(
    '#type' => 'select',
    '#title' => t('Column'),
    '#options' => $column,
    '#default_value' => vtcore_get_default_value('#column', $block_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-column'
      ),
    ),
  );

  $form['block'][$block]['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 19,
    '#default_value' => vtcore_get_default_value('#weight', $block_data, 0),
    '#attributes' => array(
      'class' => array(
        'element-weight',
      ),
    ),
  );

  // Let other plugin connect with this form
  vtcore_alter_process('regionarea_vtcore_block_settings', $form, $block, $block_data, $all_regions);

  return $form;
}