<?php

class context_panels_layouts_reaction_block extends context_layouts_reaction_block {
  /**
   * Override of is_enabled_region().
   * Check that there is an active layout and it supports the given region.
   */
  protected function is_enabled_region($region) {
    $layout = $this->get_active_layout();
    if ($layout && isset($layout['regions']) && is_array($layout['regions'])) {
      if (isset($layout['panels'])) {
        return in_array($region, $layout['regions'], TRUE);
      }
      else {
        return in_array($region, $layout['regions'], TRUE) && parent::is_enabled_region($region);
      }
    }
    return parent::is_enabled_region($region);
  }


  /**
   * Override of is_editable_region().
   *
   * Determine whether inline editing requirements are met and that the current
   * user may edit.
   */
  protected function is_editable_region($region, $reset = FALSE) {
    // Check requirements.
    // Generally speaking, it does not make sense to allow anonymous users to
    // edit a context inline. Though it may be possible to save (and restore)
    // an edited context to an anonymous user's cookie or session, it's an
    // edge case and probably not something we want to encourage anyway.
    static $requirements;
    if (!isset($requirements) || $reset) {
      global $user;
      if ($user->uid) {
        $requirements = TRUE;
        drupal_add_library('system', 'ui.droppable');
        drupal_add_library('system', 'ui.sortable');
        drupal_add_js(drupal_get_path('module', 'context') . '/plugins/context_reaction_block.js');
        drupal_add_css(drupal_get_path('module', 'context') . '/plugins/context_reaction_block.css');
      }
      else {
        $requirements = FALSE;
      }
    }
    // Check that this region is not locked by the theme.
    global $theme;
    $info = system_get_info('theme', $theme);
    if ($info && isset($info['regions_locked']) && in_array($region, $info['regions_locked'])) {
      return FALSE;
    }
    // Check that this region is not hidden
    $visible = system_region_list($theme, REGIONS_VISIBLE);
    // Panels-stype regions should always be visible.
    if (strpos($region, 'context_panels_layouts') === 0) {
      $visible[$region] = TRUE;
    }
    return $requirements && $this->is_enabled_region($region) && isset($visible[$region]);
  }

  /**
   * Override of editable_region().
   *
   * Add markup for making a region editable.
   */
  protected function editable_region($region, $build) {
    if ($this->is_editable_region($region) &&
        (!empty($build) ||
         variable_get('context_reaction_block_all_regions', FALSE) ||
         context_isset('context_ui', 'context_ui_editor_present'))
    ) {
      global $theme;
      $regions = system_region_list($theme);
      if ($layout = $this->get_active_layout()) {
        if (isset($layout['panels'])) {
          foreach ($layout['panels'] as $key => $label) {
            $regions['context_panels_layouts_' . $key] = $label;
          }
        }
      }
      $name = isset($regions[$region]) ? $regions[$region] : $region;
      $build['context']['#markup'] = "<a class='context-block-region' id='context-block-region-{$region}'>{$name}</a>";
    }
    return $build;
  }

  /**
   * Retrieve the first layout specified found by any active contexts.
   */
  function get_active_layout($info = TRUE) {
    $contexts = $this->get_contexts();
    $layouts = context_panels_layouts_get_layouts();
    if (!empty($contexts) && !empty($layouts)) {
      foreach ($contexts as $context) {
        $values = $this->fetch_from_context($context);
        if (isset($values['layout']) && isset($layouts[$values['layout']])) {
          return $info ? $layouts[$values['layout']] : $values['layout'];
        }
      }
    }
    // Fallback to default layout if provided.
    if (isset($layouts['default'])) {
      return $info ? $layouts['default'] : 'default';
    }
    return FALSE;
  }

  /**
   * Add the layout stylesheet to the CSS.
   */
  function add_layout_stylesheet() {
    if ($layout = $this->get_active_layout()) {
      if (!empty($layout['stylesheet'])) {
        $path = isset($layout['path']) ? $layout['path'] : drupal_get_path('theme', $layout['theme']);
        drupal_add_css($path . '/' . $layout['stylesheet']);
      }
    }
  }

  /**
   * Override of options form.
   */
  function options_form($context) {
    $form = parent::options_form($context);
    $options = $this->fetch_from_context($context);
    // Only alter the options form if the theme provides layouts.
    $theme_key = variable_get('theme_default', 'bartik');
    $weight_delta = $this->max_block_weight();
    $layouts = $this->get_layout_options();
    if (!empty($layouts)) {

      // Add regions provided by panels-style layouts.
      foreach (_context_panels_layouts_get_panels_layout_data() as $name => $layout) {
        foreach ($layout['panels'] as $region => $label) {
          $region = 'context_panels_layouts_' . $region;
          $form['blocks'][$region] = array(
            '#type' => 'item',
            '#title' => $label,
            '#tree' => TRUE,
          );
          foreach ($this->get_blocks($region, $context) as $block) {
            if (!empty($block->context)) {
              $form['blocks'][$region][$block->bid] = array(
                '#value' => check_plain($block->info),
                '#weight' => $block->weight,
                '#type' => 'markup',
                '#tree' => TRUE,
                'weight' => array('#type' => 'weight', '#delta' => $weight_delta, '#default_value' => 0),
              );
            }
          }
        }
      }
      // Add js.
      // @TODO: Move this to a theme function or somewhere that will get called even
      // if the form is using a cached version of itself (e.g. when validate fails).
      drupal_add_js(drupal_get_path('module', 'context_panels_layouts') . '/plugins/context_panels_layouts_reaction_block.js');
    }
    return $form;
  }

  /**
   * Override of submit handler.
   */
  function options_form_submit($values) {
    $options = parent::options_form_submit($values);

    // Only alter the options form if the theme provides layouts.
    $theme_key = variable_get('theme_default', 'bartik');
    $layouts = context_panels_layouts_get_layouts($theme_key);

    // Check that this is a valid layout.
    if (isset($values['layout']) && !empty($values['layout']) && isset($layouts[$values['layout']])) {
      $layout = $values['layout'];
      $options['layout'] = $layout;

      // Remove blocks that don't belong to regions in this layout.
      if (isset($layouts[$layout]['regions'])) {
        foreach ($options['blocks'] as $bid => $block) {
          if (!in_array($block['region'], $layouts[$layout]['regions'])) {
            unset($options['blocks'][$bid]);
          }
        }
      }
    }
    return $options;
  }

  /**
   * Override of get layout options for the given theme.
   */
  protected function get_layout_options($theme_key = NULL) {
    $theme_key = !isset($theme_key) ? variable_get('theme_default', 'bartik') : $theme_key;
    $layouts = context_panels_layouts_get_layouts($theme_key);
    $layout_options = array();
    if (!empty($layouts)) {
      foreach ($layouts as $layout => $info) {
        $layout_options[$layout] = isset($info['name']) ? $info['name'] : $layout_options;
      }
    }
    return $layout_options;
  }

  /**
   * Get a layout to region map for the given theme.
   */
  protected function get_layout_regions($theme_key = NULL) {
    $theme_key = !isset($theme_key) ? variable_get('theme_default', 'bartik') : $theme_key;
    $layouts = context_panels_layouts_get_layouts($theme_key);
    if (!empty($layouts)) {
      $layout_regions = array();
      foreach ($layouts as $layout => $info) {
        $layout_regions[$layout] = is_array($info['regions']) ? $info['regions'] : array();
      }
    }
    return $layout_regions;
  }

  /**
   * Override of execute.
   */
  function execute(&$page) {
    parent::execute($page);

    $layout = $this->get_active_layout();
    if (isset($layout['regions']) && isset($layout['theme_hook'])) {
      $content = array();
      foreach ($layout['regions'] as $region) {
        $blocks = $this->block_get_blocks_by_region($region);
        // Strip the 'context_panels_layouts_' prefix.
        $region = substr($region, 23);
        // TODO: output this as a renderable array instead of rendering.
        $content[$region] = $blocks ? drupal_render($blocks) : '';
      }
      if (!empty($content)) {
        $page['content']['system_main']['context_panels_layouts'] = array(
          '#content' => $content,
          '#css_id' => 'context-panels-layouts-' . $layout['key'],
          '#theme' => $layout['theme_hook'],
        );
      }
    }
  }
}

