<div role="document" class="page">
  <?php if (!empty($page['header'])): ?>
    <header id="site-header">
      <div class="outer-wrapper">
        <?php print render($page['header']); ?>
      </div>
    </header>
  <?php endif; ?>

  <?php if (!empty($page['featured'])): ?>
    <section id="featured">
      <div class="outer-wrapper">
        <?php print render($page['featured']); ?>
      </div>
    </section>
  <?php endif; ?>

  <?php if ($messages): ?>
    <section id="messages">
      <div class="outer-wrapper">
        <?php print $messages; ?>
      </div>
    </section>
  <?php endif; ?>

  <?php if ($breadcrumb): ?>
    <section id="breadcrumb">
      <div class="outer-wrapper">
        <?php print $breadcrumb; ?>
      </div>
    </section>
  <?php endif; ?>

  <main role="main" class="outer-wrapper">
    <?php if (!empty($page['sidebar_first'])): ?>
      <aside id="sidebar-first" role="complementary" class="sidebar">
        <?php print render($page['sidebar_first']); ?>
      </aside>
    <?php endif; ?>

    <section id="content">
      <?php if ($title): ?>
        <?php print render($title_prefix); ?>
          <h1 id="page-title"><?php print $title; ?></h1>
        <?php print render($title_suffix); ?>
      <?php endif; ?>

      <?php if (!empty($tabs)): ?>
        <?php print render($tabs); ?>
        <?php if (!empty($tabs2)): print render($tabs2); endif; ?>
      <?php endif; ?>

      <?php if ($action_links): ?>
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>

      <?php print render($page['content']); ?>
    </section>

    <?php if (!empty($page['sidebar_second'])): ?>
      <aside id="sidebar-second" role="complementary" class="sidebar">
        <?php print render($page['sidebar_second']); ?>
      </aside>
    <?php endif; ?>
  </main>

  <?php if (!empty($page['footer_top'])): ?>
    <footer id="site-footer" role="contentinfo">
      <?php if (!empty($page['footer_top'])): ?>
        <section class="footer-top">
          <?php print render($page['footer_top']); ?>
        </section>
      <?php endif; ?>
    </footer>
  <?php endif; ?>
</div>