<?php
/**
 * @file
 * Search block template.
 */
?>
<form id="mkdru-block-search-<?php print $nid ?>" target="_blank" onsubmit="document.location.href = '<?php print $path ?>#query=' + jQuery('#mkdru-block-search-<?php print $nid ?> input:text').attr('value'); return false;">
  <input type="text" width="100%"/></br>
  <input type="submit" value="<?php print t("Search") ?>">
</form>
