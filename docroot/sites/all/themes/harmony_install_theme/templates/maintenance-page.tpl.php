<?php

/**
 * @file
 * Maintenance page.
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
  <body class="<?php print $classes; ?>">
    <div class="container">
      <div class="header">
        <a class="logo" href="http://getharmony.io" target="_blank">Harmony</a>
      </div>

      <div class="body">
        <?php if (!empty($title)): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php if (!empty($messages)): print $messages; endif; ?>
        <?php print $content; ?>
      </div>

      <footer class="footer text-center text-muted">
        <p><small>Need some help? Come over to <a href="http://getharmony.io/installing" target="_blank">http://getharmony.io/installing</a>.</small></p>
      </footer>
    </div>
  </body>
</html>
