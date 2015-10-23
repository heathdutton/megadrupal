<?php
/**
 * @file
 */
function _remove_base($path) {
  $url = parse_url($path);
  if (isset($url['scheme']) && $url['scheme'] == 'http') {
     return $path;
  }
  $file_directory_path = variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files');
  // Remove the base_path
  $path = substr($path, strlen(base_path()));
  // remove the file_directory_path
  $path = substr($path, strlen($file_directory_path) + 1);  
  
  return $path;
}

function _add_base($path) {
  $url = parse_url($path);
  if (isset($url['scheme']) && $url['scheme'] == 'http') {
     return $path;
  }
  $file_directory_path = variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files');
  // Add the base_path
  $path = $file_directory_path . "/" . $path;
   
  return $path;
}


class more_boxes_imce extends boxes_box {
  /**
   * Implementation of boxes_box::options_defaults().
   */
  public function options_defaults() {
    return array(
      'link_url' => '',
      'imce_image_path' => '',
      'imce_flash_path' => '',
    );
  }

  /**
   * Implementation of boxes_box::options_form().
   */
  public function options_form(&$form_state) {
    $form = array();
    $form['link_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Link URL'),
      '#default_value' => $this->options['link_url'],
    );
    $form['imce_image_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Image path'),
      '#default_value' => $this->options['imce_image_path'],
      '#description' => 'Use http:// for external path.', 
      '#suffix' => '<div class="imce-browser-ph"><a href="#">' . t('Browse') . '</a></div>',
    );
    $form['imce_flash_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Flash path'),
      '#default_value' => $this->options['imce_flash_path'],
      '#description' => 'Use http:// for external path.',   
      '#suffix' => '<div class="imce-browser-ph"><a href="#">' . t('Browse') . '</a></div>',
    );

    return $form;
  }
  
  
  /**
   * Implementation of boxes_box::save().
   */
  public function save() {
    if (empty($this->delta)) {
      throw new Exception(t('Cannot save box without a specified delta.'));
    }
    self::reset();
    
    // Remove the base path before saving.
    if (!empty($this->options['imce_image_path'])) {
      $this->options['imce_image_path'] = _remove_base($this->options['imce_image_path']);
    }
    
    // Remove the base path before saving.
    if (!empty($this->options['imce_flash_path'])) {
      $this->options['imce_flash_path'] = _remove_base($this->options['imce_flash_path']);
    }
    
    $existing = boxes_box_load($this->delta);
    if ($existing && ($existing->export_type & EXPORT_IN_DATABASE)) {
      drupal_write_record('box', $this, array('delta'));
    }
    else {
      drupal_write_record('box', $this);
    }
    $this->new = FALSE;
    self::reset();
    module_exists('context') ? context_invalidate_cache() : NULL;
  }

  /**
   * Implementation of boxes_box::render().
   */
  public function render() {
    $title = isset($this->title) ? check_plain($this->title) : NULL;
    $block['title'] = $title;
    $block['subject'] = NULL;
    $block['delta'] = $this->delta;
    $link_options = array('html' => TRUE);
       
    if ($this->options['imce_flash_path']) {
      $swf_path = _add_base($this->options['imce_flash_path']);
      $swf_path = url($swf_path, array('absolute' => TRUE));
      $content = '<embed name="plugin" src="' . $swf_path . '" type="application/x-shockwave-flash">';
    }
    elseif ($this->options['imce_image_path']) {
      $image_path = _add_base($this->options['imce_image_path']); 
      $image = theme('image', array('path' => $image_path, 'alt' => $title, 'title' => $title));
      $content = l($image, $this->options['link_url'], $link_options);
    }
    
    $block['content'] = isset($content) ? $content : "";
    return $block;
  }
}