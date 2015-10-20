<?php

/**
 * @file maintenance-page.tpl.php
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
  <head>
    <?php print $head; ?>
    <title><?php print $head_title; ?></title>
    <?php print $styles; ?>
    <?php print $scripts; ?>
    <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyled Content in IE */ ?> </script>
  </head>
  <body<?php print $attributes; ?>>
    <?php if (!empty($logo)): ?>
      <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
    <?php endif; ?>
    <h1 id="site-name"><?php print $site_name; ?></h1>
    <h2 class="title" id="page-title"><?php print $title; ?></h2>
    <?php print $content; ?>
    <?php print $footer_message; ?>
    <?php print $footer; ?>
    <?php print $closure; ?>
  </body>
</html>
