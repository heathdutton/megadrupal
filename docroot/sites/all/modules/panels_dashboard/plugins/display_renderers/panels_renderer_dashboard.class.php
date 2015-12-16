<?php

class panels_renderer_dashboard extends panels_renderer_standard {
  /**
   * Include all non-standard JS and CSS files.
   */
  function add_meta() {
    parent::add_meta();

    // Include jQuery UI drag and drop functionality
    drupal_add_library('system', 'ui.draggable');
    drupal_add_library('system', 'ui.droppable');
    drupal_add_library('system', 'ui.sortable');

    // Introduce some new CSS and JS files into the mix
    ctools_add_js('panels_dashboard', 'panels_dashboard');
    ctools_add_css('panels_dashboard', 'panels_dashboard');

    // Inject some settings to JS space. These are used for AJAX calls and some other behaviors.
    drupal_add_js(array("panelsDashboard" => array("did" => $this->display->did)), "setting");
  }

  /**
   * Inject some Ids and Classes to panes
   *
   * @return array
   *   An array of rendered panel regions, keyed on the region name.
   */
  function render_panes() {
    ctools_include('content');

    // First, render all the panes into little boxes.
    $this->rendered['panes'] = array();

    foreach ($this->prepared['panes'] as $pid => $pane) {
      // Panes may have no CSS information provided. Define our own.
      // @todo: stub into helper, or see if needed this in depth.
      if (empty($pane->css)) {
        $pane->css['css_id'] = 'pane-' . $pane->pid;
        $pane->css['css_class'] = 'panels-dashboard-pane';
      }
      // There is existing CSS data
      else {
        $pane->css['css_id'] = (isset($pane->css['css_id'])) ? $pane->css['css_id'] : 'pane-' . $pane->pid;
        $pane->css['css_class'] .= ' panels-dashboard-pane';
      }


      $content = $this->render_pane($pane);
      if ($content) {
        $this->rendered['panes'][$pid] = $content;
      }
    }
  }

  /**
   * Wrap regions inside dashboard divs.
   *
   * @return array
   *   An array of rendered panel regions, keyed on the region name.
   */
  function render_regions() {
    parent::render_regions();

    foreach($this->rendered['regions'] as $region_id => $region) {
      $this->rendered['regions'][$region_id] = '<div id="panels-dashboard-region-'.$region_id.'" class="panels-dashboard-region">'.$region.'</div>';
    }

    return $this->rendered['regions'];
  }
}
