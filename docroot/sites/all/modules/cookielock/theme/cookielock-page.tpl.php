<?php

/**
 * @file
 * Template for Cookie Lock pages. Taken from modules/system/html.tpl.php.
 *
 * @see template_preprocess_page()
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>
<head profile="<?php print $grddl_profile; ?>">
  <?php print $head; ?>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body>
  <?php if ($logo): ?>
    <div id="logo">
      <img src="<?php print $logo; ?>" alt="<?php print $head_title; ?>" />
    </div>
  <?php endif; ?>

  <?php print $messages ?>

  <?php print $content ?>
</body>
</html>
