<?php

/**
 * @file
 * Definition of Scribble entity class.
 */

/*
 * Entity class for the Scribble entity.
 */
class Scribble extends Entity {

  /**
   * Overrides Entity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content = array();
    if ($view_mode == 'full') {
      $content['blackboard'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('scribble-blackboard-wrapper'),
        ),
        'blackboard' => drupal_get_form('scribble_blackboard_form', $this),
      );
    }
    elseif ($view_mode == 'snapshot_administration') {
      $image_items = field_get_items('scribble', $this, 'scribble_image');
      $snapshot_items = field_get_items('scribble', $this, 'scribble_image_snapshots');

      $headers = array(
        t('Image'),
        t('Snapshot'),
        t('Image link'),
        t('Snapshot link'),
        t('Delete'),
      );

      $rows = array();
      if (!empty($image_items)) {
        // Collect row data of images, snapshots, filenames and delete links.
        foreach ($image_items as $delta => $item) {
          // Used image style is configurable.
          // @todo add selectbox with imagestyles for this to the settings form.
          $image_style = variable_get('scribble_image_list_image_style', 'medium');
          $rows[] = array(
            theme('image', array('path' => image_style_url($image_style, $item['uri']))),
            theme('image', array('path' => image_style_url($image_style, $snapshot_items[$delta]['uri']))),
            l($item['filename'], file_create_url($item['uri'])),
            l($snapshot_items[$delta]['filename'], file_create_url($snapshot_items[$delta]['uri'])),
            l(t('Delete'), 'admin/config/media/scribble/' . $this->sid . '/remove-image/' . $item['fid']),
          );
        }
      }

      // @todo add pager.
      $content = array(
        '#theme' => 'table',
        '#header' => $headers,
        '#rows' => $rows,
        '#empty' => t('No images were drawn on this scribble yet.'),
      );
    }

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

}
