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

        <header class="navigation" role="navigation">
            <div class="container">
                <?php if ($title): ?>
                <h1><?php print $title; ?></h1>
                <?php endif; ?>
            </div>
        </header> <!-- /#navigation -->

        <div class="page container">

            <div class="main row">

                <?php if ($sidebar_first): ?>
                <aside class="sidebar col-md-3">
                    <?php if ($logo): ?>
                    <img class="logo" src="<?php print $logo ?>" alt="<?php print $site_name ?>" />
                    <?php endif; ?>
                    <?php print $sidebar_first; ?>
                </aside>
                <?php endif; ?>

                <div class="main-content <?php print $layout; ?>" role="main">
                    <a id="main-content"></a>
                    <?php print $messages; ?>
                    <?php print $help; ?>
                    <?php print $content; ?>
                </div> <!-- /#main-content -->

            </div> <!-- /#main -->

        </div> <!-- /#page -->

        <?php print $page_bottom; ?>

    </body>
</html>
