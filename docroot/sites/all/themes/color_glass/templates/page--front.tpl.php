<?php
/**
 * @file
 * Front page template.
 */
?>

<header id="navbar" role="banner" class="navbar navbar-fixed-top navbar-default">
  <div class="container">
    <div class="navbar-header">
      <?php if ($logo): ?>
        <a class="logo navbar-btn pull-left" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>

      <?php if (!empty($site_name)): ?>
        <a class="name navbar-brand" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
      <?php endif; ?>

      <?php if (!empty($primary_nav) || !empty($secondary_nav) || !empty($page['navigation'])): ?>
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
          <span class="sr-only"><?php print t('Toggle navigation'); ?></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
      <?php endif; ?>
    </div>

    <?php if (!empty($primary_nav) || !empty($secondary_nav) || !empty($page['navigation']) || !empty($search_box)): ?>
      <div class="navbar-collapse collapse pull-right">
        <nav role="navigation">
          <?php if (!empty($primary_nav)): ?>
            <?php print render($primary_nav); ?>
          <?php endif; ?>
          <?php if (!empty($secondary_nav)): ?>
            <?php print render($secondary_nav); ?>
          <?php endif; ?>
          <?php if (!empty($page['navigation'])): ?>
            <?php print render($page['navigation']); ?>
          <?php endif; ?>
          <?php if (!empty($search_box)): ?>
            <?php print render($search_box); ?>
          <?php endif; ?>   
        </nav>
      </div>
    <?php endif; ?>
  </div>
</header>
<?php
/* Print welcome block */
print _bootstrap_region_render($page['home_welcome'], 'home_welcome', 'section', 'home-welcome', 'home-welcome home-section'); ?>

<!-- Render the Content light green Region -->
<?php print _bootstrap_region_render($page['home_cta'], 'home_cta', 'div', 'home-cta', 'home-cta home-section'); ?>

<!-- Render the Content dark orange Region -->
<?php print _bootstrap_region_render($page['home_features'], 'home_features', 'div', 'home-features', 'home-features home-section'); ?>

<!-- Render the Content yellow Region -->
<?php print _bootstrap_region_render($page['home_bottom'], 'home_bottom', 'div', 'home-bottom', 'home-bottom home-section'); ?>

<footer id = "footer-section" class = "footer-section">
<?php
/* Print footer message block */
print _bootstrap_region_render($page['footer_message'], 'footer_message', 'div', 'footer-message', 'footer-message');
?>
<?php if (!empty($page['footer_block1']) || !empty($page['footer_block2']) || !empty($page['footer_block3']) || !empty($page['footer_social'])): ?>
<div class = "footer-blocks">
  <div class = "container">
  <div class = "row">
    <div class = "col-sm-3 footer-menu footer-menu-1">
       <?php print render($page['footer_block1']); ?>
    </div>
    <div class = "col-sm-3 footer-menu footer-menu-2">
       <?php print render($page['footer_block2']); ?>
    </div>
    <div class = "col-sm-3 footer-menu footer-menu-3">
       <?php print render($page['footer_block3']); ?>
    </div>
    <div class = "col-sm-3 footer-menu footer-social">
       <?php print render($page['footer_social']); ?>
    </div>    
  </div>
  </div>
</div>
<?php endif; ?>

<?php
/* Print copyright block */
print _bootstrap_region_render($page['footer_copyright'], 'footer_copyright', 'div', 'footer-copyright', 'footer-copyright text-center');
?>
</footer>
<?php if($theme_credits) : ?>
<div class = "theme-credit text-center clr-secondary-bg clr-link-color">
  <?php print $theme_credits; ?>
</div>
<?php endif; ?>
