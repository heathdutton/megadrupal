<?php
/**
 * @file wiredocs-button.tpl.php
 *
 * This template handles the layout of the wiredocs button.
 *
 * Variables available:
 * - $file: A full file object having file fid (fid), uri etc.
 * - $title: Button label
 */
?>
<span class="wiredocs-edit">
  <input type="submit" class="wiredocs-edit-button form-submit" value="<?php print $title; ?>">
  <input class="wiredocs-fid" type="hidden" value="<?php print $file->fid; ?>"/>
</span>