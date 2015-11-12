<?php
/**
 * @file wiredocs-applet.tpl.php
 *
 * This template handles the layout of the wiredocs applet.
 *
 * Variables available:
 * - $file: A full file object having file fid (fid), uri etc.
 * - $archiveJar: URL to main java applet archive
 * - $downloadURL: Download URL
 * - $uploadURL: Upload URL
 * - $cookie: Cookie string with session name and id (separated by "=")
 */
?>
<object id="wiredocs-file-<?php  print $file->fid; ?>" name="wiredocs-file-<?php  print $file->fid; ?>" class="wiredocs-applet" type="application/x-java-applet" width="1" height="1">
  <param name="code" value="org.osce.wiredocs.WireDocsApplet" />
  <param name="archive" valuetype="ref" type="application/java" value="<?php print $archiveJar; ?>" />
  <param name="downloadURL" valuetype="ref" value="<?php print $downloadURL; ?>">
  <param name="uploadURL" valuetype="ref" value="<?php print $uploadURL; ?>">
  <param name="filename" value="<?php print $file->filename; ?>" />
  <param name="cookie" value="<?php print $cookie; ?>" />
  <param name="scriptable" value="true" />
  <param name="mayscript" value="true" />
</object>