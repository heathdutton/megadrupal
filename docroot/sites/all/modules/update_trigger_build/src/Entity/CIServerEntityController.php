<?php

/**
 * @file 
 *  CIServerEntityController class.
 */

/**
 * Extend EntityAPIController to define how the entity should be displayed.
 * 
 * @todo remove this and use the default entity controller. We don't actually 
 * create any entities of type CI Server, since the child entity types are used,
 * e.g. JenkinsCIServerType.
 */
class CIServerEntityController extends EntityAPIController {

  /**
   * Override EntityAPIController::buildContent in order to add custom display logic for
   * our custom entity fields.
   *
   * @see EntityAPIController::buildContent()
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);

    // Our additions to the $build render array.
    $build['description'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->description),
      '#prefix' => '<div class="ci-server-description">',
      '#suffix' => '</div>',
    );
    $build['type'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($entity->type),
      '#prefix' => '<p class="ci-server-type">',
      '#suffix' => '</p>',
    );
    return $build;
  }
}
