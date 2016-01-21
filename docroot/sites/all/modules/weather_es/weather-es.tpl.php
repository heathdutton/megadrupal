<div class="weather_es" rel="<?php print $page; ?>">
  <ul>
    <?php foreach($obj as $data => $info): ?>
      <?php if ($data == 'date'): ?>
        <p><strong>Day: <?php print $info[0]; ?></strong></p>
      <?php endif; ?>
      <?php if ($data == 'sky'): ?>
        <?php foreach($info as $periode => $value): ?>
          <?php foreach($value as $txt => $img): ?>
            <li>Sky conditions <?php print $periode; ?>:<div style="text-align: left;"><?php print $img; ?></div></li>
          <?php endforeach; ?>
        <?php endforeach; ?>
      <?php endif; ?>
      <?php if ($data == 'rain'): ?>
        <?php foreach($info as $periode => $value): ?>
          <li>Rainfall <?php print $periode; ?>: <?php print $value; ?> %</li>
        <?php endforeach; ?>
      <?php endif; ?>
      <?php if ($data == 'temp'): ?>
        <?php foreach($info as $periode => $value): ?>
          <li>Temperature <?php print $periode; ?>: <?php print $value; ?> ºC</li>
        <?php endforeach; ?>
      <?php endif; ?>
      <?php if ($data == 'chill'): ?>
        <?php foreach($info as $periode => $value): ?>
          <li>Chill <?php print $periode; ?>: <?php print $value; ?> ºC</li>
        <?php endforeach; ?>
      <?php endif; ?>
      <?php if ($data == 'snow'): ?>
        <?php foreach($info as $periode => $value): ?>
          <li>Snow level <?php print $periode; ?>: <?php print $value; ?> m</li>
        <?php endforeach; ?>
      <?php endif; ?>
      <?php if ($data == 'wind'): ?>
        <?php foreach($info as $periode => $sd): ?>
          <?php foreach($sd as $dir => $speed): ?>
            <li>Wind <?php print $periode .': '. $dir; ?> <?php print $speed; ?> km/h</li>
          <?php endforeach; ?>
        <?php endforeach; ?>
      <?php endif; ?>
      <?php if ($data == 'gust'): ?>
        <?php foreach($info as $periode => $value): ?>
          <li>Gust of wind <?php print $periode; ?>: <?php print $value; ?> km/h</li>
        <?php endforeach; ?>
      <?php endif; ?>
      <?php if ($data == 'hum'): ?>
        <?php foreach($info as $periode => $value): ?>
          <li>Humidity <?php print $periode; ?>: <?php print $value; ?> %</li>
        <?php endforeach; ?>
      <?php endif; ?>
      <?php if ($data == 'uv'): ?>
        <li>UV: <?php print $info[0]; ?></li>
      <?php endif; ?>
    <?php endforeach; ?>
  </ul>
</div>