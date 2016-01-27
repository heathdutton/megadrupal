<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"
  <?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <meta http-equiv="X-UA-Compatible" content="IE=Edge" />
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <meta name = "viewport" content = "user-scalable=no, initial-scale=1.0, maximum-scale=1.0, width=device-width" />
  <meta name="apple-mobile-web-app-capable" content="yes"/>
  <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent" />
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes; ?><?php if (isset($background)): ?> style="<?php print $background; ?>;"<?php endif; ?>>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
