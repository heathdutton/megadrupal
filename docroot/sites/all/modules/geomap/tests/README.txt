<h2>The following are manual tests (Please help out with automating tests):</h2>
<ol>
<li>Turn on the Geomap module</li>
<li>Add the block to the content region </li>
<li>Rendered HTML specific tests: Add the following fixed HTML to a node of type Page. View the page to see if everything is ok</li>
</ol>
<hr /><br /><br />
<ol>
<li>
<h2>Minimum example - google drop at point - no window - raw HTML printed</h2>
<div class="geo" title="Canterbury">
  Canterbury, United Kingdom
  <span class="latitude" title="51.2667"/>
  <span class="longitude" title="1.08333"/>
</div>
<hr />
<h1>Adding map div</h1>
<div class="map" id="map"><div id="map-loader-image">&nbsp;</div>Map here</div>
<hr /><br /><br />
</li>
<li>
<h2>Minimum - google drop at point - no window - HTML rendered using theme('geomap_geolocations') - Note: Input Format must be PHP to work</h2>
<?php
  $geolocations = array(
    array(
      'title' => "Großglockner, Austria", 
      'latitude' => 47.07388888888889,
      'longitude' => 12.694722222222222,
    ),
  );
?>
<h1>This is the array for Großglockner, Austria: </h1><pre><?php print_r($geolocations[0]); ?></pre>
<h1>This is what theme_geomap_geolocations returns:</h1><pre><?php print htmlspecialchars(theme('geomap_geolocations', $geolocations)); ?></pre>
<?php print theme('geomap_geolocations', $geolocations); ?>
<hr />
<h1>Adding map div</h1>
<div class="map" id="map"><div id="map-loader-image">&nbsp;</div>Map here</div>
<hr />
</li>
<li>
<h2>maximum - custom image, info-window, link - raw HTML printed</h2>
<div class="geo" title="Mount Everest">
  Mount Everest, India
  <span class="latitude" title="27.988055555555555" >N 27� 59' 17''</span>
  <span class="longitude" title="86.92527777777778" >E 86� 55' 31''</span>
  <div class="marker" >
    <div class="infowindow">
      <div class="infowindow-text"><img src="http://upload.wikimedia.org/wikipedia/commons/thumb/4/4b/Everest_kalapatthar_crop.jpg/280px-Everest_kalapatthar_crop.jpg" width="60px" height="40px" /><h4>Mount Everest</h4></div>
    </div>
    <div class="icon" src="http://www.quax.at/sites/all/themes/quax3/images/google/google-button-dubisthier1.png" >
      <div class="option" name="shadow" value="http://www.quax.at/sites/all/themes/quax3/images/google/google-button-dubisthier1-shadow.png" ></div>
      <div class="option" name="transparent" value="http://www.quax.at/sites/all/themes/quax3/images/google/google-button-dubisthier1-transparent.png" ></div>
      <div class="option" name="iconwidth" value="30" ></div>
      <div class="option" name="iconheight" value="30" ></div>
    </div>
  </div>
  <div class="node" nid="1" link="http://en.wikipedia.org/wiki/Everest" />
</div>
<hr />
<h1>Adding map div</h1>
<div class="map" id="map"><div id="map-loader-image">&nbsp;</div>Map here</div>
<hr />
</li>
<li>
<h2>maximum - custom image, info-window, link to node - HTML rendered using theme('geomap_geolocations')</h2>
<?php
  $geolocations = array(
    array(
      'title' => "Mount St. Helens, WA, US", 
      'latitude' => 46.199444444444445,
      'longitude' => -122.19111111111111,
      'nid' => $node->nid,
      'type' => $node->type,
      'path' => 'http://de.wikipedia.org/wiki/Mount_St._Helens', //linking back to this node would be boring! link to something interesting
      'marker' => array(
        'icon' => array(
          //in this test we are overriding the icon - hence all database settings will be ignored
          'src' => "http://www.quax.at/sites/all/themes/quax3/images/google/google-button-dubisthier1.png",
          'icon_classes' => NULL,
          'icon_id' => NULL,
          'icon_options' => array(
            array(
              'option_classes' => NULL,
              'option_name' => "shadow",
              'option_value' => "http://www.quax.at/sites/all/themes/quax3/images/google/google-button-dubisthier1-shadow.png",
            ),
            array(
              'option_classes' => NULL,
              'option_name' => "transparent",
              'option_value' => "http://www.quax.at/sites/all/themes/quax3/images/google/google-button-dubisthier1-transparent.png",
            ),
            array(
              'option_classes' => NULL,
              'option_name' => "iconwidth",
              'option_value' => "30",
            ),
            array(
              'option_classes' => NULL,
              'option_name' => "iconheight",
              'option_value' => "30",
            ),
            array(
              'option_classes' => NULL,
              'option_name' => "iconanchor_x",
              'option_value' => "3",
            ),
            array(
              'option_classes' => NULL,
              'option_name' => "iconanchor_y",
              'option_value' => "30",
            ),
          ),
        ),
        'infowindow' => array(
          'windowtext' => array(
            'text' => "Text for Mount St. Helens, WA, US",
            'text_classes' => NULL,
            'text_id' => NULL,
          ),
        ),
      ),
    ),
  );
  print theme('geomap_geolocations', $geolocations);
?>
<hr /><br /><br />
</li>
