<?php

/**
 * @file
 * weblinks-link.tpl.php
 */

?>
<div class="weblinks weblinks-item weblinks-link-<?php print $variables['node']->nid;?>">
  <?php
  if ($teaser && variable_get('weblinks_show_title', TRUE)) {
    print '<h2 class="title">' . $title . '</h2>';
  }
?>

<?php print $link_status; ?>
<?php
  if (!$teaser || variable_get('weblinks_show_url', TRUE)) {
    print $link;
  }
?>

<div class="weblinks-body">
<?php print $weblinks_body; ?>
</div><!--class="weblinks-body"-->

<?php
  if (isset($click_count) && $click_count > 0) {
    print '<div class="weblinks-click-stats">'. t('Clicked !count times. Last clicked !last.', array('!count' => $click_count, '!last' => $last_click)) .'</div><!--class="weblinks-click-stats"-->';
  }
?>
</div><!--class="weblinks weblinks-item weblinks-link-<?php print $variables['node']->nid;?>"-->
