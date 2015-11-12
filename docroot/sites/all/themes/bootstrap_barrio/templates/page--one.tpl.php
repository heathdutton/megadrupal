<?php

/**
 * @file
 * Override of Bootstrap page.tpl.php.
 */
?>
<?php if (!empty($secondary_nav) || !empty($page['top'])): ?>
<header role="banner">
  <div id="top" class="container">
    <?php print render($secondary_nav); ?>
    <?php print render($page['top']); ?>
  </div>
</header>
<?php endif; ?>
<header id="navbar" class="<?php print $navbar_classes; ?>" role="banner">
  <div class="navbar-inner">
     <div id="main-navs">
      <div class="container">
       <div class="navbar-header">
        <?php if (!empty($logo)): ?>
          <a class="logo pull-left" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        <?php endif; ?>

        <?php if (!empty($site_name)): ?>
          <h1 id="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" class="brand"><?php print $site_name; ?></a>
          </h1>
        <?php endif; ?>
        <!-- .btn-navbar is used as the toggle for collapsed navbar content -->
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#main-nav">
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
       </div>
      
       <div id="header" class="header">
         <div>
          <?php print render($page['header']); ?>
          <?php if (!empty($page['yamm'])): ?>
            <div class="navbar yamm">
              <div id="navyamm" class="nav-collapse">
                <ul class="nav navbar-nav">
                  <?php print render($page['yamm']); ?>
                </ul>
              </div>
            </div>
          <?php endif; ?>
          <?php if (!empty($primary_nav) || !empty($page['navigation'])): ?>
            <div id="main-nav" class="<?php print $collapse; ?>">
              <nav role="navigation">
                <?php if (!empty($primary_nav)): ?>
                  <?php print render($primary_nav); ?>
                <?php endif; ?>
                <?php if (!empty($page['navigation'])): ?>
                  <?php print render($page['navigation']); ?>
                <?php endif; ?>
              </nav>
            </div>
          <?php endif; ?>
        </div>
      </div></div>
    </div>
  </div>
</header>

<?php if (!empty($page['slider'])): ?>
  <div id="slideshow_container">
    <?php print render($page['slider']); ?>
  </div>
<?php endif; ?>

<div class="main-container">

  <header role="banner" id="page-header">
    <?php if (!empty($site_slogan)): ?>
      <p class="lead"><?php print $site_slogan; ?></p>
    <?php endif; ?>
  </header> <!-- /#header -->

    <?php if (!empty($page['preface'])): ?>
      <div id="preface" class="page">
    <div class="container">
        <?php print render($page['preface']); ?>
      </div>  <!-- /#preface -->
    </div>
    <?php endif; ?>  
<div class="container">
      <a id="main-content"></a>
      <?php print $messages; ?>
      <?php if (!empty($tabs)): ?>
        <?php print render($tabs); ?>
      <?php endif; ?>
      <?php if (!empty($page['help'])): ?>
        <div class="well"><?php print render($page['help']); ?></div>
      <?php endif; ?>
      <?php if (!empty($action_links)): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
</div>      
      <?php if (!empty($page['content_top'])): ?>
      <div id="page-top" class="page">
    <div class="container">
        <?php print render($page['content_top']); ?>
      </div>  <!-- /#sidebar-second -->
    </div>
      <?php endif; ?>
      
      <?php if ($print_content): ?>
        <div id="content-inner" class="page">
    <div class="container">
          <?php print render($page['content']); ?>
        </div>
    </div>
     <?php else: ?>
        <?php
          render($pagemetatag);
        ?>
      <?php endif; ?>
      
      <?php if (!empty($page['content_bottom'])): ?>
      <div id="page-bottom" class="page">
    <div class="container">
        <?php print render($page['content_bottom']); ?>
      </div>  <!-- /#sidebar-second -->
    </div>
      <?php endif; ?>

    <?php if (!empty($page['postcript_top'])): ?>
      <div id="postcript_top" class="page">
    <div class="container">
        <?php print render($page['postcript_top']); ?>
      </div>  <!-- /#preface -->
    </div>
    <?php endif; ?>  

    <?php if (!empty($page['postcript_bottom'])): ?>
      <div id="postcript_bottom" class="page">
    <div class="container">
        <?php print render($page['postcript_bottom']); ?>
      </div>  <!-- /#preface -->
    </div>
    <?php endif; ?>  

</div>
<footer class="footer">
    <div id="footer-inner" class="container">
    <?php print render($page['footer']); ?>
    </div>
  </footer>
