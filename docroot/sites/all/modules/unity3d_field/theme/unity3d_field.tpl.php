<?php 
/**
 * @file
 * The Unity 3D Web Player object themed for page viewing.
 *
 * Using JS calls, this shows the .unity3d file as a web player.
 */


// For each field entry, display the file in a web player.
foreach ($items as $item) :
  // Show the placeholder image if the viewer doesn't have the web player
  // or JavaScript installed.
  ?>
<div id="<?php echo $item['player_id'] ?>">
  <div class="missing">
    <a href="http://unity3d.com/webplayer/" title="Unity Web Player. Install now!">
      <img alt="Unity Web Player. Install now!" src="http://webplayer.unity3d.com/installation/getunity.png" width="193" height="63" />
    </a>
  </div>
</div>
<?php endforeach; ?>
