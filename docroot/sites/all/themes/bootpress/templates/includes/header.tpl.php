<?php
/**
 * header template
 * Boot Press
 * @author Pitabas Behera
 *
 **/
?>

<div id="header-top" class="clearfix">
  <div class="container">
    <div class="row">
      <div class="col-lg-7 col-md-7">
        <?php
        if(theme_get_setting('header_message')) {
          print filter_xss_admin(theme_get_setting('header_message'));
        }
        ?>
      </div>
      <div class="col-lg-5 col-md-5 text-right">
        <?php if (!empty($secondary_nav)): ?>
          <?php print render($secondary_nav); ?>
        <?php endif; ?>
      </div>
    </div>
  </div>
</div>

<header id="header" itemscope="itemscope">
  <div class="container">
    <div class="row">
      <div class="col-lg-12 col-md-12 col-sm-12">
        <!-- Website Logo -->
        <?php if ($logo):?>
          <div id="logo" class="logo clearfix">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /> </a>
          </div>
        <?php endif; ?>

        <?php if ($site_name):?>
          <div id="site-name">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
          </div>
        <?php endif; ?>
          
        <?php if ($site_slogan):?>
          <div id="site-slogan">
          <?php print $site_slogan; ?>
          </div>
        <?php endif; ?>
        
        <!-- Main Navigation -->
        <nav class="main-menu">
        </nav>
        <div id="responsive-menu-container"></div>
      </div>
    </div>
  </div>
</header>

  <div id="main-navigation" class="clearfix navbar">
    <div class="navbar-header">
      <div class="container">
        <div class="main-navigation-inner">
          <!-- This below site title will display on the mobile size screen device -->
          <?php if ($site_name):?>
            <div class="mobile-version-site-name pull-left">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
            </div>
          <?php endif; ?>
          <!-- .btn-navbar is used as the toggle for collapsed navbar content -->
          <button class="navbar-toggle" type="button" data-toggle="collapse" data-target=".bs-navbar-collapse">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <?php if (!empty($primary_nav) || !empty($page['navigation'])): ?>
            <nav class="collapse navbar-collapse bs-navbar-collapse" role="navigation">
              <?php if (!empty($primary_nav)): ?>
                <?php print render($primary_nav); ?>
              <?php elseif(!empty($page['navigation'])): ?>
                <?php print render($page['navigation']); ?>
              <?php endif; ?>
            </nav>
          <?php endif; ?>
        </div>
      </div>
    </div>
  </div>

<?php if ($page['banner']) : ?>
  <div id="banner" class="clearfix" role="banner">
    <?php print render($page['banner']); ?>
  </div>
<?php endif; ?>

<?php if ($breadcrumb && theme_get_setting('breadcrumb_display')):?> 
  <div id="breadcrumb" class="page-top clearfix">
    <div id="breadcrumb-inside" class="clearfix">
      <div class="container">
        <div class="row">
          <div class="col-lg-12 col-md-12 col-sm-12">
          <?php if( !empty($node) && $node->type == 'page'): ?>
            <?php print render($title_prefix); ?>
            <?php if ($title):?>
            <h1 class="page-title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
          <?php endif; ?>
            <?php print $breadcrumb; ?>
          </div>
        </div>
      </div>
    </div>
  </div>
<?php endif; ?>