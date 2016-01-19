<?php

/**
 * @file
 * Contains class for build "Dynamic pane" content type plugin as IPE.
 */

namespace Drupal\dynamic_panes_ipe;

use Drupal\dynamic_panes\Pane;

/**
 * Class for build "Dynamic pane" content type plugin as IPE.
 */
class PaneIPE extends Pane {

  /**
   * Render pane.
   *
   * @return array
   *   And array contains rendered content.
   */
  public function render() {
    $this->preRender();

    $content = array();

    $form = &drupal_static('dynamic_panes_ipe_toolbar_form', array());

    $levels = $this->getLevels();

    foreach ($this->layoutProvider->getLayouts() as $layout) {
      if ($region = $layout->getRegion()) {
        $region->sortForm($form);

        $region_id = $region->getRegionID();
        $content[$region_id] = array();

        foreach ($levels as $level) {
          $content[$region_id][$level] = array(
            '#theme' => array('dynamic_panes_ipe_region'),
            '#pane' => $this,
            '#region' => $region,
            '#level' => $level,
          );
        }
      }
    }

    return $content;
  }
}
