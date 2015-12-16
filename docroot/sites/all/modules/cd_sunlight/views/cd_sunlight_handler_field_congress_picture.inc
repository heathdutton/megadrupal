<?php

/**
 * @file
 * Views handlers for cd_sunlight.
 */

/**
 * Field handler to fetch the congress member's photo from Bioguide.
 *
 * @ingroup views_field_handlers
 */
class cd_sunlight_handler_field_congress_picture extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    if (function_exists('image_styles')) {
      $formats = array('' => '- None -');
      foreach (image_styles() as $style_key => $style) {
        $formats[$style_key] = $style['name'];
      }
      $form['format'] = array(
        '#title' => t('Format'),
        '#type' => 'select',
        '#default_value' => $this->options['format'],
        '#options' => $formats,
      );
    }
  }

  /**
   * Add all the fields that we need.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['bioguide_id'] = 'bioguide_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if (empty($values->{$this->aliases['bioguide_id']})) {
      return '';
    }

    $img['path'] = cd_sunlight_bioguide_image_url($values->{$this->aliases['bioguide_id']});
    $img['alt'] = $img['title'] = '';
    if ($values->cd_sunlight_legislators_lastname) {
      $img['title'] = "$values->cd_sunlight_legislators_title. $values->cd_sunlight_legislators_firstname $values->cd_sunlight_legislators_lastname";
      $img['alt'] = "Photo of $img[title]";
    }
    // If the image style is resizing the image, add the dimensions to the img tag.
    // This is a front-end performance improvement.  But it will also prevent
    // layout issues if Bioguide is being slow and thus we get images directly
    // from them and not the cache.
    if (!empty($this->options['format']) && function_exists('image_styles')) {
      $img['style_name'] = $this->options['format'];
      $styles = image_styles();
      foreach ($styles[$this->options['format']]['effects'] as $effect) {
        foreach (array('width', 'height') as $dimension) {
          if ($effect['data'][$dimension] && strpos($effect['data'][$dimension], '%') === FALSE) {
            $img[$dimension] = $effect['data'][$dimension];
          }
        }
      }
    }
    if (!empty($img['style_name'])&& strpos($img['path'], 'http') !== 0) {
      return theme('imagecache', $img);
    }
    else {
      return theme('image', $img);
    }
  }

}
