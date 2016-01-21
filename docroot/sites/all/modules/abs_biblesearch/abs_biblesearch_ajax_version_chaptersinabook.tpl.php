<div class="abs-biblesearch-chaptersinabook">
  <p><strong>Now pick a chapter:</strong></p>
  <?php
  foreach ($chapters as $chapter) {
    echo '<a class="book" href="?viewid=' . $chapter->id . '">'. $chapter->chapter . '</a>';
  }
  ?>
</div>