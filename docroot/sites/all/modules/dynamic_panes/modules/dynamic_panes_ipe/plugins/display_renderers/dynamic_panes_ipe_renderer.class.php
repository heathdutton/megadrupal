<?php

/**
 * @file
 * Contains class for renderer panels as IPE.
 */

/**
 * Class dynamic_panes_ipe_renderer.
 */
class dynamic_panes_ipe_renderer extends panels_renderer_standard {

  /**
   * Attach out-of-band page metadata (e.g., CSS and JS).
   *
   * This must be done before render, because panels-within-panels must have
   * their CSS added in the right order: inner content before outer content.
   */
  public function add_meta() {
    parent::add_meta();

    ctools_include('cleanstring');
    $this->clean_key = ctools_cleanstring($this->display->cache_key);

    $button = array(
      '#type' => 'link',
      '#title' => t('Show layout edit controls'),
      '#href' => '',
      '#id' => 'dynamic-panes-ipe-customize-page',
      '#attributes' => array(
        'class' => array('dynamic-panes-ipe-startedit', 'dynamic-panes-ipe-pseudobutton'),
      ),
      '#prefix' => '<div class="dynamic-panes-ipe-pseudobutton-container">',
      '#suffix' => '</div>',
    );

    dynamic_panes_ipe_toolbar_add_button($this->clean_key, 'dynamic-panes-ipe-startedit', $button);

    drupal_add_library('system', 'jquery.form');
    drupal_add_library('system', 'drupal.progress');
    drupal_add_library('system', 'drupal.ajax');
    drupal_add_library('system', 'ui.sortable');

    ctools_add_css('dynamic_panes_ipe', 'dynamic_panes_ipe');
    ctools_add_js('dynamic_panes_ipe', 'dynamic_panes_ipe');
  }

  /**
   * Render a single panel region.
   *
   * Primarily just a passthrough to the panel region rendering callback
   * specified by the style plugin that is attached to the current panel region.
   *
   * @param int $region_id
   *   The ID of the panel region being rendered
   * @param array $panes
   *   An array of panes that are assigned to the panel that's being rendered.
   *
   * @return string
   *   The rendered, HTML string output of the passed-in panel region.
   */
  public function render_region($region_id, $panes) {
    $output = parent::render_region($region_id, $panes);

    $attributes = array('class' => array('dynamic-panes-ipe-pane'));

    return '<div' . drupal_attributes($attributes) . '>' . $output . '</div>';
  }

  /**
   * Prepare the attached display for rendering.
   */
  public function prepare($external_settings = NULL) {
    parent::prepare($external_settings);

    $this->display->dynamicPanesPaneClass = '\Drupal\dynamic_panes_ipe\PaneIPE';
    foreach ($this->prepared['panes'] as $pane) {
      if ($pane->type == 'dynamic_panes' && $pane->subtype == 'dynamic_panes') {
        $pane->configuration['pane'] = $pane;
        $pane->configuration['dynamic_panes_ipe'] = TRUE;
      }
    }
  }
}
