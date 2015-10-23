<?php
/**
 * @file drulenium-plugin.tpl.php
 * Theme the more link
 *
 * - $plugin_type: the type of this plugin
 * - $tag_name : the tagname of this plugin
 */

print $plugin_type;
print"<br/><hr/>";
print drupal_attributes($options);
print"<br/><hr/>";
print $tag_name
print"<br/><hr/>";
?>
<div class="drulenium-<?php print $plugin_type?>-plugin">
</div>

