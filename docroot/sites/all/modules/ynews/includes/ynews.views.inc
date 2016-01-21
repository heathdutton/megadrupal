<?php
/**
 * @file
 * Views integration for Yandex news module.
 */

/**
 * Implements hook_views_plugin().
 */
function ynews_views_plugins() {
  $plugins['style']['ynews'] = array(
    'title' => t('Yandex.News'),
    'help' => t('Publish nodes to Yandex.News'),
    'handler' => 'ynews_style_plugin',
    'path' => drupal_get_path('module', 'ynews') . '/includes',
    'theme' => 'ynews_view',
    'theme path' => drupal_get_path('module', 'ynews') . '/includes',
    'uses row plugin' => FALSE,
    'uses options' => TRUE,
    'uses fields' => TRUE,
    'uses grouping' => FALSE,
    'type' => 'feed',
  );
  return $plugins;
}


function template_preprocess_ynews_view(&$variables) {
  $view = $variables['view'];
  $display_id = empty($view->current_display) ? 'default' : $view->current_display;
  $variables['title'] = $view->get_title();

  $options = $view->style_options;
  
  $variables['description'] = yml_safe($options['channel_description']);
  
  $variables['image_url'] = $GLOBALS['base_url'] . $GLOBALS['base_path'] . $options['logo_path'];
  
  if (!$options['title'] || !$options['nid']) {
    $msg = t('Title and Nid mappings are required, go to Yandex.News views format settings and select appropriate fields for these elements');
    drupal_set_message($msg, 'error');
  }
  
  $fields = $view->display[$display_id]->handler->get_handlers('field');

  
  $ynews_rows = array();
  foreach ($variables['rows'] as $key => $row) {
    $ynews_item = array();
    
    
    $alias = $fields[$options['title']]->field_alias;
    $ynews_item['title'] = yml_safe($row->$alias);
    
    $nid_alias = $fields[$options['nid']]->field_alias;
    if (isset($row->$nid_alias)) {
      $ynews_item['link'] = yml_safe(url('node/' . $row->$nid_alias, array('absolute' => TRUE)));
    }

    if ($options['author']) {
      $alias = $fields[$options['author']]->field_alias;
      $ynews_item['author'] = yml_safe($row->$alias);
    }
    
    if ($options['description']) {
      $alias = $fields[$options['description']]->field_alias;
      $ynews_item['description'] = yml_safe($view->style_plugin->get_field($key, $options['description']));
    }
    
    if ($options['pubdate']) {
      $alias = $fields[$options['pubdate']]->field_alias;
      $ynews_item['pubdate'] = isset($row->$alias) ? $row->$alias : NULL;
      if (is_numeric($ynews_item['pubdate'])) {
        $ynews_item['pubdate'] = date(DATE_RFC822, $ynews_item['pubdate']);
      }
    }
    
    $alias = $fields[$options['body']]->field_alias;
    $body = $view->style_plugin->get_field($key, $options['body']);
    
    //$ynews_item['body'] = isset($row->$alias) ? $row->$alias : NULL;
    $ynews_item['body'] = yml_safe($body);

    
    
    if ($options['category']) {
      $alias = $fields[$options['category']]->field_alias;
      $ynews_item['category'] = isset($row->$alias) ? yml_safe($row->$alias) : NULL;
    }
    
    
    $ynews_item['enclosure'] = array();
    foreach (array('enclosure', 'enclosure2') as $enc_name) {
      if ($options[$enc_name]) {
        $alias = $fields[$options[$enc_name]]->field_alias;
        $enclosure = $view->style_plugin->get_field($key, $options[$enc_name]);
        if (isset($enclosure)) {
          $matches = array();
          preg_match_all("#src=[\"\'](http://.*?)[\"\']#",
           $enclosure, $matches);
          if (count($matches)) {
            foreach ($matches[1] as $match) {
              $ynews_item['enclosure'][] = array(
                'url' => $match,
                'ctype' => file_get_mimetype($match)
              );
            }
          }
        }
      }
    }
    
    
    
    $ynews_rows[] = $ynews_item;
  }
  
  $variables['rows'] = $ynews_rows;
}
