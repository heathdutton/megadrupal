<?php

/**
 * Internal function find the image style name out of the selected option
 * within the rules conditions and actions provided by this module.
 */
function _imagestyles_rules_style_name($option) {
  if (substr($option, 0, 2) == 'n:') {
    return substr($option, 2);
  }
  else {
    $styles = image_styles();
    $id = substr($option,2);
    foreach ($styles as $style) {
      if (isset($style['isid']) && $style['isid'] == $id) {
        return $style['name'];
      }
    }
  }
}

/**
 * Implementation of hook_rules_data_type_info().
 */
function imagestyles_rules_rules_data_info() {
  return array(
    'imagestyles_rules_image' => array(
      'label' => t('Image'),
      'group' => t('Image Styles'),
    ),
    'imagestyles_rules_dimensions' => array(
      'label' => t('Dimensions'),
      'group' => t('Image Styles'),
    ),
  );
}

/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function imagestyles_rules_rules_event_info() {
  return array(
    'imagestyles_rules_effect' => array(
      'label' => t('Image style rules effect is being applied'),
      'module' => 'imagestyles_rules',
      'group' => t('Image Styles'),
      'variables' => array(
        'original_image' => array('type' => 'imagestyles_rules_image', 'label' => t('Original image')),
        'image' => array('type' => 'imagestyles_rules_image', 'label' => t('Image')),
        'dimensions' => array('type' => 'imagestyles_rules_dimensions', 'label' => t('Dimensions')),
        'image_style' => array('type' => 'text', 'label' => t('Image style being used')),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function imagestyles_rules_rules_condition_info() {
  $conditions = array();

  $conditions['imagestyles_rules_condition_style'] = array(
    'label' => t('Image style being used'),
    'parameter' => array(
      'image_style' => array(
        'type' => 'text',
        'label' => t('Image style used'),
        'restriction' => 'selector',
      ),
      'image_style_check' => array(
        'type' => 'text',
        'label' => t('Image style'),
        'restriction' => 'input',
        'options list' => 'imagestyles_rules_action_style_options',
      ),
    ),
    'group' => t('Image Styles'),
  );

  $conditions['imagestyles_rules_condition_width'] = array(
    'label' => t('Width comparison'),
    'parameter' => array(
      'original_image' => array(
        'type' => 'imagestyles_rules_image',
        'label' => t('Image'),
        'restriction' => 'selector',
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The comparison operator.'),
        'optional' => TRUE,
        'default value' => '==',
        'options list' => 'rules_condition_data_is_operator_options',
        'restriction' => 'input',
      ),
      'value' => array(
        'type' => 'decimal',
        'label' => t('Data value'),
        'description' => t('The value to compare the data with.'),
        'default mode' => 'input',
      ),
    ),
    'group' => t('Image Styles'),
  );

  $conditions['imagestyles_rules_condition_height'] = array(
    'label' => t('Height comparison'),
    'parameter' => array(
      'original_image' => array(
        'type' => 'imagestyles_rules_image',
        'label' => t('Image'),
        'restriction' => 'selector',
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The comparison operator.'),
        'optional' => TRUE,
        'default value' => '==',
        'options list' => 'rules_condition_data_is_operator_options',
        'restriction' => 'input',
      ),
      'value' => array(
        'type' => 'decimal',
        'label' => t('Data value'),
        'description' => t('The value to compare the data with.'),
        'default mode' => 'input',
      ),
    ),
    'group' => t('Image Styles'),
  );

  $conditions['imagestyles_rules_condition_aspectratio'] = array(
    'label' => t('Aspect ratio comparison'),
    'parameter' => array(
      'original_image' => array(
        'type' => 'imagestyles_rules_image',
        'label' => t('Image'),
        'restriction' => 'selector',
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The comparison operator.'),
        'optional' => TRUE,
        'default value' => '==',
        'options list' => 'rules_condition_data_is_operator_options',
        'restriction' => 'input',
      ),
      'value' => array(
        'type' => 'decimal',
        'label' => t('Value'),
        'description' => t('The value to compare the data with.'),
        'default mode' => 'input',
      ),
    ),
    'group' => t('Image Styles'),
  );

  return $conditions;
}

function imagestyles_rules_condition_style($image_style, $image_style_check) {
  $image_style_check = _imagestyles_rules_style_name($image_style_check);
  return ($image_style == $image_style_check);
}

/**
 * Condition: Compare width.
 */
function imagestyles_rules_condition_width($image, $op, $value) {
  module_load_include('inc', 'rules', 'modules/data.eval');
  $data = $image->info['width'];
  return rules_condition_data_is($data, $op, $value);
}

/**
 * Condition: Compare height.
 */
function imagestyles_rules_condition_height($image, $op, $value) {
  module_load_include('inc', 'rules', 'modules/data.eval');
  $data = $image->info['height'];
  return rules_condition_data_is($data, $op, $value);
}

/**
 * Condition: Compare aspect ratio.
 */
function imagestyles_rules_condition_aspectratio($image, $op, $value) {
  module_load_include('inc', 'rules', 'modules/data.eval');
  $data = $image->info['width']/$image->info['height'];
  return rules_condition_data_is($data, $op, $value);
}

/**
 * Implements hook_rules_action_info().
 */
function imagestyles_rules_rules_action_info() {
  return array(
    'imagestyles_rules_action_style' => array(
      'label' => t('Use an image style'),
      'group' => t('Image Styles'),
      'parameter' => array(
        'image' => array('type' => 'imagestyles_rules_image', 'label' => t('Image.')),
        'dimensions' => array('type' => 'imagestyles_rules_dimensions', 'label' => t('Dimensions.')),
        'image_style' => array(
          'type' => 'text',
          'label' => t('Image style to use'),
          'restriction' => 'input',
          'options list' => 'imagestyles_rules_action_style_options',
        ),
      ),
      'module' => 'imagestyles_rules',
    ),
  );
}

/**
 * Option callback: Image style option list.
 */
function imagestyles_rules_action_style_options($element, $name) {
  $styles = image_styles();
  $options = array();
  foreach ($styles as $style) {
    // naming convention to prevent possible conflicts
    $id = 'n:'.$style['name'];
    if (isset($style['isid'])) {
      $id = 'i:'.$style['isid'];
    }
    $options[$id] = $style['label'];
  }
  return $options;
}

/**
 * Action: Execute an image style to an image object
 *
 * For simplicity of use, we use the same action for both image and dimension
 * transformations
 */
function imagestyles_rules_action_style($image, $dimensions, $image_style, $settings, $state, $action, $name) {
  $image_style = _imagestyles_rules_style_name($image_style);

  if ($image_style == $state->variables['image_style']) {
    drupal_set_message('The same image style used by rules invokes the event, doing nothing to prevent an infinite loop. Please fix.', 'error');
  }

  // We use a dummy object to detect one or the other
  if (isset($image->imagestyles_rules) && $image->imagestyles_rules == 'dimensions') {
    $dimensions_array = (array) $dimensions;
    image_style_transform_dimensions($image_style, $dimensions_array);
    foreach ($dimensions_array as $k => $v) {
      $dimensions->$k = $v;
    }
  }
  else {
    $styles = image_styles();
    $style = $styles[$image_style];

    foreach ($style['effects'] as $effect) {
      image_effect_apply($image, $effect);
    }
  }
}
