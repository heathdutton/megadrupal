<?php

function template_preprocess_biblereadings_daily_email_html(&$vars) {
  
}

function template_preprocess_biblereadings_daily_email_text(&$vars) {
  
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_biblereadings_readings_this_week(&$vars) {
  $readings = $vars['readings'];
  $show_downloads = $vars['show_downloads'];
  $abbreviated = $vars['abbreviated'];

  $path = drupal_get_path('module', 'biblereadings');
  drupal_add_css("$path/css/biblereadings.css");

  $today = strtotime(date('Y-m-d'));

  $output = '';

  $dt_format = 'l';
  $version = 'NKJV';

  $head = array(t('Day'), t('Devotions'), t('Readings'));
  $rows = array();

  foreach ($readings as $i) {
    $class = (date('Y-m-d', $i->date) == date('Y-m-d', $today)) ? 'today' : '';
        
    $regular = biblereadings_format_regular_reading($i, $abbreviated, TRUE, $version);
    
    $psalms = biblereadings_format_psalms($i, $abbreviated, TRUE, $version);
    
    $psalms .= ' / ';
   
    $psalms .= biblereadings_format_proverb($i, $abbreviated, TRUE, $version);

    $rows[] = array(
        'class' => array($class),
        'data' => array(
            date($dt_format, $i->date),
            $psalms,
            $regular,
        ),
    );
  }

  $output .= theme('table', array('header' => $head, 'rows' => $rows, 'attributes' => array('id' => 'biblereadings-this-week')));
  
  $output .= '<h3>' . t('Get Daily Readings in Your Inbox!') . '</h3>';
  $form = drupal_get_form('biblereadings_subscribe_form');
  $output .= drupal_render($form);
  

  if ($show_downloads) {
    $links = array();
    $standard = variable_get('biblereadings_standard_readings_download_path', '');
    $chronological = variable_get('biblereadings_chronological_readings_download_path', '');

    if ($standard) {
      $links[] = l('Standard Bible reading plan', file_create_url($standard));
    }
    if ($chronological) {
      $links[] = l('Chronological Bible reading plan', $chronological, array('attributes' => array('target' => '_blank')));
    }

    $output .= theme('item_list', array('items' => $links, 'title' => 'Download a Printable Reading Plan'));
  }

  return $output;
}
