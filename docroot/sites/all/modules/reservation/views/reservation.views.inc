<?php

function reservation_views_data_alter(&$data) {
  $data['reservation']['uid']['field']['handler'] = 'views_handler_field_user';
  $data['reservation']['uid']['relationship'] = array(
    'title' => 'User',
    'help' => 'Relate a reservation to the user who created it.',
    'handler' => 'views_handler_relationship',
    'base' => 'users',
    'field' => 'uid',
    'label' => 'user',
  );
}
