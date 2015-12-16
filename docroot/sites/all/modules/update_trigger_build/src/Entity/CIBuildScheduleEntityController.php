<?php

/**
 * @file 
 *  CIBuildScheduleEntityController class.
 */

/**
 * Extend EntityAPIController to define how the entity should be displayed.
 */
class CIBuildScheduleEntityController extends EntityAPIControllerExportable {

  /**
   * Override EntityAPIController::buildContent in order to add custom display logic for
   * our custom entity fields.
   *
   * @todo needs a bit of work to display all fields.  We're not actively displaying 
   * the full entity anywhere, so this is low priority.
   *
   * @see EntityAPIController::buildContent()
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);

    // Our additions to the $build render array.
    $build['description'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($entity->description),
        '#prefix' => '<div class="ci-build-description">',
        '#suffix' => '</div>',
    );
    $build['ci_server'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($entity->ci_server),
        '#prefix' => '<p class="ci-build-server">',
        '#suffix' => '</p>',
    );
    return $build;
  }
}
