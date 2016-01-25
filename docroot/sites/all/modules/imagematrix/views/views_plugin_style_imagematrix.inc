<?php

/**
 * @file
 * Views style plugin implementation for Image Matrix module.
 */

/**
 * Style plugin to render images in as an image matrix.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_imagematrix extends views_plugin_style {

  /**
   * Set default options
   */
  function option_definition() {
    $options = array();
    $options['block_width'] = array('default' => 600);
    $options['block_height'] = array('default' => NULL);
    $options['block_images_count'] = array('default' => 7);
    $options['image_padding'] = array('default' => 3);
    $options['reduce_duplicates'] = array('default' => FALSE);
    $options['randomize_layout'] = array('default' => FALSE);
    $options['cache_layout'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    $form['#imagematrix'] = TRUE;
    $form['block_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width of one block'),
      '#default_value' => $this->options['block_width'],
      '#required' => TRUE,
      '#element_validate' => array('views_element_validate_integer'),
      '#field_suffix' => 'px',
      '#size' => 5,
    );
    $form['block_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height of one block'),
      '#description' => t('This value is optional. If you leave it empty, you will most probably get blocks with different heights, but images will not be cropped. If you specify it on the other hand, all blocks will have the same height (which would be desirable for example when displaying them next to each other), but images could (and most probably will) be partially cropped.'),
      '#default_value' => $this->options['block_height'],
      '#element_validate' => array('views_element_validate_integer'),
      '#field_suffix' => 'px',
      '#size' => 5,
    );
    $form['block_images_count'] = array(
      '#type' => 'select',
      '#title' => t('Number of images in one block'),
      '#options' => drupal_map_assoc(range(2, 8)),
      '#default_value' => $this->options['block_images_count'],
      '#multiple' => FALSE,
      '#element_validate' => array('views_element_validate_integer'),
    );
    $form['image_padding'] = array(
      '#type' => 'textfield',
      '#title' => t('Image padding'),
      '#description' => t('Padding around each image.'),
      '#default_value' => $this->options['image_padding'],
      '#required' => TRUE,
      '#element_validate' => array('views_element_validate_integer'),
      '#field_suffix' => 'px',
      '#size' => 5,
    );
    $form['reduce_duplicates'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reduce duplicates'),
      '#description' => t('When more than one field is selected to use Image Matrix formatter, and any of those selected fields are configured to allow multiple values, it could happen that the same image will be displayed multiple times. Enabling this option should remedy this.'),
      '#default_value' => $this->options['reduce_duplicates'],
    );
    $form['randomize_layout'] = array(
      '#type' => 'checkbox',
      '#title' => t('Randomize generated layout'),
      '#description' => t('Enabling this option will try to randomize each block layout each time it is generated. This option is exclusive with "Cache generated layout".'),
      '#default_value' => $this->options['randomize_layout'],
    );
    $form['cache_layout'] = array(
      '#type' => 'checkbox',
      '#title' => t('Cache generated layout'),
      '#description' => t('Enabling this option will save once generated layout to the cache, which could slightly improve page loading time. On the other hand it will also cause images to appear in exactly the same places on each page load (which could be slightly randomized otherwise).'),
      '#default_value' => $this->options['cache_layout'],
    );
  }

  /**
   * This is essentially only so that it is not called in parent,
   * as it was triggering a PHP notice about undefined index: grouping,
   * which we don't really want to use and handle here.
   */
  function options_validate(&$form, &$form_state) {
    if (!empty($form_state['values']['style_options']['randomize_layout']) && !empty($form_state['values']['style_options']['cache_layout'])) {
      form_set_error('randomize_layout', t('<em>Randomize generated layout</em> and <em>Cache generated layout</em> options are mutually exclusive. Please disable one of them.'));
    }
  }

  /**
   * Allow the style to do stuff before each row is rendered.
   */
  function pre_render($result) {
    parent::pre_render($result);

    // First let's find out which fields are handled by this plugin,
    // and which have "Display all values in the same row" option enabled.
    $imagematrix_fields = array();
    $group_rows_fields = array();
    $empty_imagematrix_result_fields = array();

    foreach ($this->view->field as $field_name => $field) {
      if (!empty($field->options['type']) && $field->options['type'] == 'imagematrix') {

        // There is inconsistency here between field name obtained
        // from $this->view->field array (for example field_image) and name
        // in $this->view->result array (field_field_image). It seems it should
        // be enough to just add "field_" at the beginning, but is there
        // any other way to get result's name to be sure of that?
        $field_name = 'field_' . $field_name;
        $imagematrix_fields[$field_name] = $field->options['settings']['image_link'];

        // If field has "Display all values in the same row" option enabled,
        // it means that multiple images could be added to one result row,
        // which this plugin doesn't like, and will undo it on the next step.
        if ($field->options['group_rows']) {
          $group_rows_fields[$field_name] = $field_name;
        }

        // This could be used during rewriting view results.
        $empty_imagematrix_result_fields[$field_name] = array();

      }
    }

    $fids = array();
    $new_result = array();
    $new_result_count = 0;
    $images_in_results = array();

    // Loop through all view results.
    foreach ($this->view->result as $result_id => $result) {

      // For each result, loop through all Image Matrix fields.
      foreach (array_keys($imagematrix_fields) as $field_name) {

        // If we have even only ONE Image Matrix field with "Display all
        // values in the same row" option enabled, then we still need to
        // process ALL fields and rewrite view results.
        foreach ($this->view->result[$result_id]->{$field_name} as $element_id => $element) {

          // If "Display all values in the same row" in "Multiple field settings"
          // fieldset is enabled for any field using Image Matrix formatter, or
          // there is more than one field in the view configured to use Image
          // Matrix formatter, we need to rewrite view results to output each
          // image in a separate row.
          if (count($group_rows_fields) > 0 || count($imagematrix_fields) > 0) {

            // Option "Reduce duplicates" is enabled and we have already processed this image.
            if ($this->view->style_options['reduce_duplicates'] && in_array($element['raw']['fid'], $images_in_results)) {
              continue;
            }

            // Create new result array based on original result,
            // but without any of Image Matrix fields.
            $new_result[$new_result_count] = array_diff_key((array) $this->view->result[$result_id], $imagematrix_fields);

            // Add empty arrays in place of all Image Matrix fields.
            $new_result[$new_result_count] += $empty_imagematrix_result_fields;

            // Replace one of empty arrays with real field data.
            $new_result[$new_result_count][$field_name][0] = $element;

            // Convert it to object.
            $new_result[$new_result_count] = (object) $new_result[$new_result_count];

            // Add to array used for reducing duplicates.
            $images_in_results[] = $element['raw']['fid'];

            $new_result_count++;
          }
          $fids[] = $element['raw']['fid'];
        }
      }
    }
    // If "Display all values in the same row" is enabled for any field,
    // or there is more than one field in the view configured to use Image
    // Matrix formatter, new view's result array was generated,
    // so let's re-assing it now back to the view.
    if (count($group_rows_fields) > 0 || count($imagematrix_fields) > 0) {
      $this->view->result = $new_result;
    }

    // Settings for MagazineLayout class.
    $mag_options = array(
      'block_width' => $this->view->style_options['block_width'],
      'block_height' => $this->view->style_options['block_height'],
      'block_images_count' => $this->view->style_options['block_images_count'],
      'image_padding' => $this->view->style_options['image_padding'],
      'randomize_layout' => $this->view->style_options['randomize_layout'],
    );

    // Before initializing MagazineLayout class, let's check
    // if we don't already have block definitions stored in cache.
    if (!empty($this->view->style_options['cache_layout'])) {

      $cid_elements = $mag_options + $imagematrix_fields + $fids;

      // Number of $cid_elements could get quite long (longer than 255 chars
      // allowed for cids), so we cannot use it directly, let's then sha1 it.
      $cid = 'imagematrix:' . sha1(implode(':', $cid_elements));

      $cached = cache_get($cid, 'cache_views');
      if (is_object($cached) && isset($cached->data)) {
        $blocks = $cached->data;
      }
    }
    // Block definitions were not found in cache,
    // so we need to regenerate them unfortunately.
    if (empty($this->view->style_options['cache_layout']) || empty($cached->data)) {

      // Allow other modules to change MagazineLayout options.
      $context = array(
        'function' => __FUNCTION__,
        'view' => $this,
      );
      drupal_alter('imagematrix_layout_options', $mag_options, $context);

      // Initialize MagazineLayout class.
      $mag = new MagazineLayout($mag_options);

      // Loop through all results returned by Views.
      foreach ($this->view->result as $result_id => $result) {

        // For each result, loop through all fields using Image Matrix.
        foreach (array_keys($imagematrix_fields) as $field_name) {

          // Check if the formatter involves a link.
          if (!empty($imagematrix_fields[$field_name])) {
            $image_link = $imagematrix_fields[$field_name];
          }

          // For each field, loop through all returned values.
          foreach ($result->{$field_name} as $item_id => $item) {

            // Let's use rendered value provided by formatter -
            // it has more data compared to the original raw value.
            $item = $item['rendered']['#item'];

            // Prepare array of image attributes to be passed
            // to MagazineLayout class, and, well, pass them.
            $image_attributes = array(
              'fid' => $item['fid'],
              'path' => drupal_realpath($item['uri']),
              'width' => $item['width'],
              'height' => $item['height'],
              'title' => $item['title'],
              'alt' => $item['alt'],
              'view_result_id' => $result_id,
              'view_result_item_id' => $item_id,
              'field_name' => $field_name,
              'item_id' => $item_id,
            );
            $mag->addImage($image_attributes);
          }
        }
      }

      // Get processed blocks back from MagazineLayout class.
      $blocks = $mag->getProcessedBlocks();

      // Save blocks to cache.
      if (!empty($this->view->style_options['cache_layout'])) {
        cache_set($cid, $blocks, 'cache_views');
      }

    }

    if (!empty($blocks)) {

      // Merge obtained data with original unprocessed data.
      foreach ($blocks as $block_id => $block) {
        foreach ($block as $image_id => $image) {
          // Merge image data received from MagazineLayout class
          // with original image data from $view->result.
          $image = array_merge(
            $this->view->result[$image['view_result_id']]->{$image['field_name']}[$image['item_id']]['rendered']['#item'],
            $image
          );
          // Add merged image data back to $view->result array.
          $this->view->result[$image['view_result_id']]->{$image['field_name']}[$image['view_result_item_id']]['rendered']['#item'] = $image;
          // And update it in $blocks array as well.
          $blocks[$block_id][$image_id] = $image;
        }
      }

      // MagazineLayout class changes order of images, so we need
      // to re-sort view's results and assign them back to the view object.
      $new_result = array();
      foreach ($blocks as $block_id => $block) {
        foreach ($block as $image_id => $image) {
          $new_result[] = $this->view->result[$image['view_result_id']];
        }
      }
      $this->view->result = $new_result;

      // Save blocks to the view object, we will need it in preprocess function.
      $this->view->imagematrix = array(
        'blocks' => $blocks,
      );
    }
  }

}
