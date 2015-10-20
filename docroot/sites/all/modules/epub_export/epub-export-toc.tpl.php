<?php

/**
 * @file
 * Default theme implementation to display the Table of Contents (toc)
 *
 * Variables:
  * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $content: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see template_preprocess()
 * @see template_preprocess_epub()
 * @see template_process()
 */
?><?php print '<?xml version="1.0" encoding="UTF-8" standalone="no"?>'; ?>
<html xmlns="http://www.w3.org/1999/xhtml"  xml:lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" xmlns:epub="http://www.idpf.org/2007/ops">

<head>
  <?php print $head; ?>
  <?php if (!empty($head_title)) { ?>
  <title><?php print $head_title; ?></title>
  <?php }?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <nav epub:type="toc" id="toc">
  <h1 class="title">Table of Contents</h1>
  <ol>
    <?php print $links; ?>
  </ol>
  </nav>
  <?php print $page_bottom; ?>
</body>
</html>
