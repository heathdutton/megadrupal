<?php
/**
 * @file
 * Default theme implementation to display the Last.fm Now Playing block.
 *
 * Available variables:
 * - $track['error'] (string): Reason why no track information is returned. Can
 *   be a cURL or Last.fm/Libre.fm-related error. NULL if no errors.
 * - $track['site'] (string): The website that track information is retrieved
 *   from; either 'lastfm' or 'librefm'.
 * - $track['now_playing'] (boolean): TRUE if the track is currently being
 *   played. FALSE if not (i.e. it was the last track to be played).
 * - $track['name'] (string): The name of the track.
 * - $track['artist'] (string): The name of the track's artist.
 * - $track['album'] (string): The name of the album the track comes from.
 * - $track['url'] (string): The URL of the track on Last.fm.
 * - $track['image'] (array): An associative array of the track's album cover
 *   URLs. Array keys are the sizes ('small', 'medium', 'large', 'extralarge').
 * - $track['date'] (integer): The Unix timestamp of the date/time the track was
 *   last played ('0' if it's currently playing). Use Drupal's format_date()
 *   function to display it in an appropriate format.
 * - $track['user'] (string): The URL of the user's account on Last.fm/Libre.fm.
 */
?>

<?php if (!empty($track['error'])): ?>
  <!-- Error -->
  <p class="error">Error: <?php print $track['error']; ?></p>
<?php else: ?>

  <?php if (!$track['now_playing']): ?>
    <!-- Not currently playing -->
    <p class="not_playing">Not currently playing anything...</p>
  <?php else: ?>

    <!-- Display track information -->
    <!-- Album cover -->
    <?php if (!empty($track['album']) && !empty($track['image'])): ?>
      <?php if (module_exists('colorbox')): ?>
        <?php
          $variables = array(
            'image' => array(
              'path' => $track['image']['medium'],
              'style_name' => NULL,
              'alt' => "'" . $track['album'] . "' album cover",
              'title' => $track['album'],
            ),
            'path' => $track['image']['extralarge'],
            'title' => $track['album'],
          );
          $image = theme('colorbox_imagefield', $variables);
        ?>
      <?php else: ?>
        <?php
          $variables = array(
            'path' => $track['image']['medium'],
            'alt' => "'" . $track['album'] . "' album cover",
            'title' => $track['album'],
          );
          $image = theme('image', $variables);
        ?>
      <?php endif; ?>
      <div class="album-cover"><?php print $image; ?></div>
    <?php endif; ?>

    <!-- Track -->
    <?php
      if (!empty($track['url'])) {
        $name = l($track['name'], $track['url']);
      }
      else {
        $name = $track['name'];
      }
    ?>
    <h3 class="track"><?php print $name; ?></h3>

    <!-- Artist -->
    <?php if (!empty($track['artist'])): ?>
      <p class="artist">by <?php print $track['artist']; ?></p>
    <?php endif; ?>

    <!-- Album -->
    <?php if (!empty($track['album'])): ?>
      <p class="album">from <?php print $track['album']; ?></p>
    <?php endif; ?>

  <?php endif; ?>

  <!-- Link -->
  <?php if (!empty($track['user'])): ?>
    <?php
      if ($track['site'] == 'lastfm') {
        $text = t('Last.fm profile');
      }
      else {
        $text = t('Libre.fm profile');
      }
    ?>
    <p class="link"><?php print l($text, $track['user']); ?></p>
  <?php endif; ?>

<?php endif; ?>

