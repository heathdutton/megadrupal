<?php
/**
 * @file
 * Implement the renderer plugin itself.
 */

/**
 * Renderer class for Selective In-Place Editor (IPE) behavior.
 */
class PanelsLockRegionSelectiveIpe extends panels_renderer_ipe {

  /**
   * Override region render to show lock buttons.
   */
  public function render_region($region_id, $panes) {
    // If user can administer locks.
    if (user_access('administer panels region lock')) {
      // Generate this region's 'empty' placeholder pane from the IPE plugin.
      $empty_ph = theme(
        'panels_ipe_placeholder_pane',
        array(
          'region_id' => $region_id,
          'region_title' => $this->plugins['layout']['regions'][$region_id],
        )
      );

      // List of classes for the region wrapper.
      $attributes = array(
        'class' =>  array(
          'panels-ipe-placeholder',
          'panels-ipe-on',
          'panels-ipe-portlet-marker',
          'panels-ipe-portlet-static',
          'lock-ipe-' . $this->display->panel_settings['region_lock'][$region_id],
        ),
      );

      // Render buttons of the region.
      $content = $empty_ph . theme('panels_lock_region_region_buttons',
        array(
          'region_id' => $region_id,
          'display' => $this->display,
          'renderer' => $this,
        )
      );

      // Wrap placeholder and buttons in some markup.
      $control = '<div ' . drupal_attributes($attributes) . '>' . $content . '</div>';

      // Generate output.
      $output = theme(
        'panels_ipe_region_wrapper',
        array(
          'output' => panels_renderer_editor::render_region($region_id, $panes),
          'region_id' => $region_id,
          'display' => $this->display,
          'controls' => $control,
          'renderer' => $this,
        )
      );

      // Add required CSS styles.
      $module_path = drupal_get_path('module', 'panels_lock_region');
      drupal_add_css($module_path . '/css/panels_lock_region.css');

      // Prepare attributes for container.
      $attributes = array(
        'id' => 'panels-ipe-regionid-' . $region_id,
        'class' => 'panels-ipe-region',
      );

      return '<div ' . drupal_attributes($attributes) . '>' . $output . '</div>';
    }

    // User can not administer locks, so render region as usual.
    if (!_panels_lock_region_region_is_locked($this->display, $region_id)) {
      return parent::render_region($region_id, $panes);
    }

    return panels_renderer_standard::render_region($region_id, $panes);
  }

  /**
   * Override pane render to use panels ipe or panels standard renderer.
   */
  public function render_pane(&$pane) {
    if (user_access('administer panels region lock') ||
        !_panels_lock_region_region_is_locked($this->display, $pane->panel)) {
      return parent::render_pane($pane);
    }
    return panels_renderer_standard::render_pane($pane);
  }

  /**
   * AJAX command to lock a region on current display.
   */
  public function ajax_change_lock($region = NULL, $action = NULL) {
    if (user_access('administer panels region lock')) {
      // Validate region exists on current layout.
      if (!array_key_exists($region, $this->plugins['layout']['regions'])) {
        ctools_ajax_render_error(t('Invalid region'));
      }

      // Actually lock region.
      $this->display->panel_settings['region_lock'][$region] = $action . 'ed';
      $this->cache->display->panel_settings['region_lock'][$region] = $action . 'ed';
      panels_edit_cache_set($this->cache);

      // Render components again.
      $this->prepare();
      $this->render_panes();
      $this->render_regions();

      // Replace HTML content using AJAX.
      $this->commands[] = ajax_command_replace("#panels-ipe-regionid-$region", $this->rendered['regions'][$region]);
    }
  }

}
