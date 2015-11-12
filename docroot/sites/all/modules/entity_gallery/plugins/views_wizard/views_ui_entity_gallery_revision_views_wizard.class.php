<?php

/**
 * @file
 * Definition of ViewsUiEntityGalleryRevisionViewsWizard.
 */

/**
 * Tests creating entity gallery revision views with the wizard.
 */
class ViewsUiEntityGalleryRevisionViewsWizard extends ViewsUiEntityGalleryViewsWizard {

  /**
   * Entity gallery revisions do not support full posts or teasers, so remove them.
   */
  protected function row_style_options($type) {
    $options = parent::row_style_options($type);
    unset($options['teasers']);
    unset($options['full_posts']);
    return $options;
  }

  protected function default_display_options($form, $form_state) {
    $display_options = parent::default_display_options($form, $form_state);

    // Add permission-based access control.
    $display_options['access']['type'] = 'perm';
    $display_options['access']['perm'] = 'view entity gallery revisions';

    // Remove the default fields, since we are customizing them here.
    unset($display_options['fields']);

    /* Field: Entity gallery revision: Created date */
    $display_options['fields']['timestamp']['id'] = 'timestamp';
    $display_options['fields']['timestamp']['table'] = 'entity_gallery_revision';
    $display_options['fields']['timestamp']['field'] = 'timestamp';
    $display_options['fields']['timestamp']['alter']['alter_text'] = 0;
    $display_options['fields']['timestamp']['alter']['make_link'] = 0;
    $display_options['fields']['timestamp']['alter']['absolute'] = 0;
    $display_options['fields']['timestamp']['alter']['trim'] = 0;
    $display_options['fields']['timestamp']['alter']['word_boundary'] = 0;
    $display_options['fields']['timestamp']['alter']['ellipsis'] = 0;
    $display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
    $display_options['fields']['timestamp']['alter']['html'] = 0;
    $display_options['fields']['timestamp']['hide_empty'] = 0;
    $display_options['fields']['timestamp']['empty_zero'] = 0;

    /* Field: Entity gallery revision: Title */
    $display_options['fields']['title']['id'] = 'title';
    $display_options['fields']['title']['table'] = 'entity_gallery_revision';
    $display_options['fields']['title']['field'] = 'title';
    $display_options['fields']['title']['label'] = '';
    $display_options['fields']['title']['alter']['alter_text'] = 0;
    $display_options['fields']['title']['alter']['make_link'] = 0;
    $display_options['fields']['title']['alter']['absolute'] = 0;
    $display_options['fields']['title']['alter']['trim'] = 0;
    $display_options['fields']['title']['alter']['word_boundary'] = 0;
    $display_options['fields']['title']['alter']['ellipsis'] = 0;
    $display_options['fields']['title']['alter']['strip_tags'] = 0;
    $display_options['fields']['title']['alter']['html'] = 0;
    $display_options['fields']['title']['hide_empty'] = 0;
    $display_options['fields']['title']['empty_zero'] = 0;
    $display_options['fields']['title']['link_to_entity_gallery'] = 0;
    $display_options['fields']['title']['link_to_entity_gallery_revision'] = 1;

    return $display_options;
  }
}
