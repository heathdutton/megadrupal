<?php
/**
 * @file
 * Contains admin help documentation.
 */

/**
 * Implements hook_help().
 */
function sclib_enkoder_help($path, $arg) {
  if ($path == 'admin/help#sclib_enkoder') {
    drupal_add_css('http://alexgorbatchev.com/pub/sh/current/styles/shThemeEclipse.css',
      array('type' => 'external'));
    drupal_add_js('http://alexgorbatchev.com/pub/sh/current/scripts/shCore.js',
      array('type' => 'external'));
    drupal_add_js('http://alexgorbatchev.com/pub/sh/current/scripts/shAutoloader.js',
      array('type' => 'external'));

    drupal_add_css(drupal_get_path('module', 'sclib_enkoder') . '/shStyle.css');
    drupal_add_js(drupal_get_path('module', 'sclib_enkoder') . '/shSetup.js');

    // Translated text.
    $t_about = t('About');
    $t_setup = t('Setup');
    $t_usage = t('Usage');
    $t_how_does_it_work = t('How does it work?');
    $t_about_1 = t('This module formatter settings to enkode arbitrary blocks of text to prevent spambots and other malicious scripts from pulling sensitive data. It also exposes a simple PHP function to do this in a custom way in your own modules or themes.');
    $t_setup_1 = t('Turn this module on.');
    $t_setup_2 = t('(Optionally) If you want to use the PHP function inside a module or theme, declare <strong>sclib_enkoder</strong> as a module dependency.*');
    $t_setup_2_aside = t('* If you are trying to use sclib_enkoder inside a theme, there\'s no good way to do this. Maybe Drupal 8 will allow for themes to depend on modules, but currently there\'s no clean way to do it.  The only possible solution is to wrap every call to sclib_enkoder() in an "if (function_exists(\'sclib_enkoder\'))" check and throw an exception if the test fails.');
    $t_usage_as_formatter = t('As a Field Formatter');
    $t_usage_as_formatter_1 = t('Go to Structure -> Content Types and then "Manage Display" of whatever content type you want to edit.  Then, on any field that has the "Enkode?" summary, you can click the gear to change the setting.');
    $t_usage_as_formatter_2 = t('There are two "on" modes: <em>full</em> and <em>email-only</em>.  <em>Full</em> will enkode the entire field content.  This is pretty secure, and is best used for fields that are dedicated to something sensitive like an email field.  However, this can significantly slow down page performance if used too aggressively or on fields with lots of text in them.');
    $t_usage_as_formatter_3 = t('<em>Email-only</em> enkoding will instead try to parse out two specific matches and encode <em>just</em> those matches:');
    $t_usage_as_formatter_3_a = t('any <code>&lt;a href="mailto:foo@foo&gt;bar&lt;/a&gt;</code> blocks of HTML');
    $t_usage_as_formatter_3_b = t('any bare email addresses');
    $t_usage_as_formatter_4 = t('Email-only matching will potentially be nowhere near as performance-draining as full enkoding, but is only 99% effective (it may not capture complicated mailto: blocks or some rarely used top-level domains).');
    $t_usage_as_formatter_5 = t('In general, if you have an "email" field (or something else sensitive), you should use full enkoding.  Otherwise, if you have a large body text, user-created WYSIWYG field, or something similar where an email address <em>may</em> be stored, you should use email pattern only.');
    $t_usage_as_php = t('In PHP code');
    $t_usage_as_php_1 = t('Simply make a call to sclib_enkoder() passing in the text to enkode as the first argument and optionally placeholder text (for non-Javascript users) as the second.  You\'ll get back enkoded code (per <a href="http://hivelogic.com/enkoder/">http://hivelogic.com/enkoder/</a>).');
    $t_usage_as_php_example_comment = t('This example may output something like:');
    $t_how_does_it_work_1 = t("As per Hivelogic's Enkoder, this module takes any arbitrary text and passes several random encoders over it (all the while constructing a parallel decoding script).  By default, the enkode process makes twenty passes.");
    $t_how_does_it_work_2 = t('The resulting encoding is outputted in a &lt;script&gt; block along with some companion javascript to decode it.  The javascript works by recursive evaluation.');
    $t_how_does_it_work_3 = t('Because of the way it works, simple spambots (which do not evaluate javascript) will have no valid email address pattern to match against.  Moreover, because of the recursive evaluation needed, all but the most determined spambots will be stopped in their tracks.  Combined with a good email host (like <a href="http://gmail.com">Gmail</a>), you should get virtually no spam.');
    $t_how_does_it_work_4 = t('Be warned though - using this too aggressively can foil search engine optimization. Enkoding something a link will make it impossible for a search engine like google to crawl your site, as the enkoded javascript will be unrecognizable as a link (and search engines do not evaluate javascript during their web crawls).');
    $t_how_does_it_work_5 = t('(Note however, that if you provide a sitemap file to google/bing/whatever, you can still enkode even links without any problem.)');

    $base_help = <<<EOT
<ul>
  <li><a href="#about">$t_about</a></li>
  <li><a href="#setup">$t_setup</a></li>
  <li><a href="#usage">$t_usage</a></li>
  <li><a href="#how-does-it-work">$t_how_does_it_work</a></li>
</ul>

<h2><a id="about"></a>$t_about</h2>
<p>$t_about_1</p>

<h2><a id="setup"></a>$t_setup</h2>
<ol>
  <li>$t_setup_1</li>
  <li>$t_setup_2</li>
</ol>

<p>$t_setup_2_aside</p>
<h2><a id="usage"></a>$t_usage</h2>
<h3>$t_usage_as_formatter</h3>
<p>$t_usage_as_formatter_1</p>
<p>$t_usage_as_formatter_2</p>
<p>$t_usage_as_formatter_3</p>
<ol>
  <li>$t_usage_as_formatter_3_a</li>
  <li>$t_usage_as_formatter_3_b</li>
</ol>
<p>$t_usage_as_formatter_4</p>
<p>$t_usage_as_formatter_5</p>

<h3>$t_usage_as_php</h3>
<p>$t_usage_as_php_1</p>

<pre class="brush: php">
\$enk_email = sclib_enkoder( "johnny@foo.com");
drupal_set_message( \$enk_email); /* $t_usage_as_php_example_comment
  &lt;script type="text/javascript"&gt;
  //&lt;![CDATA[
  &lt;!--
  var x="function f(x){var i,o=\"\",ol=x.length,l=ol;while(x.charCodeAt(l/13)!" +
  "=33){try{x+=x;l+=l;}catch(e){}}for(i=l-1;i&gt;=0;i--){o+=x.charAt(i);}return o" +
  ".substr(0,ol);}f(\")15,\\\"lfn}q?&gt;3a!\\\"\\\\*310\\\\3'&/)/fk,-%KWOK520\\\\" +
  "NW]ZCV[W\\\"(f};o nruter};))++y(^)i(tAedoCrahc.x(edoCrahCmorf.gnirtS=+o;721" +
  "=%y;i=+y)15==i(fi{)++i;l&lt;i;0=i(rof;htgnel.x=l,\\\"\\\"=o,i rav{)y,x(f noitc" +
  "nuf\")"                                                                      ;
  while(x=eval(x));
  //--&gt;
  //]]&gt;
  &lt;/script&gt;
*/
</pre>

<h2><a id="how-does-it-work"></a>$t_how_does_it_work</h2>
<p>$t_how_does_it_work_1</p>
<p>$t_how_does_it_work_2</p>
<p>$t_how_does_it_work_3</p>
<p>$t_how_does_it_work_4</p>
<p>$t_how_does_it_work_5</p>
EOT;
    return $base_help;
  }
}
