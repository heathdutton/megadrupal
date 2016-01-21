<p>The basic configuration and design of the A/B tests is made by logging into your account on the <a href="http://optimize.ly/OZRdc0" target="_NEW">Optimizely website.</a></p>

<?php if (($variables['form']['optimizely_project_code']['#default_value'] == 0) && ($variables['form']['optimizely_oid']['#value'] == 1)): ?>

  <p>In order to use this module, you'll need an <a href="http://optimize.ly/OZRdc0">Optimizely account</a>. A Free 30 day trial account is available.</p>

  <ul>
    <li><strong>Add the account ID to the <a href="/admin/config/system/optimizely/settings">Account Info</a> settings page to be able to enable this
    entry</strong>.</li>
  </ul>
  
  <p>The default Project javascript (js) file (snippet) uses the Optimizely account ID for it's file name. The "Default" / first project entry in the
  <a href="/admin/config/system/optimizely">Project Listing</a> page uses the account ID value for the Project Code setting. The Default project entry targets all pages site wide with it's initial settings. Enabling this entry will result in the initial Optimizely experiments running site wide.</p>

<?php endif; ?>

<?php echo drupal_render_children($form) ?>