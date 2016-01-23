<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in page.tpl.php. Some may be left
 * blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */
?>
<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>

<div id="wrapper">
  <div class="inwrap">
    <div id="header-top">
        <div class="logo">
          <?php if ($logo): ?>
           <div id="logoimg">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
            </div>
          <?php endif; ?>
         <div class="sitename">
            <h1><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
            <h2><?php print $site_slogan; ?></h2>
         </div>
        </div>
    </div>
 <div id="content-container">

  <?php print render($page['header']); ?>
  <section id="main" role="main" class="clearfix">
    <?php print $messages; ?>
    <a id="main-content"></a>
    <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
    <?php print $content; ?>
  </section> <!-- /#main -->
  <br/>
  </div>
  </div> <!-- /#wrapper -->
</div>
</body>
</html>