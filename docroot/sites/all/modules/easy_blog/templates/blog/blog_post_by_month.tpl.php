<?php
/**
 * @file
 * blog_post_by_month.tpl.php
 * theme archive blog
 */
?>
<div class="easy-blog-archive">
  <?php if (isset($archive) && is_array($archive)) : ?>
    <ul class="easy-blog-archive-list">
      <?php foreach ($archive as $year => $months): ?>
        <?php $month_first = reset($months); ?>
        <li>
          <?php
          $class = "";
          if (current_path() == 'blog/all/' . $year) {
            $class = "active";
          }
          ?>
          <?php if (!empty($months)) : ?>
            <span class="btn-arrow <?php print $class; ?>"></span>
          <?php endif; ?>
          <a href="<?php print url('blog/all/' . $year); ?>" class="link-dropdown <?php print $class; ?>"><?php print $year; ?> <span
              class="number">(<?php print $month_first->year_count; ?>)</span></a>
          <ul class="<?php print $class; ?>">
            <?php foreach ($months as $month_name => $data): ?>
              <li>
                <?php
                $class = "";
                if (current_path() == 'blog/all/' . $data->year . '-' . $data->month_digits) {
                  $class = "active";
                }
                ?>
                <?php if (isset($data->nodes) && is_array($data->nodes)) : ?>
                  <span class="btn-arrow <?php print $class; ?>"></span>
                <?php endif; ?>
                <a href="<?php print url('blog/all/' . $data->year . '-' . $data->month_digits);?>"  class="link-dropdown <?php print $class; ?>"><?php print $month_name; ?>
                  <span class="number">(<?php print $data->count; ?>)</span>
                </a>

                <?php if (isset($data->nodes) && is_array($data->nodes)): ?>
                  <ul class="<?php print $class; ?>">
                    <?php foreach ($data->nodes as $nid => $title): ?>
                      <li><?php print l($title, 'node/' . $nid); ?></li>
                    <?php endforeach; ?>
                  </ul>
                <?php endif; ?>
              </li>
            <?php endforeach; ?>
          </ul>
        </li>
      <?php endforeach; ?>
    </ul>
  <?php endif; ?>
</div>