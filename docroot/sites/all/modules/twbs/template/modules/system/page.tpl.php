<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see twbs_preprocess_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
<header class="navigation navbar navbar-default navbar-static-top" role="navigation">
    <div class="container">
        <div class="navbar-header">
            <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navigation .navbar-collapse">
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
</header> <!-- /.navigation -->

<div class="page container">

    <?php print render($page['header']); ?>

    <div class="main row">

        <?php if ($page['sidebar_first']): ?>
        <aside class="sidebar col-md-3">
            <?php print render($page['sidebar_first']); ?>
        </aside>
        <?php endif; ?>

        <div class="main-content <?php print $layout; ?>" role="main">
            <?php print render($page['highlighted']); ?>
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
            <?php print render($page['help']); ?>
            <?php if ($action_links): ?>
            <ul class="action-links"><?php print render($action_links); ?></ul>
            <?php endif; ?>
            <?php print render($page['content']); ?>
            <?php print $feed_icons; ?>
        </div> <!-- /.main-content -->

        <?php if ($page['sidebar_second']): ?>
        <aside class="sidebar col-md-3">
            <?php print render($page['sidebar_second']); ?>
        </aside>
        <?php endif; ?>

    </div> <!-- /.main -->

    <?php print render($page['footer']); ?>

</div> <!-- /.page -->
