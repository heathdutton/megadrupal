<?php // $Id: page.tpl.php,v 1.2 2009/02/16 16:18:13 njt1982 Exp $ ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>
  </head>
  <body<?php print phptemplate_body_class($left, $right); ?>>
    <div id="header">
      <div class="content">
        <!-- Header -->
        <div id="logo-floater">
        <?php
          // Prepare header
          $site_fields = array();
          if ($site_name) {
            $site_fields[] = check_plain($site_name);
          }
          if ($site_slogan) {
            $site_fields[] = check_plain($site_slogan);
          }
          $site_title = implode(' ', $site_fields);
          $site_fields[0] = '<span>'. $site_fields[0] .'</span>';
          $site_html = implode(' ', $site_fields);

          if ($logo || $site_title) {
            print '<h1><a href="'. check_url($base_path) .'" title="'. $site_title .'">';
            if ($logo) {
              print '<img src="'. check_url($logo) .'" alt="'. $site_title .'" id="logo" />';
            }
            print $site_html .'</a></h1>';
          }
        ?>
        </div>

        <?php if (isset($primary_links)) : ?>
          <?php print theme('links', $primary_links, array('class' => 'links primary-links')) ?>
        <?php endif; ?>
        <?php if (isset($secondary_links)) : ?>
          <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')) ?>
        <?php endif; ?>

        <?php print $header ?>
      </div>
      <div class="r5"></div><div class="r4"></div><div class="r3"></div><div class="r2"></div><div class="r1 b"></div>
    </div>

    <div id="center" class="column">
      <div class="r1 t"></div><div class="r2"></div><div class="r3"></div><div class="r4"></div><div class="r5"></div>
      <div class="content">
        <!-- Main Content -->
        <?php if ($breadcrumb): print $breadcrumb; endif; ?>
        <?php if ($mission): print '<div id="mission">'. $mission .'</div>'; endif; ?>

        <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
        <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
        <?php if ($tabs): print $tabs .'</div>'; endif; ?>

        <?php if (isset($tabs2)): print $tabs2; endif; ?>

        <?php if ($help): print $help; endif; ?>
        <?php if ($messages): print $messages; endif; ?>
        <?php print $content ?>
        <div class="clear-block"></div>
        <?php print $feed_icons ?>
      </div>
      <div class="r5"></div><div class="r4"></div><div class="r3"></div><div class="r2"></div><div class="r1 b"></div>
    </div>

    <?php if ($left) : ?>
    <div id="left" class="column">
      <!-- Left Sidebar -->
      <?php print $left ?>
    </div>
    <?php endif; ?>

    <?php if ($right): ?>
    <div id="right" class="column">
      <!-- Right Sidebar -->
      <?php print $right ?>
    </div>
    <?php endif; ?>

    <?php if ($footer || $footer_message): ?>
    <div id="footer">
      <div class="r1 t"></div><div class="r2"></div><div class="r3"></div><div class="r4"></div><div class="r5"></div>
      <div class="content clear-block">
        <!-- Footer Contents -->
        <?php print $footer . $footer_message ?>
      </div>
    </div>
    <?php endif; ?>
    <?php print $closure; ?>
  </body>
</html>
