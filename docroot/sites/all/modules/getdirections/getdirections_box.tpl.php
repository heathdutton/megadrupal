<?php

/**
 * @file getdirections_box.tpl.php
 * Template file for colorbox implementation
 * Copy this file to your theme's folder and edit to match your theme.
 */

?>
<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<!-- getdirections_box -->
<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <style>
    body {
      width: 800px;
    }
    #page {
      min-width: 800px;
      width: 800px;
    }
  </style>
</head>
<body class="<?php print $body_classes; ?>">
  <div id="page-wrapper"><div id="page">
    <div id="main-wrapper" class="clearfix"><div id="main" class="clearfix">
      <div id="content" class="column"><div class="section">
        <?php if ($title): ?>
          <h2 class="title"><?php print $title; ?></h2>
        <?php endif; ?>
        <div class="content">
          <?php print $content; ?>
        </div>
      </div></div>
    </div></div>
  </div></div>
  <?php print $closure; ?>
</body>
</html>
