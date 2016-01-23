<?php 
/**
 * @file
 * 
 * Template file for recent posts Jump list javascript.
 */
?>
window.onload = function() {
  try {
    if (window.external.msIsSiteMode()) {
      window.external.msSiteModeClearJumpList();
      window.external.msSiteModeCreateJumpList("<?php print $category_name;?>");
      <?php foreach ($recent_posts as $post) { ?>
        window.external.msSiteModeAddJumpListItem("<?php print $post['title']; ?>", "<?php print $post['url']; ?>", "<?php print $icon; ?>");
      <?php } ?>
      window.external.msSiteModeShowJumplist();
    }
  }
  catch (ex) {}
}
