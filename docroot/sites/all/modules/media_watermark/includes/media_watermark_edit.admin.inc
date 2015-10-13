<?php

/**
 * @file
 * Media watermark module integration.
 *
 * Main interface for CRUD actions.
 */

/**
 * Page callback: Displays a table of watermarks.
 *
 * @return array
 *   A render array for a page containing a table of watermarks.
 *
 * @see media_watermark_menu()
 */
function media_watermark_get_sort_table_watermarks() {
  $header = array(
    array(
      'data' => t('Image'),
      'field' => 'mw.fid',
    ),
    array(
      'data' => t('Name'),
      'field' => 'mw.name',
    ),
    array(
      'data' => t('Horizontal Position'),
      'field' => 'mw.hor_position',
    ),
    array(
      'data' => t('Vertical Position'),
      'field' => 'mw.ver_position',
    ),
    array(
      'data' => t('Horizontal Margin'),
      'field' => 'mw.hor_margin',
    ),
    array(
      'data' => t('Horizontal Margin'),
      'field' => 'mw.ver_margin',
    ),
    array(
      'data' => t('Edit/Delete'),
      'field' => 'mw.wid',
    ),
  );

  $query = db_select('media_watermark', 'mw')
    ->fields('mw', array(
      'fid',
      'name',
      'hor_position',
      'ver_position',
      'hor_margin',
      'ver_margin',
      'wid',
    ));

  $query = $query->extend('TableSort')->orderByHeader($header);
  $query = $query->extend('PagerDefault')->limit(10);
  $rows = $query->execute();
  $rows = $rows->fetchAll(PDO::FETCH_ASSOC);

  if (!empty($rows)) {
    foreach ($rows as $key => &$value) {
      // Clean output data.
      $value = array_map('check_plain', $value);

      if (!empty($value['fid'])) {
        $file = file_load($value['fid']);
        $image = theme('image_formatter', array(
          'item' => array(
            'uri' => $file->uri,
            'alt' => t('edit'),
            'title' => t('Edit'),
          ),
          'path' => array(
            'path' => 'admin/config/media/watermark/edit/' . $value['wid'],
            'options' => array('html' => TRUE),
          ),
          'image_style' => 'media_watermark',
        ));
        $rows[$key]['fid'] = $image;
        $rows[$key]['name'] = $value['name'];
        // Create Edit Delete render array.
        $links = array(
          'links' => array(
            '#theme' => 'links',
            '#links' => array(
              array(
                'title' => 'Edit',
                'href' => 'admin/config/media/watermark/edit/' . $value['wid'],
              ),
              array(
                'title' => 'Delete',
                'href' => 'admin/config/media/watermark/delete/' . $value['wid'],
              ),
            ),
            "#attributes" => array('class' => array('edit-delete-links')),
          ),
        );
        // Used render because $links array is not processed into theme table.
        $rows[$key]['wid'] = render($links);
      }
    }
    // Prepare render array.
    $element['pager_table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('Your table is empty'),
    );
    $element['pager_pager'] = array('#theme' => 'pager');

    // Add css to style edit/delete link.
    drupal_add_css(drupal_get_path('module', 'media_watermark') . '/media_watermark.css',
      array('group' => CSS_DEFAULT, 'every_page' => FALSE));

    return $element;
  }
  else {
    return t('Please add at least one watermark.');
  }
}
