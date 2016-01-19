<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see twbs_preprocess_maintenance_page()
 *
 * @ingroup themeable
 */
?>
<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <?php print $head; ?>
        <title><?php print $head_title; ?></title>
        <?php print $styles; ?>
        <?php print $scripts; ?>
    </head>
    <body class="<?php print $classes; ?>" <?php print $attributes;?>>
        <div id="skip-link">
            <a href="#main-content" class="sr-only"><?php print t('Skip to main content'); ?></a>
        </div>

        <?php print $page_top; ?>

        <header class="navigation navbar navbar-default navbar-static-top" role="navigation">
            <div class="container">
                <div class="navbar-header">
                    <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#navigation .navbar-collapse">
                        <span class="sr-only">Toggle navigation</span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                    </button>
                    <?php if ($logo): ?>
                    <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="logo navbar-brand">
                        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
                    </a>
                    <?php endif; ?>
                    <?php if ($site_name): ?>
                    <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="site-name navbar-brand">
                        <?php print $site_name; ?>
                    </a>
                    <?php endif; ?>
                    <?php if ($site_slogan): ?>
                    <div class="site-slogan navbar-text">
                        <?php print $site_slogan; ?>
                    </div>
                    <?php endif; ?>
                </div> <!-- /.navbar-header -->
                <div class="navbar-collapse collapse">
                    <?php print theme('links__system_main_menu', array(
                        'links' => $main_menu,
                        'attributes' => array(
                            'class' => array(
                                'main-menu',
                                'nav',
                                'navbar-nav',
                            ),
                        ),
                    )); ?>
                    <?php print theme('links__system_secondary_menu', array(
                        'links' => $secondary_menu,
                        'attributes' => array(
                            'class' => array(
                                'secondary-menu',
                                'nav',
                                'navbar-nav',
                                'navbar-right',
                            ),
                        ),
                    )); ?>
                </div> <!-- /.nav-collapse -->
            </div>
        </header> <!-- /#navigation -->

        <div class="page container">

            <?php print $header; ?>

            <div class="main row">

                <?php if ($sidebar_first): ?>
                <aside class="sidebar col-md-3">
                    <?php print $sidebar_first; ?>
                </aside>
                <?php endif; ?>

                <div class="main-content <?php print $layout; ?>" role="main">
                    <?php print $highlighted; ?>
                    <?php print $breadcrumb; ?>
                    <a id="main-content"></a>
                    <?php print render($title_prefix); ?>
                    <?php if ($title): ?>
                    <div class="page-header">
                        <h1><?php print $title; ?></h1>
                    </div>
                    <?php endif; ?>
                    <?php print render($title_suffix); ?>
                    <?php print $messages; ?>
                    <?php print render($tabs); ?>
                    <?php print $help; ?>
                    <?php print $content; ?>
                    <?php print $feed_icons; ?>
                </div> <!-- /#main-content -->

                <?php if ($sidebar_second): ?>
                <aside class="sidebar col-md-3">
                    <?php print $sidebar_second; ?>
                </aside>
                <?php endif; ?>

            </div> <!-- /#main -->

            <?php print $footer; ?>

        </div> <!-- /#page -->

        <?php print $page_bottom; ?>

    </body>
</html>
