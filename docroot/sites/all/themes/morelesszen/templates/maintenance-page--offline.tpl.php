<?php

/**
 * @file
 * This file controls the page that is displayed when Drupal cannot 
 * access the database (for whatever reason).
 *
 * Implementation to display a single Drupal page while offline.
 *
 * Database is not accessable, so key settings must be hardcoded
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 * @see bartik_process_maintenance_page()
 */
  
  $head_title = 'Maintenance';
  $title = 'Sorry!';
  $logo = path_to_theme() .'/logo.png';
  // If your theme is set to display the site name, uncomment this line and replace the value:
  // $site_name = 'Your Site Name';
  // If your theme is set to *not* display the site name, uncomment this line:
  unset($site_name);
  // If your theme is set to display the site slogan, uncomment this line and replace the value:
  // $site_slogan = 'My Site Slogan';
  // If your theme is set to *not* display the site slogan, uncomment this line:
  unset($site_slogan);
  // Main message. Note HTML markup.
  $content = '<p>This site is currently under maintenance. We should be back shortly. Thank you for your patience.</p>';
  
?>

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

  <div id="page-wrapper">
    <div id="page">

      <div id="header">
        <div class="section middle clearfix">
        
          <?php if ($logo): ?>
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="logo">
              <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
            </a>
          <?php endif; ?>
          
        </div> <!-- /.section -->
      </div> <!-- /#header -->

      <div id="main-wrapper">
        <div id="main" class="clearfix middle">
          <div id="content" class="column"><div class="section">

            <?php if ($title): ?>
              <h1 class="title" id="page-title"><?php print $title; ?></h1>
            <?php endif; ?>

            <?php print $content; ?>

            <?php if ($messages): ?>
              <div id="messages">
                <div class="section clearfix"><?php print $messages; ?></div>
              </div> <!-- /#messages -->
            <?php endif; ?>

          </div></div> <!-- /.section, /#content -->
        </div> <!-- /#main -->
      </div> <!-- /#main-wrapper -->

    </div> <!-- /#page -->
  </div> <!-- /#page-wrapper -->


</body>
</html>