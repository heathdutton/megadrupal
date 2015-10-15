<div role="document" class="page">
  <header role="banner" class=""></header>


  <main role="main" class="grid body">
      <a id="main-content"></a>

      <?php if ($breadcrumb): print $breadcrumb; endif; ?>

      <?php if ($title && !$is_front): ?>
        <?php print render($title_prefix); ?>
        <h1 id="page-title" class="title"><?php print $title; ?></h1>
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

      <div class="button-group">
        <button class="button" type="button">Left</button>
        <button class="button" type="button">Middle</button>
        <button class="button" type="button">Right</button>
      </div>

      <ul class="button-group">
        <li><button class="button" type="button">Left</button></li>
        <li><button class="button" type="button">Middle</button></li>
        <li><button class="button" type="button">Right</button></li>
      </ul>
  </main>
  <!--/.l-main-->

  <footer></footer>
</div>
<!--/.page -->
