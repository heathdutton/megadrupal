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
 * @see template_preprocess_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
<header role="banner">
  <div id="wb-bnr">
    <div id="wb-bar">
      <div class="container">
        <div class="row">
          <?php $base = theme_get_setting('base'); ?>
          <?php if($base == 0): ?>
          <object id="gcwu-sig" type="image/svg+xml" tabindex="-1" role="img" data="/sites/all/themes/wet4/assets/sig-en.svg" aria-label="Government of Canada"></object>
          <?php elseif ($base == 1): ?>
          <object id="gcwu-sig" type="image/svg+xml" tabindex="-1" role="img" data="/sites/all/themes/wet4/intranet/assets/sig-blk-en.svg" aria-label="Government of Canada"></object>
          <?php endif; ?>
          <?php if ($page['top_links']): ?>
            <?php print render($page['top_links']); ?>
          <?php endif; ?>

          <section class="wb-mb-links col-xs-12 visible-sm visible-xs" id="wb-glb-mn">
            <h2><?php print t('Search and menus'); ?></h2>
            <ul class="pnl-btn list-inline text-right">
              <li><a href="#mb-pnl" title="Search and menus" aria-controls="mb-pnl" class="overlay-lnk btn btn-sm btn-default" role="button"><span class="glyphicon glyphicon-search"><span class="glyphicon glyphicon-th-list"><span class="wb-inv">Search and menus</span></span></span></a></li>
            </ul>
            <div id="mb-pnl"></div>
          </section>
        </div>
      </div>
    </div>
    <div class="container">
      <div class="row">
        <div id="wb-sttl" class="col-md-5">
          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <div id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </div>
            <?php else:?>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>

            <?php endif; ?>
          <?php endif; ?>
        </div>
        <?php if($base == 0): ?>
        <object id="wmms" type="image/svg+xml" tabindex="-1" role="img" data="/sites/all/themes/wet4/assets/wmms.svg" aria-label="Symbol of the Government of Canada"></object>
        <?php elseif ($base == 1): ?>
        <object id="wmms" type="image/svg+xml" tabindex="-1" role="img" data="/sites/all/themes/wet4/intranet/assets/wmms-intra.svg" aria-label="Symbol of the Government of Canada"></object>
        <?php endif; ?>
        <section id="wb-srch" class="visible-md visible-lg">
          <h2><?php print t('Search'); ?></h2>
          <?php if ($page['search']): ?>
            <?php print render($page['search']); ?>
            <!-- /.section, /#search -->
          <?php endif; ?>
        </section>
      </div>
    </div>
  </div>
  <?php $lang = $GLOBALS['language']->prefix;?>
  <?php $base = $GLOBALS['base_url'];?>
  <nav role="navigation" id="wb-sm" data-ajax-fetch="<?php print $base;?>/<?php print $lang; ?>/menu/export" data-trgt="mb-pnl" class="wb-menu visible-md visible-lg" typeof="SiteNavigationElement">
    <div class="container nvbar">
      <h2><?php print t('Topics menu'); ?></h2>
      <div class="row">
        <?php if ($page['menu']): ?>
          <?php print render($page['menu']); ?>
          <!-- /.section, /#menu -->
        <?php endif; ?>
      </div>
    </div>
  </nav>
  <nav role="navigation" id="wb-bc" property="breadcrumb">
    <h2><?php print t('You are here:'); ?></h2>
    <div class="container">
      <div class="row">
        <?php if ($breadcrumb): ?>
          <?php print $breadcrumb; ?>
        <?php endif; ?>
      </div>
    </div>
  </nav>
</header>
<div class="container">
  <div class="row">
    <?php if ($page['sidebar_first']):?>
    <main role="main" property="mainContentOfPage" class="col-md-9 col-md-push-3">
      <?php else:?>
      <main role="main" property="mainContentOfPage" class="col-md-12">
        <?php endif; ?>
        <?php print $messages; ?>
        <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?><h1 id="wb-cont" property="name"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        <?php if (module_exists('flag')){$flag = flag_get_flag('archived');}?>
        <?php if (isset($node) && module_exists('flag') && $flag && $flag->is_flagged($node->nid)): ?>
          <section id="archived" class="alert alert-warning wb-inview" data-inview="archived-bnr">
            <h2><?php print t('This page has been archived on the Web'); ?></h2>
            <p><?php print t('Information identified as archived is provided for reference, research or recordkeeping purposes. It is not subject to the Government of Canada Web Standards and has not been altered or updated since it was archived. Please contact us to request a format other than those available.'); ?></p>
          </section>

          <section id="archived-bnr" class="wb-overlay modal-content overlay-def wb-bar-t">
            <header>
              <h2 class="wb-inv"><?php print t('Archived'); ?></h2>
            </header>
            <p><a href="#archived"><?php print t('This page has been archived on the Web.'); ?></a></p>
          </section>
        <?php endif; ?>
        <?php if ($page['sidebar_aside']): ?>
          <aside id="sidebar-aside" class="sidebar col-md-4 pull-right"><div class="section well">
              <?php print render($page['sidebar_aside']); ?>
            </div></aside> <!-- /.section, /#sidebar-aside -->
        <?php endif; ?>
        <div class="node-body clearfix">
        <?php print render($page['content']); ?>
        </div>
        <?php if (isset($node)):?>
          <dl id="wb-dtmd">
            <dt><?php print t('Date modified:'); ?> </dt>
            <dd><time property="dateModified"><?php print format_date($node->changed, 'date_modified'); ?></time></dd>
          </dl>
        <?php endif;?>
      </main>

      <?php if ($page['sidebar_first']): ?>
        <nav id="sidebar-first" class="column sidebar col-md-3 col-md-pull-9 visible-md visible-lg">
            <?php print render($page['sidebar_first']); ?>
          </nav> <!-- /.section, /#sidebar-first -->
      <?php endif; ?>

  </div>
</div>
<footer role="contentinfo" id="wb-info" class="visible-sm visible-md visible-lg wb-navcurr">
  <div class="container">

    <nav role="navigation">
      <h2><?php print t('About this site'); ?></h2>
      <?php if ($page['about_site'] && theme_get_setting('base') == 0): ?>
        <?php print render($page['about_site']); ?>
        <!-- /.section, /#about_site -->
      <?php endif; ?>
      <div class="row">
        <?php if ($page['footer_1']): ?>
          <section class="col-sm-3">
            <?php print render($page['footer_1']); ?>
          </section> <!-- /.section, /#footer_1 -->
        <?php endif; ?>
        <?php if ($page['footer_2']): ?>
          <section class="col-sm-3">
            <?php print render($page['footer_2']); ?>
          </section> <!-- /.section, /#footer_3 -->
        <?php endif; ?>
        <?php if ($page['footer_3']): ?>
          <section class="col-sm-3">
            <?php print render($page['footer_3']); ?>
          </section> <!-- /.section, /#footer_3 -->
        <?php endif; ?>
        <?php if ($page['footer_4']): ?>
          <section class="col-sm-3">
            <?php print render($page['footer_4']); ?>
          </section> <!-- /.section, /#footer_4 -->
        <?php endif; ?>
      </div>
    </nav>
  </div>
  <div id="gc-info">
    <div class="container">
      <nav role="navigation">
        <h2><?php print t('Government of Canada footer'); ?></h2>
        <?php if ($page['footer_bottom']): ?>
          <?php print render($page['footer_bottom']); ?>
          <!-- /.section, /#footer_bottom -->
        <?php endif; ?>
      </nav>
    </div>
  </div>
</footer>
