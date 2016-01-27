<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */
?>
<!doctype html>
<!--[if lt IE 9]><html class="ie" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf->version . $rdf->namespaces; ?>><![endif]-->
<!--[if gte IE 9]><!--><html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf->version . $rdf->namespaces; ?>><!--<![endif]-->
<head>
<head profile="<?php print $grddl_profile; ?>">
<meta charset="utf-8"/>
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1"/>
<title>Golden Grid System Demo</title>
<meta name="viewport" content="width=device-width, initial-scale=1"/>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body>
    <header>
      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>

      <?php if ($site_name || $site_slogan): ?>
        <div id="name-and-slogan">
          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <div id="site-name"><strong>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </strong></div>
            <?php else: /* Use h1 when the content title is empty */ ?>   
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>   
            <?php endif; ?>
          <?php endif; ?>
 
          <?php if ($site_slogan): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div> <!-- /#name-and-slogan -->
      <?php endif; ?>
 
      <?php print render($header); ?>
    </header>
 
    <?php print $messages; ?>
 
    <?php if ($sidebar_first): ?>
      <aside id="sidebar-first">
        <?php print render($sidebar_first); ?>
      </aside>
    <?php endif; ?>
 
    <div id="content">
      <?php if ($highlighted): ?><div id="highlighted"><?php print render($highlighted); ?></div><?php endif; ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): ?><menu class="tabs"><?php print render($tabs); ?></menu><?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?><menu class="action-links"><?php print render($action_links); ?></menu><?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div>
 
    <?php if ($sidebar_second): ?>
      <aside id="sidebar-second" class="column sidebar">
        <?php print render($sidebar_second); ?>
      </aside>
    <?php endif; ?>

    <footer>
      <?php print render($footer); ?>
    </footer>
</body>
</html>
