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
 * @see bootstrap_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see bootstrap_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
<header id="navbar" role="banner" class="<?php print $navbar_classes; ?>">
  <div id="wb-bnr" class="container">
    <section id="wb-lng" class="visible-md visible-lg text-right">
      <h2 class="wb-inv"><?php print t('Language selection'); ?></h2>
      <div class="row">
        <div class="col-md-12">
          <?php print $menu_bar; ?>
        </div>
      </div>
    </section>
    <div class="row">
      <div class="brand col-xs-8 col-sm-9 col-md-6">
          <?php if ($site_name || $site_slogan || $logo): ?>
            <a href="<?php print 'http://www.canada.ca/' . $language; ?>">
            <?php if ($logo && $logo_svg): ?>
              <object id="header-logo" data='<?php print $logo_svg; ?>' role="img" tabindex="-1" type="image/svg+xml">
                <img alt="<?php print t('Government of Canada'); ?>" src="<?php print $logo; ?>"  />
              </object>
            <?php elseif ($logo): ?>
              <img alt="<?php print t('Government of Canada'); ?>" src="<?php print $logo; ?>"  />
            <?php endif; ?>
          </a>
        <?php endif; ?>
      </div>
      <section class="wb-mb-links col-xs-4 col-sm-3 visible-sm visible-xs" id="wb-glb-mn">
        <h2><?php print t('Search and menus'); ?></h2>
        <ul class="list-inline text-right chvrn">
          <li>
            <a href="#mb-pnl" title="<?php print t('Search and menus'); ?>" aria-controls="mb-pnl" class="overlay-lnk" role="button">
              <span class="glyphicon glyphicon-search">
                <span class="glyphicon glyphicon-th-list">
                  <span class="wb-inv"><?php print t('Search and menus'); ?></span>
                </span>
              </span>
            </a>
          </li>
        </ul>
        <div id="mb-pnl"></div>
      </section>
        <section id="wb-srch" class="col-xs-6 text-right visible-md visible-lg">
            <h2 class="wb-inv"><?php print t('Search'); ?></h2>
            <?php if ($search_box): ?>
              <?php print $search_box; ?>
            <?php endif; ?>
        </section>
    </div>
  </div>
  <?php if (!empty($gcweb_cdn)): ?>
    <nav role="navigation" id="wb-sm" class="wb-menu visible-md visible-lg" data-trgt="mb-pnl" data-ajax-replace="//cdn.canada.ca/gcweb-cdn-live/sitemenu/sitemenu-<?php print $language; ?>.html" typeof="SiteNavigationElement">
      <div class="pnl-strt container nvbar">
        <h2 class="wb-inv"><?php print t('Topics menu'); ?></h2>
        <div class="row">
          <?php print render($page['mega_menu']); ?>
        </div>
      </div>
    </nav>
  <?php else: ?>
    <nav role="navigation" id="wb-sm" class="wb-menu visible-md visible-lg" data-trgt="mb-pnl">
      <div class="pnl-strt container nvbar">
        <h2 class="wb-inv"><?php print t('Topics menu'); ?></h2>
        <div class="row">
          <?php print render($page['mega_menu']); ?>
        </div>
      </div>
    </nav>
  <?php endif; ?>
  <?php print render($page['header']); ?>
  <nav role="navigation" id="wb-bc" property="breadcrumb">
    <div class="container">
      <div class="row">
        <?php print render($breadcrumb); ?>
      </div>
    </div>
  </nav>
</header>
<?php if (!$is_front): ?>
<main role="main" class="container mrgn-tp-lg">
  <div class="row">
<?php endif; ?>
    <section<?php print $content_column_class; ?>>
      <?php if (empty($panels_layout)): ?>
        <?php if (!empty($page['highlighted'])): ?>
          <?php print render($page['highlighted']); ?>
        <?php endif; ?>
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if (!empty($title)): ?>
          <h1 class="page-header" id="wb-cont"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if (!empty($tabs)): ?>
          <?php print render($tabs); ?>
        <?php endif; ?>
        <?php if (!empty($page['help'])): ?>
          <?php print render($page['help']); ?>
        <?php endif; ?>
        <?php if (!empty($action_links)): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>
      <?php endif; ?>
      <?php if (!empty($messages)): ?>
        <?php print render($messages); ?>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </section>
<?php if (!$is_front): ?>
  </div>
</main>
<?php endif; ?>
<?php if (!empty($gcweb_cdn) && empty($gcweb_election)): ?>
  <aside class="gc-nttvs container">
    <h2><?php print t('Government of Canada activities and initiatives') ?></h2>
    <div id="gcwb_prts" class="wb-eqht row" data-ajax-replace="//cdn.canada.ca/gcweb-cdn-live/features/features-<?php print $language; ?>.html">
      <p class="mrgn-lft-md">
        <a href="http://www.canada.ca/<?php ($language == 'en') ? 'activities' : 'activites' ?>.html"><?php print t('Access Government of Canada activities and initiatives') ?></a>
     </p>
    </div>
  </aside>
<?php else: ?>
  <?php if (!empty($page['featured'])): ?>
    <aside class="gc-nttvs container">
      <?php print render($page['featured']); ?>
    </aside>
  <?php endif; ?>
<?php endif; ?>
<footer role="contentinfo" id="wb-info" class="visible-sm visible-md visible-lg wb-navcurr">
  <div class="container">
    <nav role="navigation" class="row">
    <h2><?php print t('About this site'); ?></h2>
    <?php print render($page['footer']); ?>
    <div class="col-sm-3 col-lg-3 brdr-lft">
      <section>
        <h3><?php print t('Feedback'); ?></h3>
        <p><a href="<?php print $gcweb['feedback'][$language]; ?>" class="gl-footer"><img src="<?php print $library_path; ?>/assets/feedback.png" alt="<?php print t('Feedback about this Web site'); ?>"></a></p>
      </section>
      <section>
        <h3><?php print t('Social Media'); ?></h3>
        <p><a href="<?php print $gcweb['social'][$language]; ?>" class="gl-footer"><img src="<?php print $library_path; ?>/assets/social.png" alt="<?php print t('Social Media'); ?>"></a></p>
      </section>
      <section>
        <h3><?php print t('Mobile Centre'); ?></h3>
        <p><a href="<?php print $gcweb['mobile'][$language]; ?>" class="gl-footer"><img src="<?php print $library_path; ?>/assets/mobile.png" alt="<?php print t('Mobile centre'); ?>"></a></p>
      </section>
    </div>
    </nav>
  </div>
  <div class="brand">
    <div class="container">
      <div class="row ">
        <div class="col-xs-6 visible-sm visible-xs tofpg">
          <a href="#wb-cont"><?php print t('Top of Page'); ?><span class="glyphicon glyphicon-chevron-up"></span></a>
        </div>
        <div class="col-xs-6 col-md-12 text-right">
          <?php if ($logo && $logo_bottom_svg): ?>
            <object data='<?php print $logo_bottom_svg; ?>' role="img" tabindex="-1" type="image/svg+xml">
              <img alt="<?php print t('WxT Logo'); ?>" src="<?php print $logo_bottom_svg; ?>"  />
            </object>
          <?php elseif ($logo): ?>
            <img alt="<?php print t('WxT Logo'); ?>" src="<?php print $logo_bottom; ?>"  />
          <?php endif; ?>
        </div>
      </div>
    </div>
  </div>
</footer>
