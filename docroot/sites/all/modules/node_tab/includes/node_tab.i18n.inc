<?php

/**
 * The i18n controller for node_tab entities.
 */
class NodeTabI18nStringController extends EntityDefaultI18nStringController {

  /**
   * Overrides EntityDefaultI18nStringController::hook_object_info().
   */
  public function hook_object_info() {
    $info = parent::hook_object_info();
    $wildcard = $this->menuWildcard();
    $path = $this->menuBasePath() . '/' . $wildcard;

    $info['node_tab']['title'] = 'Tab';
    $info['node_tab']['placeholders']['%node_type'] = 'node_type';
    $info['node_tab']['edit path'] = $path;
    $info['node_tab']['translate tab'] = $path . '/translate';
    $info['node_tab']['string translation']['translate path'] = $path . '/translate/%i18n_language';

    return $info;
  }
}
