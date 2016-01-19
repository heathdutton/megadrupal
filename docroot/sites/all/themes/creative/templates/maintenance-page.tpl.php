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


<header id="header">
  <div class="top-bg"></div>
  <div class="layout-978">
    <hgroup id="logo-wrap">
      <h1 id="site-name">
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
           <?php if ($logo): ?><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /><?php endif; ?>
          <span><?php print $site_name; ?></span>
        </a>
      </h1>
       <?php if ($site_slogan): ?><h2 id="site-slogan"><?php print $site_slogan; ?></h2><?php endif; ?>
    </hgroup>
    <div class="row-end"></div>
  </div>
</header>

<div id="main" class="layout-978">
  <section id="content" role="main" class="col12 no-margin-left">
    <?php print $messages; ?>
    <a id="main-content"></a>
    <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
    <?php print $content; ?><br/><br/>
  </section> <!-- /#main -->
</div>

</body>
</html>
