<?php

/**
 * @file
 * Provides an entity class for fragments.
 */

/**
 * A class for fragment entities.
 */
class Fragment extends Entity {

  /**
   * Builds a structured array representing the entity's content.
   *
   * @see entity_build_content()
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    // Break the link apart into options that url() can use, and store them
    // for our helpers.
    $this->link_options = drupal_parse_url($this->link);

    // This is just a redirect into the controller by default, but we can
    // add to what is returned.
    $build = entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode);

    // Set our own theme.
    $build['#theme'] = 'fragment';

    foreach (element_children($build) as $key) {
      foreach (element_children($build[$key]) as $index) {
        // Turn each field into a link. We can't affect #markup directly here
        // because -- WTF! -- not every field type is fully rendered by this
        // point, eg image fields still have theme_image_formatter() to run
        // through, and adapting for every possible renderable structure is
        // insane.
        $build[$key][$index]['#post_render'] = array('fragment_entity_link');
        // Have to stash our path in at every step. Not terribly elegant.
        $build[$key][$index]['#fragment_link_options'] = $this->link_options;
      }
    }

    // Add in contextual links.
    // This (and other things too) can be removed when patch to Entity API at
    // http://drupal.org/node/1598652 lands.
    $build['#contextual_links'] = array(
      'fragment' => array(
        'admin/content/fragments/manage',
        array($this->identifier()),
      )
    );

    return $build;
  }

}
