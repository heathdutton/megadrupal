<?php

function hook_share_light_channel_info() {
  $channels['email'] = '\\Drupal\\share_light\\Email';
  return $channels;
}
