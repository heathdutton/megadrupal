<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>

  <div id="page-wrapper"><div id="page">
  
    <div id="main-wrapper"><div id="main" class="clearfix">
  
      <div id="content" class="column">
      
        <?php if ($db_is_active != ""): ?>
          <div class="maintenance-ico"><?php print '<img src="' . $base_path . path_to_theme() . '/images/ico-maintenance.png" />' ?></div>
        <?php else: ?>
          <div class="maintenance-ico"><?php print '<img src="' . $base_path . path_to_theme() . '/images/ico-maintenance-nodb.png" />' ?></div>
        <?php endif; ?>
  
        <?php print render($title_prefix); ?>
        <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
  
        <div id="content" class="column">
          <?php print $messages; ?>
          <?php print $content; ?>
        </div>

      </div> <!-- /#content -->
  
    </div></div> <!-- /#main, /#main-wrapper -->
  </div></div> <!-- /#page, /#page-wrapper -->

</body>
</html>