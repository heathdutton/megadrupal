<?php $has_m_content_left = isset($page['m_content_left']) && count($page['m_content_left']); ?>
<?php $has_m_footer_left = isset($page['m_footer_left']) && count($page['m_footer_left']); ?>
<div class="page">

  <div class="head-part page-part page-part-line page-part-dark">
    <div class="pp-data clearfix">
      <?php print render($page['m_admin_area']); ?>
      <?php print render($page['m_user_menu']); ?>
      <?php if ($logo) { ?>
        <a id="logo" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php } ?>
      <?php print render($page['m_header']); ?>
    </div>
  </div>

  <div class="body-part page-part">
    <div class="pp-data clearfix<?php if ($has_m_content_left) print ' pp-data-with-left-column'; ?>">
      <?php if ($has_m_content_left) { ?>
        <div class="column-l">
          <div class="column-data">
            <?php print render($page['m_content_left']); ?>
          </div>
        </div>
      <?php } ?>
      <div class="column-m">
        <div class="column-data">
          <?php print $breadcrumb; ?>
          <a id="main-content"></a>
          <?php print render($title_prefix); ?>
          <?php if ($title) { ?><h1 class="page-title"><?php print $title; ?></h1><?php } ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs) { ?>
            <div class="tabs">
              <?php print render($tabs); ?>
            </div>
          <?php } ?>
          <?php print $messages; ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links) { ?><ul class="action-links"><?php print render($action_links); ?></ul><?php } ?>
          <?php print render($page['content']); ?>
        </div>
      </div>
    </div>
  </div>

  <div class="foot-part page-part page-part-line page-part-dark">
    <div class="pp-data clearfix<?php if ($has_m_footer_left) print ' pp-data-with-left-column'; ?>">
      <?php if ($has_m_footer_left) { ?>
        <div class="column-l">
          <div class="column-data">
            <?php print render($page['m_footer_left']); ?>
          </div>
        </div>
      <?php } ?>
      <div class="column-m">
        <div class="column-data">
          <?php print render($page['m_footer']); ?>
        </div>
      </div>
    </div>
  </div>

  <div class="tech-part-bottom">
    <?php print render($page['m_tech_bottom']); ?>
  </div>

</div>