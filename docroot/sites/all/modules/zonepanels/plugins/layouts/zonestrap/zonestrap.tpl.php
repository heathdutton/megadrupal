<?php
/**
 * Created by PhpStorm.
 * User: ZM
 * Date: 24/9/2014
 * Time: 6:48 PM
 */
$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';

print $panel_prefix;

foreach ($variables['display']->layout_settings['zonepanels'] as $key => $zone) {
  if ( empty( $zone['zone'] )) {
    continue;
  }
  if ($admin_panels) {
    print format_string("<div class='@zone zone-admin-panels zone-count-@count'>", array(
      '@count' => $zone['zone'],
      '@zone'  => $key,
    ));
  }
  if ( ! $admin_panels && isset( $settings['zonepanels'][$key]['wrapper']['prefix'] )) {
    print $settings['zonepanels'][$key]['wrapper']['prefix'];
  }
  print '<div class="row">';

  for ($i = 1; $i <= $zone['zone']; $i++) {
    $zone_key = $key . '_' . $i;
    if (isset( $content[$zone_key] )) {
      print render($content[$zone_key]);
    }
  }

  print '</div>';

  if ( ! $admin_panels && isset( $settings['zonepanels'][$key]['wrapper']['suffix'] )) {
    print $settings['zonepanels'][$key]['wrapper']['suffix'];
  }

  if ($admin_panels) {
    print "</div>";
  }
}

print $panel_suffix;
