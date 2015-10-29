<?php

/**
 * @file
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Image url field handler.
 */

class imagepicker_views_handler_field_image_url extends views_handler_field {

  // options should be full, thumbnail plus presets
  function option_definition() {
    $options = parent::option_definition();
    $options['presets'] = array('default' => '');
    $options['img_size'] = array('default' => 'full');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    if (module_exists('image') && imagepicker_variable_get('imagepicker_image_enable', 0)) {
      $opts = image_style_options();
      $form['presets'] = array(
        '#type' => 'select',
        '#title' => t('Presets'),
        '#options' => $opts,
        '#default_value' => $this->options['presets'],
      );
    }
    $form['img_size'] = array(
      '#type' => 'select',
      '#title' => t('Size'),
      '#description' => t('Size if not using a preset'),
      '#options' => array('full' => t('Full'), 'thumb' => t('Thumbnail')),
      '#default_value' => $this->options['img_size'],
    );
  }

  // return a full url from img_id
  function render($values) {

    $preset = FALSE;
    if (module_exists('image') && imagepicker_variable_get('imagepicker_image_enable', 0)) {
      $preset = $this->options['presets'];
    }
    $img_size = $this->options['img_size'];

    $img_id = $values->{$this->field_alias};
    $query = db_select('imagepicker', 'i');
    $query->fields('i', array('uid', 'img_name'));
    $query->range(0, 1);
    $query->join('users', 'u', 'i.uid = u.uid');
    $query->condition('i.img_id', $img_id);
    $img = $query->execute()->fetchObject();
    // $img is now object

    if (module_exists('image') && imagepicker_variable_get('imagepicker_image_enable', 0) && $preset) {
      $imgpath = imagepicker_get_image_path($img, $img_size, array('uid' => $img->uid), TRUE);
      $imgpath = preg_replace("~__PRESET__~", $preset, $imgpath);
    }
    else {
      $imgpath = imagepicker_get_image_path($img, $img_size, array('uid' => $img->uid));
    }
    return $imgpath;

  }
}
