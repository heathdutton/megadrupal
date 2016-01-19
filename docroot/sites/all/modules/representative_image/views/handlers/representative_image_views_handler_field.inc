<?php

/**
 * @file
 * Definition of representative_image_views_handler_field_representative_image.
 */

/**
 * A handler to provide proper displays for the representative image.
 *
 * @ingroup views_field_handlers
 */
class representative_image_views_handler_field extends views_handler_field {

  function query() {
    // Do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['image_style'] = array('default' => '');
    $options['image_link'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $image_styles = image_style_options(FALSE);
    $form['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#empty_option' => t('None (original image)'),
      '#default_value' => isset($this->options['image_style']) ? $this->options['image_style'] : NULL,
      '#options' => $image_styles,
    );

    $link_types = array(
      'content' => t('Content'),
      'file' => t('File'),
    );
    $form['image_link'] = array(
      '#title' => t('Link image to'),
      '#type' => 'select',
      '#empty_option' => t('Nothing'),
      '#default_value' => isset($this->options['image_link']) ? $this->options['image_link'] : NULL,
      '#options' => $link_types,
    );
  }

  function get_value($values, $field = NULL) {
    if (isset($values->{$field})) {
      return $values->{$field};
    }
  }

  function render($values, $entity = NULL, $type = '') {
    if (is_null($entity) || empty($type)) {
      return '';
    }

    $bundle = _representative_image_extract_bundle($type, $entity);
    if ($field = representative_image_get_field($type, $bundle)) {
      $image_field = field_view_field($type, $entity, $field);

      // Get the fid. Image and Media handle this slightly differently.
      $fid = '';
      if (isset($image_field[0]['#file']->fid)) {
        $fid = $image_field[0]['#file']->fid;
      }
      elseif (isset($image_field[0]['#item']['fid'])) {
        $fid = $image_field[0]['#item']['fid'];
      }

      if (!empty($fid)) {
        $image = file_load($fid);
        $image_wrapper = entity_metadata_wrapper('file', $image);

        // When using the media module there is a chance that this image will be
        // a video, or text file, etc. In that case we return the default render
        // of that field.
        if (isset($image->type) && $image->type != 'image') {
          $image_field['#label_display'] = 'hidden';
          return drupal_render($image_field);
        }

        $settings = array(
          'item' => array(
            'uri' => $image->uri,
          ),
          'image_style' => $this->options['image_style'],
        );

        // Some image fields have alt and title fields available.
        if (isset($image_wrapper->field_file_image_alt_text)) {
          $settings['item']['alt'] = $image_wrapper->field_file_image_alt_text->value();
        }
        if (isset($image_wrapper->field_file_image_title_text)) {
          $settings['item']['alt'] = $image_wrapper->field_file_image_title_text->value();
        }

        switch ($this->options['image_link']) {
          case 'content':
            $settings['path'] = array(
              'path' => 'node/' . $node->nid,
            );
            break;
          case 'file':
            $settings['path'] = array(
              'path' => $image->uri,
            );
            break;
        }

        return theme('image_formatter', $settings);
      }
    }
    else {
      return '';
    }
  }

}
