<?php

/**
 * @file
 * Default theme implementation to display help for a single permission.
 *
 * Available variables:
 * - $permission: the permission key defined via hook_permission().
 */
?>
<div class="description">
  <em class="permission-warning permission-key">
    <?php print t('Permission: !permission', array('!permission' => $permission)) ?>
  </em>
</div>