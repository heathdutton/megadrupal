<?php

/**
 * @file
 * Fortissimmo pand pictures handler
 *
 * @TODO: try to inherit from views_handler_field_picture_file
 */

class views_handler_field_pand_picture_file extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);
    // Retrieve Fortissimmo ID if we need to make this a link
    $this->additional_fields['fortissimmo'] = array('table' => $view->base_table, 'field' => 'fortissimmo');

    // Get alt text
    $this->additional_fields['picture_comment'] = array('table' => $view->base_table, 'field' => 'picture_comment');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['image_style'] = array('default' => 'none');
    $options['fortissimmo_title_make_link'] = array('default' => TRUE, 'bool' => TRUE);
    $options['fortissimmo_use_lightbox'] = array('default' => FALSE, 'bool' => TRUE);
    $options['fortissimmo_title_as_alt'] = array('default' => TRUE, 'bool' => TRUE);

    return $options;
  }

  /**
   * Provide formatter option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = $this->options;

    // Get image styles
    $styles = image_styles();
    $presets['none'] = 'none';
    if ($styles) {
      foreach ($styles as $style_name => $style_value) {
        $presets[$style_name] = $style_value['name'];
      }
    }

    $form['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#options' => $presets,
      '#required' => TRUE,
      '#default_value' => $options['image_style'],
      '#weight' => 4,
    );

    $form['fortissimmo_title_make_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link to property detail page'),
      '#default_value' => $options['fortissimmo_title_make_link'],
    );

    $form['fortissimmo_use_lightbox'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add lightbox rel attribute'),
      '#description' => t('Make sure to uncheck "Link to property detail page" above for this to work'),
      '#default_value' => $options['fortissimmo_use_lightbox'],
    );

    $form['fortissimmo_title_as_alt'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use property title as alt/link text'),
      '#default_value' => $options['fortissimmo_title_as_alt'],
    );
  }


  function render($values) {
    $output = NULL;
    $options = $this->options;
    $make_link = $options['fortissimmo_title_make_link'];
    $title_as_alt = $options['fortissimmo_title_as_alt'];
    $preset_name = $options['image_style'];
    $use_lightbox = $options['fortissimmo_use_lightbox'];
    $filename = $values->{$this->field_alias};

    if ($filename) {
      $alt = $title = $values->{$this->aliases['picture_comment']};

      if (empty($alt) && $title_as_alt) {
        $fortissimmo_id = $values->{$this->aliases['fortissimmo']};
        // @TODO: try to fix this with a JOIN on the base table instead...
        $pictures_table = $this->view->base_table;
        $pand_table = str_replace('_pictures', '', $pictures_table);

        $result = db_query("SELECT reference, title FROM {" . $pand_table . "} WHERE fortissimmo = :fortissimmo", array(':fortissimmo' => $fortissimmo_id));
        if (!empty($result)) {
          $pand_info = array_shift($result);
          if (!empty($pand_info->title)) {
             $alt = $title = $pand_info->title;
           }
           else {
             $alt = $title = $pand_info->reference;
           }

        }
      }

      $file = fortissimmo_create_asset_url($filename, 'images');
      if ($preset_name == "none") {
        $variables = array(
          'path' => $file,
          'alt' => $alt,
          'title' => $title,
        );
        $output = theme('image', $variables);
      }
      else {
        $variables = array(
          'style_name' => $preset_name,
          'path' => $file,
          'alt' => $alt,
          'title' => $title,
        );
        $output = theme('image_style', $variables);
      }

      if (isset($this->aliases['fortissimmo']) && $make_link) {
        $fortissimmo_id = $values->{$this->aliases['fortissimmo']};
        $output = fortissimmo_generate_link($output, $fortissimmo_id, array('html' => TRUE));
      }
      elseif ($use_lightbox) {
        $file = file_create_url($file);
        $output = l($output, $file, array('html' => TRUE, 'attributes' => array('rel' => 'lightshow[photos][' . $title . ']')));
      }
    }
    return $output;
  }
}