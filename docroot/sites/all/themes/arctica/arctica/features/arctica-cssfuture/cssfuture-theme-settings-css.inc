<?php
global $abs_arctica_theme_path;
$selectors = theme_get_setting('process_selectors', $theme);
if ($selectors && theme_get_setting('futurecss_enable', $theme)) {
  $CSS .= "\n\n";
  $CSS .= "$selectors {behavior:url({$abs_arctica_theme_path}/features/arctica-cssfuture/PIE/PIE.php);}\n";
}