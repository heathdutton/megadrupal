<?php foreach ($photos as $photo): ?>
  <?php

  $description = '';
  $location = '';

  if ($settings['showDescription'] && isset($photo['description'])) {
    $description = htmlentities($photo['description']);
  }

  if ($settings['showLocation'] && isset($photo['location'])) {
    $location = htmlentities($photo['location']);
  }

  ?>
  <div class="album-wrapper" data-name="<?php print $photo['name']; ?>" data-description="<?php print $description; ?>" data-location="<?php print $location; ?>">
  <div class="album-thumb fb-link" data-album-link="facebook_album/get/album/<?php print $photo['id']; ?>">
    <span class="album-thumb-wrapper">
      <i data-cover-id="<?php print $photo['id']; ?>" class="unloaded" style="width: <?php print $settings['albumThumbWidth'] . 'px'; ?>; height:<?php print $settings['albumThumbHeight'] . 'px'; ?>"></i>
    </span>
  </div>
  <div class="album-details">
    <div class="album-text" style="max-width: <?php print $settings['albumThumbWidth'] . 'px'; ?>">
      <div class="fb-link" href="#album-<?php print $photo['id']; ?>"><?php print $photo['name']; ?></div>
    </div>
  </div>
</div>
<?php endforeach; ?>