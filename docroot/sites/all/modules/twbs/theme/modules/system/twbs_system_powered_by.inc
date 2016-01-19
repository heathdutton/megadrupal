<?php

/**
 * Returns HTML for the Powered by Drupal text.
 *
 * @ingroup themeable
 */
function twbs_system_powered_by() {
  return '<p>' . t('Powered by <a href="@poweredby">Drupal</a>', array('@poweredby' => 'http://drupal.org')) . '</p>';
}
