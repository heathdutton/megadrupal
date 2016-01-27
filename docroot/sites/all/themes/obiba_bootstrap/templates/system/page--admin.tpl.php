<?php
/**
 * @file
 * Code Admin layout.
 */

?>

<div class="main-container container">

  <header role="banner" id="page-header">
    <?php if (!empty($site_slogan)): ?>
      <p class="lead"><?php print $site_slogan; ?></p>
    <?php endif; ?>

    <?php print render($page['header']); ?>

  </header>
  <!-- /#page-header -->

  <div class="row">
    <?php if (!empty($breadcrumb)):  print $breadcrumb; endif; ?>
    <?php if (!empty($title)): ?>
      <h1 class="page-header">
        <?php if (!empty($classes_array['title_page'])) : ?>
          <span
            class="t_badge color_<?php print $classes_array['title_page']; ?> i-obiba-<?php print $classes_array['title_page']; ?>">
          </span>
        <?php endif; ?>
        <?php print $title; ?>
      </h1>
    <?php endif; ?>
    <?php if (!empty($page['obiba_help'])): ?>
      <div class="row ">
        <div class="help-info-box alert alert-info alert-dismissible"
          role="alert">
          <button type="button" class="close" data-dismiss="alert"
            aria-label="Close"><span aria-hidden="true">&times;</span></button>
          <?php print render($page['obiba_help']); ?>
        </div>
      </div>
    <?php endif; ?>
    <?php if (!empty($page['sidebar_first']) || !empty($page['facets'])): ?>
      <aside class="col-sm-4 col-lg-3" role="complementary">
        <?php if (!empty($page['facets'])): ?>
          <?php print render($page['facets']); ?>
        <?php endif; ?>
        <?php if (!empty($page['sidebar_first'])) : ?>
          <?php print render($page['sidebar_first']); ?>
        <?php endif; ?>
      </aside>  <!-- /#sidebar-first -->
    <?php endif; ?>

    <section<?php print $content_column_class; ?>>
      <?php if (!empty($page['highlighted'])): ?>
        <div
          class="highlighted jumbotron"><?php print render($page['highlighted']); ?></div>
      <?php endif; ?>

      <a id="main-content"></a>
      <?php print render($title_prefix); ?>

      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <?php if (!empty($tabs)): ?>
        <?php print render($tabs); ?>
      <?php endif; ?>
      <?php if (!empty($page['help'])): ?>
        <?php print render($page['help']); ?>
      <?php endif; ?>
      <?php if (!empty($action_links)): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </section>

    <?php if (!empty($page['sidebar_second'])): ?>
      <aside class="col-sm-3" role="complementary">
        <?php print render($page['sidebar_second']); ?>
      </aside>  <!-- /#sidebar-second -->
    <?php endif; ?>
    <!-- Sidebar -->

    <!-- Fixed side -->
    <div id="fixed-sidebar">
      <div id="sidebar-wrapper"
        class="side-bar-content sidebar-untoggled"></div>
    </div>
    <!-- /#Fixed side -->

    <!-- /#sidebar-wrapper -->
  </div>
</div>
<footer class="footer container">
  <?php print render($page['footer']); ?>
</footer>
