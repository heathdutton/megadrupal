<?php
/**
 * @file
 * The template providing the page layout, placed within the $page function
 * in the html.tpl.php file.
 *
 * @param $page
 *  Filled with variables for building regions of the page.
 * @param $linked_site_logo
 *  Provides the theme's logo with a link to the home page.
 * @param $site_name
 *  Prints the site name that was entered during the installation of the site.
 * @param $site_slogan
 *  Prints the site slogan that is entered into the site information.
 * @param $primary_navigation
 *  The primary menu links, which defaults to the Main Menu.
 * @param $secondary_navigation
 *  The menu item for secondary navigation links, which defaults to user links.
 * @param $container
 *  Holds the class for the theme setting telling # of columns in grid.
 * @param $main_grid_classes
 *  Holds the classes for the main content area that apply the grid settings.
 * @param $messages
 *  Displays help, info, and error messages from the Drupal system.
 * @param $primary_local_tasks
 *  The primary tabs for Drupal Admin (includes 'View' and 'Edit' for nodes).
 * @param $secondary_local_tasks
 *  The secondary links for the administration tabs, mostly found in admin
 *  areas.
 * @param $action_links
 *  Prints admin links based on context of the administration area.
 * @param $feed_icons
 *  Displays an RSS feed icon that links to Drupal's default RSS feed.
 *
 * This is a page template that has 100% width header and footer
 * backgrounds. Additionally you will notice most of the elements have new
 * wrapper divs, so in effect you can style things like the main menu,
 * breadcrumbs, secondary content region etc with 100% width backgrounds. This
 * template is compatible with AT's normal page layout settings (just the
 * backgrounds will be 100%).
 *
 */

?>
<div id="page-wrapper">
  <div class="overlay"></div>
  <div id="page" class="<?php print $theme_layout;?> <?php print $frame;?>">

    <?php if($page['toolbar']): ?>
      <div id="toolbar-wrapper">
        <div class="container <?php print $container;?>
    clearfix">
          <div class="inner">
            <?php print render($page['toolbar']); ?>
          </div>
        </div>
      </div>
  <?php endif; ?>

    <div id="header-wrapper">
      <div class="container <?php print $container;?> clearfix">
        <div class="inner">
          <header class="clearfix">

            <?php if ($linked_site_logo): ?>
              <div id="logo"><?php print $linked_site_logo; ?></div>
            <?php endif; ?>

            <?php if ($site_name || $site_slogan): ?>
              <hgroup<?php if (!$site_slogan && $hide_site_name): ?> class="<?php print $visibility; ?>"<?php endif; ?>>
                <?php if ($site_name): ?>
                  <h1 id="site-name"<?php if ($hide_site_name): ?> class="<?php print $visibility; ?>"<?php endif; ?>>
                  <a href="<?php print base_path();?>"><?php print $site_name; ?></a>
                </h1>
                <?php endif; ?>
                <?php if ($site_slogan): ?>
                  <h2 id="site-slogan"><?php print $site_slogan; ?></h2>
                <?php endif; ?>
              </hgroup>
            <?php endif; ?>

            <?php if ($secondary_navigation): print $secondary_navigation; endif; ?>
            <?php print render($page['header']); ?>

          </header>
        </div>
      </div>
    </div>

    <?php if ($page['menu_bar'] || $primary_navigation || $secondary_navigation): ?>
      <div id="nav-wrapper">
        <div class="container <?php print $container;?> clearfix">
          <?php print render($page['menu_bar']); ?>
          <?php if ($primary_navigation): print $primary_navigation; endif; ?>
        </div>
      </div>
    <?php endif; ?>

    <?php if ($page['secondary_content']): ?>
      <div id="secondary-content-wrapper">
        <div class="container <?php print $container;?> clearfix">
          <div class="inner">
            <?php print render($page['secondary_content']); ?>
          </div>
        </div>
      </div>
    <?php endif; ?>

    <?php if ($breadcrumb): ?>
      <div id="breadcrumb-wrapper">
        <div class="container <?php print $container;?> clearfix">
          <div class="inner">
            <section id="breadcrumb" class="clearfix">
              <?php print $breadcrumb; ?>
            </section>
          </div>
        </div>
      </div>
    <?php endif; ?>

    <div id="content-wrapper">
      <div class="container <?php print $container;?>">
        <div class="inner">
          <div id="columns">
            <div class="columns-inner clearfix">
              <div id="content-column">
                <div class="content-inner  <?php print $main_grid_classes;?>">

                  <?php print render($page['highlighted']); ?>

                  <?php $tag = $title ? 'section' : 'div'; ?>
                  <<?php print $tag; ?> id="main-content">

                    <?php if ($title || $primary_local_tasks || $secondary_local_tasks || $action_links): ?>
                      <header>

                        <?php if ($primary_local_tasks || $secondary_local_tasks || $action_links): ?>
                          <div id="tasks">
                            <?php if ($primary_local_tasks ||
                            $secondary_local_tasks):?>
                              <div id="tabs">
                                <?php if ($primary_local_tasks): ?>
                                  <ul class="tabs primary">
                                    <?php print render($primary_local_tasks); ?>
                                  </ul>
                                <?php endif; ?>
                                <?php if ($secondary_local_tasks): ?>
                                  <ul class="tabs secondary">
                                    <?php print render($secondary_local_tasks); ?>
                                  </ul>
                                <?php endif; ?>
                              </div>
                            <?php endif;?>
                            <?php if ($action_links): ?>
                              <ul class="action-links">
                                <?php print render($action_links); ?>
                              </ul>
                            <?php endif; ?>
                          </div>
                        <?php endif; ?>

                        <?php print render($title_prefix); ?>
                          <?php if ($title): ?>
                            <h1 id="page-title"><?php print $title; ?></h1>
                          <?php endif; ?>
                        <?php print render($title_suffix); ?>

                      </header>
                    <?php endif; ?>

                  <?php if ($messages || $page['help']): ?>
                  <div id="messages-help-wrapper">
                    <div class="clearfix">
                      <div class="inner">
                        <?php print $messages; ?>
                        <?php print render($page['help']); ?>
                      </div>
                    </div>
                  </div>
                  <?php endif; ?>


                    <div id="content" class="<?php print $tab_class;?>">
                      <?php print render($page['content']); ?>
                    </div>

                    <?php print $feed_icons; ?>

                  </<?php print $tag; ?>>

                  <?php print render($page['content_aside']); ?>
                </div>
              </div>

          </div>
        </div>
      </div>
      </div>
    </div>

    <?php if ($page['tertiary_content']): ?>
      <div id="tertiary-content-wrapper">
        <div class="container <?php print $container;?> clearfix">
          <div class="inner">
            <?php print render($page['tertiary_content']); ?>
          </div>
        </div>
      </div>
    <?php endif; ?>

    <?php if ($page['footer']): ?>
      <div id="footer-wrapper">
        <div class="container <?php print $container;?> clearfix">
          <div class="inner">
            <footer class="clearfix">
              <?php print render($page['footer']); ?>
            </footer>
          </div>
        </div>
      </div>
    <?php endif; ?>
  </div>
</div>
