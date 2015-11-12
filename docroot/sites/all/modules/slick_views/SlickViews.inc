<?php
/**
 * @file
 * Slick style plugin for the Views module.
 */

/**
 * Implements a style type plugin for the Views module.
 */
class SlickViews extends views_plugin_style {

  /**
   * Provides default options.
   */
  public function option_definition() {
    module_load_include('inc', 'slick_views', 'includes/admin');
    module_load_include('inc', 'slick', 'includes/slick.global');

    $options = _slick_views_option_definition() + parent::option_definition();
    return $options;
  }

  /**
   * Shows a form to edit the style options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    ctools_form_include($form_state, 'slick.admin', 'slick');
    ctools_form_include($form_state, 'admin', 'slick_views');

    $view = $this->view;
    $settings = $this->options;
    _slick_views_options_form($form, $form_state, $view, $settings);
  }

  /**
   * Performs some cleanup tasks on the options array before saving it.
   */
  public function options_submit(&$form, &$form_state) {
    $options = &$form_state['values']['style_options'];

    // The form is a #tree, but the expected output is a flattened array.
    if (!empty($options['slick'])) {
      // Pull the fieldset values one level up.
      $options += $options['slick'];
      unset($options['slick']);
    }
  }

  /**
   * Gets renderable array of field containing rendered and raw data.
   */
  public function getFieldData($row, $field_name) {
    $field = $this->view->field[$field_name]->handler_type . '_' . $field_name;
    return isset($row->{$field}[0]) ? $row->{$field}[0] : '';
  }

  /**
   * Renders the slick instances.
   */
  public function renderSlick($slick) {
    return array(
      '#theme'   => $this->theme_functions(),
      '#view'    => $this->view,
      '#options' => $this->options,
      '#rows'    => $slick,
    );
  }

  /**
   * Renders the display in this style.
   */
  public function render() {
    $view      = $this->view;
    $settings  = $this->options;
    $view_name = $view->name;
    $current   = $view->current_display;
    $slick_id  = &drupal_static('slick_id', 0);
    $id        = $settings['id'] ? $settings['id'] : drupal_clean_css_identifier("slick-views-{$view_name}-" . ++$slick_id);
    $thumb_id  = $id . '-thumbnail';
    $cid       = $settings['cid'] = $settings['optionset'] . $settings['skin'] . $view_name . $current;
    $asnavfor  = $settings['optionset_thumbnail'];

    // Renders slicks quickly from cache if any, as render cache is just cache.
    $render_cache = FALSE;
    if (!empty($settings['cache']) && $cache = cache_get($id . ':' . $cid, 'cache')) {
      $render_cache = $settings['cache'] == 'persistent' ? TRUE : REQUEST_TIME < $cache->expire;
    }

    if ($render_cache) {
      $slick[0] = $cache->data;
      if ($asnavfor && $thumb_cache = cache_get($thumb_id . ':' . $cid, 'cache')) {
        $slick[1] = $thumb_cache->data;
      }

      // Passes slicks to theme_slick_views() to render the cache.
      return $this->renderSlick($slick);
    }

    // Otherwise do the routines before a cache stored, or when disabled.
    module_load_include('inc', 'slick', 'includes/slick.global');

    // Group the rows according to the grouping field, if specified.
    $sets                  = $this->render_grouping($view->result, $settings['grouping']);
    $keys                  = array_keys($view->field);
    $slick_markups         = $settings['slide_field_wrapper'];
    $optionset             = slick_optionset_load($settings['optionset']);
    $optionset_thumbnail   = $asnavfor ? slick_optionset_load($asnavfor) : NULL;
    $settings['view_name'] = $view_name;

    $attach = array(
      'attach_skin'        => $settings['skin'],
      'attach_skin_arrows' => $settings['skin_arrows'],
      'attach_skin_dots'   => $settings['skin_dots'],
    );

    $build = array();
    foreach ($sets as $rows) {
      $items = $thumbs = $js = array();
      foreach ($rows as $index => $row) {
        $view->row_index = $index;
        $thumb = array();
        $item = array(
          'caption'  => array(),
          'slide'  => array(),
          'settings' => array(
            'layout' => $settings['slide_layout'],
          ),
        );

        if ($slick_markups) {
          // Add main image field if so configured.
          if ($slide_image = $settings['slide_image']) {
            $image = $this->getFieldData($row, $slide_image);

            // Not-empty behavior and filter fail, so add own check here.
            if (isset($image['raw']) && $image_rendered = $this->get_field($index, $slide_image)) {
              $bg_type = isset($image['raw']['type']) ? $image['raw']['type'] : 'image';
              $item['settings']['type'] = $bg_type;
              $item['slide']['#markup'] = $image_rendered;
            }

            // Add thumbnail navigation if so configured.
            if ($slide_thumbnail = $settings['slide_thumbnail']) {
              $thumbnail = $this->getFieldData($row, $slide_thumbnail);
              if (isset($thumbnail['raw']) && $thumbnail_rendered = $this->get_field($index, $slide_thumbnail)) {
                $thumb['slide']['#markup'] = $thumbnail_rendered;
              }
            }

            // Allows text-only thumbnail navigation, like regular tabs.
            // Use Views UI "Rewrite results" to sanitize the caption.
            if ($thumbnail_caption = $settings['thumbnail_caption']) {
              $thumb['caption']['data']['#markup'] = $this->get_field($index, $thumbnail_caption);
              if (!isset($thumb['slide'])) {
                $thumb['slide'] = array();
                $item['settings']['type'] = 'text';
              }
            }
          }

          // Add all caption fields if so configured.
          if ($slide_captions = $settings['slide_caption']) {
            $slide_captions = is_array($slide_captions) ? array_filter($slide_captions) : (array) $slide_captions;

            $caption_items = array();
            foreach ($slide_captions as $key => $slide_caption) {
              $slide_caption_rendered = $this->get_field($index, $slide_caption);
              if (empty($slide_caption_rendered)) {
                continue;
              }

              if (in_array($slide_caption, array_values($keys))) {
                $caption_items[$key]['#markup'] = $slide_caption_rendered;
              }
            }
            if ($caption_items) {
              $item['caption']['data'] = $caption_items;
            }
          }

          // Add overlay, this allows overlay only without image background.
          if ($slide_overlay = $settings['slide_overlay']) {
            $item['caption']['overlay']['#markup'] = $this->get_field($index, $slide_overlay);
          }

          // Add title field if so configured.
          if ($slide_title = $settings['slide_title']) {
            $item['caption']['title']['#markup'] = filter_xss_admin($this->get_field($index, $slide_title));
          }

          // Add link field if so configured.
          if ($slide_link = $settings['slide_link']) {
            $item['caption']['link']['#markup'] = $this->get_field($index, $slide_link);
          }

          // Add editor link field, Views takes care of the access control.
          if ($editor_link = $settings['editor_link']) {
            $item['caption']['editor']['#markup'] = $this->get_field($index, $editor_link);
          }

          // Add layout field, may be a list field, or builtin layout options.
          if ($slide_layout = $settings['slide_layout']) {
            if (strpos($slide_layout, 'field_') !== FALSE) {
              $item['settings']['layout'] = check_plain($this->get_field($index, $slide_layout));
            }
            else {
              $item['settings']['layout'] = $slide_layout;
            }
          }
        }
        else {
          $item['slide'] = $view->style_plugin->row_plugin->render($row);
        }

        // Main slide items.
        $items[] = $item;

        // Thumbnail slide items.
        if ($thumb) {
          $thumbs[] = $thumb;
        }
      }

      unset($view->row_index);

      // Build the Slick attributes.
      $is_asnavfor   = $asnavfor && $thumbs;
      $asnavfor_auto = $settings['asnavfor_auto'];

      $settings['count']                 = count($items);
      $settings['current_display']       = 'main';
      $settings['attributes']['id']      = $id;
      $settings['attributes']['class'][] = drupal_clean_css_identifier('slick--view--' . $view_name);
      $settings['attributes']['class'][] = drupal_clean_css_identifier('slick--view--' . $view_name . '--' . $current);

      if ($is_asnavfor) {
        $attach['attach_skin_thumbnail'] = $settings['skin_thumbnail'];
        $settings['asnavfor_target']     = $asnavfor_auto ? "#{$thumb_id}-slider" : $settings['asnavfor_main'];
      }

      // Build the Slick grid only if having a reasonable amount of grid items.
      if ($settings['grid']) {
        if (count($view->result) > $settings['visible_slides']) {
          $items = slick_build_grids($items, $settings);
        }
      }

      // Overrides common options to re-use a single optionset.
      if ($settings['override']) {
        foreach ($settings['overridables'] as $key => $override) {
          $js[$key] = empty($override) ? FALSE : TRUE;
        }
        unset($settings['overridables']);
      }

      // Build the single/main display Slick.
      $attachments = slick_attach($attach, $settings);
      $slick[0] = slick_build($items, $js, $settings, $attachments, $id, $optionset);

      if ($is_asnavfor) {
        $settings['attributes']['id'] = $thumb_id;
        $settings['asnavfor_target']  = $asnavfor_auto ? "#{$id}-slider" : $settings['asnavfor_thumbnail'];
        $settings['optionset']        = $settings['optionset_thumbnail'];
        $settings['current_display']  = 'thumbnail';

        // Build the thumbnail+/text navigation Slick.
        $slick[1] = slick_build($thumbs, array(), $settings, array(), $thumb_id, $optionset_thumbnail);
      }

      // Build the Slick.
      $build = $this->renderSlick($slick);
    }

    return $build;
  }

}
