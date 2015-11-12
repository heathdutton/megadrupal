<?php

/**
 * Preprocesses variables for block-admin-display-form.tpl.php.
 */
function twbs_preprocess_dashboard_admin_display_form(&$variables) {
  twbs_preprocess_block_admin_display_form($variables);
  if (isset($variables['block_regions'][BLOCK_REGION_NONE])) {
    $variables['block_regions'][BLOCK_REGION_NONE] = t('Other blocks');
  }
}
