<?php

/**
 * The subclass adds basic field and formatter info,
 * for field-specific subclasses to use if they need to.
 *
 * Fields could extend this class if they want field and formatter handling
 * but don't want the multiple value grouping options created by
 * content_handler_field_multiple.
 */
class webcams_handler_field extends views_handler_field {
  function options(&$options) {
    parent::options($options);

    $options['multiple'] = array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    );
    
    $options['dimensions'] = array(
      'width' => WEBCAMS_DEFAULT_WIDTH,
      'height' => WEBCAMS_DEFAULT_HEIGHT,
    );
  }

  /**
   * Provide formatter option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    $options = $this->options;

    $form['multiple'] = array(
      '#access' => $field['multiple'],
      '#weight' => 1,
    );
    $form['multiple']['group'] = array(
      '#title' => t('Group multiple values'),
      '#type' => 'checkbox',
      '#default_value' => $options['multiple']['group'],
    );
    // Make the string translatable by keeping it as a whole rather than
    // translating prefix and suffix separately.
    list($prefix, $suffix) = explode('@count', t('Show @count value(s)'));
    $form['multiple']['multiple_number'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#field_prefix' => $prefix,
      '#field_suffix' => $suffix,
      '#default_value' => $options['multiple']['multiple_number'],
      '#prefix' => '<div class="container-inline">',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-multiple-group' => array(TRUE)),
    );
    list($prefix, $suffix) = explode('@count', t('starting from @count'));
    $form['multiple']['multiple_from'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#field_prefix' => $prefix,
      '#field_suffix' => $suffix,
      '#default_value' => $options['multiple']['multiple_from'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-multiple-group' => array(TRUE)),
    );
    $form['multiple']['multiple_reversed'] = array(
      '#title' => t('Reversed (start from last values)'),
      '#type' => 'checkbox',
      '#default_value' => $options['multiple']['multiple_reversed'],
      '#suffix' => '</div>',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-multiple-group' => array(TRUE)),
    );
    
    $form['dimensions'] = array(
      '#weight' => 2,
    );
    $form['dimensions']['title'] = array(
      '#value' => '<div class="form-item"><label for="edit-options-dimensions-width">'. t('Dimensions') .':</label></div>',
    );
    $form['dimensions']['width'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#default_value' => $options['dimensions']['width'],
      '#prefix' => '<div class="container-inline">',
      '#suffix' => ' x ',
    );
    $form['dimensions']['height'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#default_value' => $options['dimensions']['height'],
      '#suffix' => '</div>',
    );
  }

  function options_validate($form, &$form_state) { }

  function render($values) {
    $options = $this->options;

    // $values will be used as a fake $node object;
    // we provide a build_mode for rendering.
    $values->build_mode = NODE_BUILD_NORMAL;

    $js = '';
    $output = '';
    $node = node_load($values->nid);
    
    if (!empty($fielddata['options'])) {
      $dimensions = explode('x', $fielddata['options'], 2);
      if (!is_numeric($dimensions[0]) || !is_numeric($dimensions[1])) {
        unset($dimensions);
      }
    }
    
    if ($options['multiple']['group']) {
      if ((!empty($options['multiple']['multiple_from']) || $options['multiple']['multiple_from'] === 0 || $options['multiple']['multiple_from'] === '0') && (!empty($options['multiple']['multiple_number']) || $options['multiple']['multiple_number'] === 0 || $options['multiple']['multiple_number'] === '0')) {
        $start = $options['multiple']['multiple_from'];
        $count = min($options['multiple']['multiple_from'] + $options['multiple']['multiple_number'], count($node->webcams));
      }
      else {
        $start = 0;
        $count = count($node->webcams);
      }
    }
    else {
      $start = 0;
      $count = 1;
    }
    
    if ($options['multiple']['multiple_reversed']) {
      // TODO
    }
    
    for ($i = $start; $i < $count; $i++) {
      $webcam = $node->webcams[$i];
      if (empty($webcam)) {
        continue;
      }
      
      if (isset($options['dimensions']['width']) && isset($options['dimensions']['height'])) {
        $webcam->width = $options['dimensions']['width'];
        $webcam->height = $options['dimensions']['height'];
      }
  
      $delay  = $webcam->delay * 1000;
      $errmsg = t('Webcam <em>@name</em> has timed out...', array('@name' => $webcam->name));
  
      $js .= "\n$.webcam.init('webcams{$webcam->nid}-webcam{$webcam->wid}', '{$webcam->name}', '{$webcam->url}', '{$webcam->default_url}', {$webcam->width}, {$webcam->height}, {$delay}, {$webcam->timeout}, 'webcams{$webcam->nid}-content', '{$errmsg}');";
      
      $output .= theme('webcams_webcam', $webcam);
    }
    $output .= '<div id="webcams'. $node->nid .'-content" class="webcams-content"></div>'."\n";
  
    drupal_add_css(drupal_get_path('module', 'webcams') .'/webcams.css');
    drupal_add_js(drupal_get_path('module', 'webcams') .'/webcams.js');
    drupal_add_js($js, 'inline', 'footer', FALSE);
    
    return $output;
  }

}