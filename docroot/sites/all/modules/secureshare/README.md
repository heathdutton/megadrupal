## Secureshare / Social Share Privacy

provides a implementation of the 2-click-solution from heise.de ( http://www.heise.de/ct/artikel/2-Klicks-fuer-mehr-Datenschutz-1333879.html )
for more privacy against Facebook, Twitter and Google+. This module implement the heise-jquery-code using libraries.

### Installation:

<ol>
  <li>Download the jquery-code from http://www.heise.de/extras/socialshareprivacy/ , extract it and put it in your libraries folder Example: sites/all/libraries/  > sites/all/libraries/secureshare</li>
<li>Go to admin/build/modules and enable secureshare and secureshare_block or secureshare_field or both.</li>
<li>Got to the settings (block or content type) and select an profile</li>
</ol>

<h3>Configuration:</h3>

<h4>Profiles:</h4>
You can create your own settings profile under Configuration > content authoring SecureShare profiles ("admin/config/content/secureshare").
<h4>Submodule: secureshare_blocks</h4>
The submodule provides a simple block you can select the profile in the normal block settings

<h4>Submodule: secureshare_fields</h4>
This submodule provides a simple element field implementation for entities. Currently only node bundles are activated. You can select the profile in the content type settings and under the display view settings you can change the visibility and position.

<h3>Known problems:</h3>

the css file in the heise library seems broken some stylings. Its not part of this module to fix these things.
But you can use hook_css_alter or theme to replace them. Or you can override the failures in your theme css. Feel you free to send an issue to heise.de to fix the css code ;)

<h3>Developers:</h3>

The core module uses hook_element_info() so you can use in your module like other drupal render elements.

<ul>
  <li>#type need to be set to "secureshare"</li>
  <li>#secureshare_profile is the machine name of the secureshare settings profile. Example: "default"</li>
  <li>#secureshare_config (optional) is a array with the heise plugin settings. Options will be merged over the profile settings.</li>
</ul>

<h4>Example (profile usage / minimal implementation):</h4>
<?php
  $element['myelement'] = array(
    '#type' => 'secureshare',
    '#secureshare_profile' => 'default',
  );
?>

<h4>Example (profile usage / configuration overwrite):</h4>
#secureshare_config will be merged over the profile settings.
<?php
  $element['myelement'] = array(
    '#type' => 'secureshare',
    '#secureshare_profile' => 'default',
    '#secureshare_config' => array(
      'css_path' => '/my/path/to/css/file.css',
    ),
  );
?>

<h4>Example (configuration alter):</h4>
You can also alter the configuration settings before used in pre render by.
<?php
hook_secureshare_config_alter(&$config, $profile = 'default'){}
?>
<h3>Drupal 6</h3>
We don't create a Drupal 6 version of this module but back port patches are welcome.

<h3>Similar modules</h3>
You can see in the Issues that we had a duplicate module problem but is unfortunately not yet clarified how it goes next. This module here has a Sandbox project before the other project was released so we got a simple time overlapping problem.

<h3>Disclaimer</h3>
The original Javascript Plugin was created under the MIT License by: Heise Zeitschriften Verlag GmbH & Co. KG and can be downloaded under: http://www.heise.de/extras/socialshareprivacy/

This drupal module was sponsored by:
<a href="http://www.projektwaterkant.de">ProjektWaterkant UG</a>
<a href="http://team-wd.de">TWD - team:Werbedesign UG</a>
