<?php

/**
 * Cron callback.
 */
function _lazyloaded_cron() {
  $plugins = lazyloaded_get_plugins();
  foreach ($plugins as $name => $plugin) {
    $types = variable_get("lazyloaded_{$name}_types", array());
    $types = array_keys(array_filter($types));
    if ($types && !empty($plugin->data)) {
      $args = array();
      for ($i = 0; $i < count($types); ++$i) {
        $args[':type' . $i] = $types[$i];
      }
      $args[':plugin'] = $plugin->name;
      $args[':expire_time'] = $plugin->data_expire_time;
      $sql = 'SELECT n.nid
      FROM {node} n
      LEFT JOIN {lazyloaded_data} ld ON n.nid = ld.nid AND ld.plugin = :plugin
      WHERE (ld.nid IS NULL OR (
        ld.last_sync < UNIX_TIMESTAMP() - :expire_time
      ))
      AND n.type IN (' . implode(', ', array_keys($args)) . ')
      ORDER BY n.nid DESC
      LIMIT 20';
      $res = db_query($sql, $args);
      while ($node = db_fetch_object($res)) {
        lazyloaded_sync($node->nid, $plugin);
      }
    }
  }
}
