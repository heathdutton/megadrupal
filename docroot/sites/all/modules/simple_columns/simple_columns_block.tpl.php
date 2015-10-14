<?php foreach ($block_items as $key => $block_item) : ?>
  <div class="col grid-2 simple-columns-block-<?php echo $key; ?>">
    <h2><?php echo $block_item['title']; ?></h2>
    <?php if (isset($block_item['body']['value']) && !empty($block_item['body']['value'])) : ?>
      <?php echo $block_item['body']['value']; ?>
    <?php endif; ?>
    <ul>
      <?php if (!empty($block_item['links'])) : ?>
        <?php $block_item['links'][LANGUAGE_NONE] = array_reverse($block_item['links'][LANGUAGE_NONE]); ?>
        <?php foreach ($block_item['links'][LANGUAGE_NONE] as $row) : ?>
          <?php if(is_array($row) && !empty($row['url'])) : ?>
            <li><?php echo l((empty($row['title']) ? $row['url'] : $row['title']), $row['url']); ?></li>
          <?php endif; ?>
        <?php endforeach; ?>
      <?php endif; ?>
    </ul>
  </div>
<?php endforeach; ?>
