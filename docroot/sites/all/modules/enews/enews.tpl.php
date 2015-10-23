<?php
/**
 * Newsletter template file.  Copy this file to the active theme directory to 
 * override it.  You can create a different template for each node type by 
 * copying this to the active theme directory and renaming it enews-{type}.tpl.php
 * where [type] is the machine readable name of the node type.
 * 
 * NOTE: The base theme "enews.tpl.php" MUST exist in the same directory as any
 * template suggestions.  Therefore one MUST include a copy of enews.tpl.php in
 * the active theme directory to use node type specific template overrides 
 * (enews-page.tpl.php).
 * 
 * 
 * Available variables.
 * $node - the node object
 * $content - the rendered node object
 * All regions from the current active theme are available as well.
 * @see path/to/active/theme.info
 * $header - header region from theme
 * $footer - footer region from theme
 * $closure - closure region from theme
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
  <head>
    <meta http-equiv="Content-type" content="text/html; charset=utf-8">
    <title><?php print $title . ' : ' . $site_name; ?></title>
  </head>
  <body id="enews.tpl">
    <?php print $header; ?>
    <h2><?php print $title; ?></h2>
    <div id="content">
      <?php print $content; ?>
    </div>
    <?php print $footer; ?>
    <?php print $closure; ?>
  </body>
</html>