<?php

function paas_metric_views_call($value) {
  switch ($value) {
    case 'views':
      paas_dump_csv('SELECT vid, name, base_table FROM {views_view}');
      break;
    case 'displays':
      paas_dump_csv('SELECT vid, id, display_title, display_plugin, position, display_options FROM {views_display}');
      break;
  }
}
