<?php

/**
 * @file
 * provides page callbacks for view booklet page.
 */

/**
 * Callback function for view booklet page.
 */
function jquery_booklet_view_page($bid) {

  $path = libraries_get_path('booklet');
  drupal_add_library('system', 'ui.draggable');
  drupal_add_js($path . '/jquery.easing.js', array(
    'type' => 'file',
      )
  );
  drupal_add_js($path . '/jquery.booklet.min.js', array(
    'type' => 'file',
      )
  );
  drupal_add_css($path . '/jquery.booklet.css', array(
    'media' => 'screen, projection, tv',
      )
  );
  drupal_add_css(drupal_get_path('module', 'jquery_booklet') . '/css/jquery_booklet.css');

  $booklet_data = db_select('booklets', 'bt')
      ->fields('bt')
      ->condition('bt.bid', $bid)
      ->execute()
      ->fetchObject();

  $query = db_select('booklet_pages', 'bp');
  $query->fields('bp');
  $query->condition('bp.bid', $bid);
  $query->orderBy('bp.weight', 'ASC');
  $booklets_pages = $query->execute()->fetchAll();

  $settings = unserialize($booklet_data->settings);
  $width = ($booklet_data->width) ? $booklet_data->width : 600;
  $height = ($booklet_data->height) ? $booklet_data->height : 400;
  $js = "jQuery(function() {
	    //single book
	    jQuery('#jquery-booklet').booklet({
          closed: true,
	        covers: true,
	        menu: '#custom-menu',
	        pageSelector: true,
	        chapterSelector: true,
          keyboard: true,
          width:  " . $width . ",
	        height: " . $height . ",
          speed: " . $settings['speed'] . ",
          direction: '" . $settings['direction'] . "',
          pageNumbers: " . $settings['page_numbers'] . ",
          auto: " . $settings['autoplay'] . ",
          arrows: " . $settings['arrows'] . ",  

});     
	    
	});";

  drupal_add_js($js, array(
    'type' => 'inline', 'weight' => 5,
      )
  );

  $pages = '';
  $booklet_title = '';
  foreach ($booklets_pages as $row) {
    if ($booklet_title == '') {
      $booklet_title = $booklet_data->booklet_title;
    }

    $node = node_load($row->nid);
    $node_view = node_view($node, variable_get('jquery_booklet_view_mode', 'full'));
    $pages .= '<div>
                ' . render($node_view) . '
              </div>';
  }

  if ($pages != '') {
    $front_cover = '';
    $back_cover = '';
    if ($booklet_data->front_cover_fid) {
      $front_cover_file = file_load($booklet_data->front_cover_fid);
      $front_cover_url = file_create_url($front_cover_file->uri);
      $front_cover = theme('image', array('path' => $front_cover_url));
    }
    if ($booklet_data->back_cover_fid) {
      $back_cover_file = file_load($booklet_data->back_cover_fid);
      $back_cover_url = file_create_url($back_cover_file->uri);
      $back_cover = theme('image', array('path' => $back_cover_url));
    }
    if (empty($front_cover)) {
      $front_cover = '<div><h3>' . check_plain($booklet_title) . '</h3></div>';
    }

    if (empty($back_cover)) {
      $back_cover = '<div><h3>' . t('End Of Book') . '</h3></div>';
    }

    $pages = $front_cover . $pages . $back_cover;
  }
  $booklet = '<div id="jquery-booklet">';
  $booklet .= $pages;
  $booklet .= '</div>';

  return $booklet;
}
