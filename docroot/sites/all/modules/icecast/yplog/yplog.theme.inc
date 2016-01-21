<?php

function template_preprocess_yplog_graph(&$variables) {
  $title = t('Listenership graph for @listen_url', array('@listen_url' => $variables['listen_url']));
  $variables['image'] = theme('image', array('path' => $variables['filename'], 'alt' => $title, 'title' => $title, 'attributes' => array('width' => 640, 'height' => 320), 'getsize' => FALSE));
  $variables['listen_link'] = l($variables['listen_url'] . '.m3u', $variables['listen_url'] . '.m3u');
}
