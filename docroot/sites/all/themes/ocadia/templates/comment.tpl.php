<div class="comment">
<ol id="commentlist">
  <li class="comment<?php if ($comment->cid%2) echo 'alt'; ?>">
    <p class="commentauthor"><?php print $author ?> <?php print t("said"); ?>,</p>
    <p class="commentmeta"><?php print $date ?> - <?php print $links ?></p>
    <?php print $content ?>
  </li>
</ol>
</div>
