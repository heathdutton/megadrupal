<article class="markdown-body entry-content" itemprop="mainContentOfPage">
  <h3>Highlight usage</h3>

  <?php if ($show_developer): ?>
    <h2>Installation</h2>

    <p>To use this plugin you need pygments in your server:</p>

    <pre><code>sudo apt-get install python-setuptools
easy_install Pygments
</code></pre>

    <p>That's all. Now you can download the plugin and install it in your Wordpress.</p>
  <?php endif; ?>

  <h2>Usage</h2>

  <?php if ($show_developer): ?>
    <p>Once you get installed the plugin the usage is straightforward. just enclose your code between tokens named with the corresponding lang:</p>
  <?php else: ?>
    <p>To highlight your code just enclose it between tokens named with the corresponding lang:</p>
  <?php endif; ?>

  <p>[javascript]....[/javascript]</p>

  <p>[php]....[/php]</p>

  <p>See <a href="#languages-filetypes-supported">Languages and filetypes supported</a> section to know available languages.</p>

  <h4>
    <a name="parameters" class="anchor" href="#parameters"><span class="mini-icon mini-icon-link"></span></a>Parameters</h4>

  <p>Tokens support a few parameters (all optionals):</p>

  <p>[php <strong>style</strong>="manni" <strong>linenumbers</strong>="table"]....[/php]</p>

  <p><code>style="manni"</code> defines code styling. Currently are 19 available styles.<br>
    Default styling is <code>default</code> wich is very nice, but maybe you like other styles. see <a href="#color-styles">Color styles</a> section.</p>

  <p><code>linenumbers="false"</code> indicates if it should show the numbers and which format. <code>linenumbers="table"</code> or <code>linenumbers="inline"</code></p>

  <h4>
    <a name="examples" class="anchor" href="#examples"><span class="mini-icon mini-icon-link"></span></a>Examples</h4>

<pre><code>[javascript]
//comment line
var foo = "foo";
var bar = function(){
    var baz;
}
[/javascript]
</code></pre>

  <p>Outputs highlighted js with <strong>default</strong> style, and <strong>no</strong> (false) linenumbers.</p>

<pre><code>[javascript style="monokai"]
//comment line
var foo = "foo";
var bar = function(){
    var baz;
}
[/javascript]
</code></pre>

  <p>Outputs highlighted js with <strong>monokai</strong> style, and <strong>no</strong> (false) linenumbers (the default).</p>

  <p>And so on.</p>

  <h2 id="color-styles">Color styles</h2>

  <p>These are supported color styles:</p>

  <ul>
    <li><p><code>monokai</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0018_Layer-20.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0018_Layer-20.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>manni</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0017_Layer-19.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0017_Layer-19.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>rrt</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0016_Layer-18.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0016_Layer-18.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>perldoc</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0015_Layer-17.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0015_Layer-17.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>borland</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0014_Layer-16.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0014_Layer-16.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>colorful</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0013_Layer-15.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0013_Layer-15.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>default</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0012_Layer-14.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0012_Layer-14.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>murphy</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0011_Layer-13.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0011_Layer-13.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>vs</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0010_Layer-12.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0010_Layer-12.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>trac</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0009_Layer-11.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0009_Layer-11.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>tango</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0008_Layer-10.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0008_Layer-10.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>fruity</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0007_Layer-9.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0007_Layer-9.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>autumn</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0006_Layer-8.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0006_Layer-8.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>bw</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0005_Layer-7.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0005_Layer-7.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>emacs</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0004_Layer-6.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0004_Layer-6.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>vim</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0003_Layer-5.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0003_Layer-5.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>pastie</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0002_Layer-4.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0002_Layer-4.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>friendly</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0001_Layer-3.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0001_Layer-3.png" alt="monokai example" style="max-width:100%;"></a></p></li>
    <li><p><code>native</code><br><a href="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0000_Layer-1.png" target="_blank"><img src="<?php print base_path() . $module_path; ?>/tpl/help/img/style__0000_Layer-1.png" alt="monokai example" style="max-width:100%;"></a></p></li>
  </ul>
  <h2 id="languages-filetypes-supported">Languages and filetypes supported</h2>

  <?php if ($show_developer): ?>
    <p>Pygments not only highlights languages. also highlights filetypes like .conf <strong>Nginx</strong> configuration file, <strong>Apache</strong> (filenames .htaccess, apache.conf, apache2.conf), etc.</p>
  <?php endif; ?>

  <h5>
    <a name="general" class="anchor" href="#general"><span class="mini-icon mini-icon-link"></span></a>General</h5>

  <ul>
    <li>
      <code>apacheconf</code>: (.htaccess, apache.conf, apache2.conf)
    </li>
    <li>
      <code>bash</code>, <code>sh</code>, <code>ksh</code>:
      (*.sh, *.ksh, *.bash, *.ebuild, *.eclass, .bashrc, bashrc)
    </li>
    <li>
      <code>ini</code>, <code>cfg</code>: (*.ini, *.cfg)
    </li>
    <li>
      <code>makefile</code>:
      (*.mak, Makefile, makefile, Makefile.*, GNUmakefile)
    </li>
    <li>
      <code>nginx</code>:
      Nginx configuration file
    </li>
    <li>
      <code>yaml</code>:
      (*.yaml, *.yml)
    </li>
    <li>
      <code>perl</code>:
      Perl (*.pl, *.pm)
    </li>
    <li>
      <code>vb.net</code>:
      VB.net (*.vb, *.bas)
    </li>
    <li>
      <code>console</code>:
      Bash Session (*.sh-session)
    </li>
  </ul>
  <h5>
    <a name="javascript" class="anchor" href="#javascript"><span class="mini-icon mini-icon-link"></span></a>Javascript</h5>

  <ul>
    <li>
      <code>javascript</code>: Pure Javascript
    </li>
    <li>
      <code>coffeescript</code>: Pure CoffeeScript
    </li>
    <li>
      <code>json</code>: Pure JSON
    </li>
  </ul>
  <h5>
    <a name="php" class="anchor" href="#php"><span class="mini-icon mini-icon-link"></span></a>PHP</h5>

  <ul>
    <li>
      <code>cssphp</code>: PHP embedded in CSS
    </li>
    <li>
      <code>htmlphp</code>: PHP embedded in HTML
    </li>
    <li>
      <code>jsphp</code>: PHP embedded in JS
    </li>
    <li>
      <code>php</code>: Pure PHP
    </li>
    <li>
      <code>xmlphp</code>: PHP embedded in XML
    </li>
  </ul>
  <h5>
    <a name="ruby" class="anchor" href="#ruby"><span class="mini-icon mini-icon-link"></span></a>Ruby</h5>

  <ul>
    <li>
      <code>ruby</code>, <code>duby</code>: Ruby (*.rb, *.rbw, *.rake, *.gemspec, *.rbx, *.duby)
    </li>
    <li>
      <code>csserb</code>, <code>cssruby</code>: Ruby embedded in CSS
    </li>
    <li>
      <code>xmlerb</code>, <code>xmlruby</code>: Ruby embedded in XML
    </li>
  </ul>
  <h5>
    <a name="css-and-css-compilers" class="anchor" href="#css-and-css-compilers"><span class="mini-icon mini-icon-link"></span></a>CSS and CSS compilers</h5>

  <ul>
    <li>
      <code>css</code>:
      CSS (*.css)
    </li>
    <li>
      <code>sass</code>:
      Sass (*.sass)
    </li>
    <li>
      <code>scss</code>:
      SCSS (*.scss)
    </li>
  </ul>
  <h5>
    <a name="html-and-html-template-systems" class="anchor" href="#html-and-html-template-systems"><span class="mini-icon mini-icon-link"></span></a>HTML and HTML template systems</h5>

  <ul>
    <li>
      <code>html</code>:
      HTML (*.html, *.htm, *.xhtml, *.xslt)
    </li>
    <li>
      <code>haml</code>:
      Haml (*.haml)
    </li>
    <li>
      <code>jade</code>:
      Jade (*.jade)
    </li>
  </ul>
  <h5>
    <a name="sql" class="anchor" href="#sql"><span class="mini-icon mini-icon-link"></span></a>SQL</h5>

  <ul>
    <li>
      <code>sql</code>:
      SQL (*.sql)
    </li>
    <li>
      <code>sqlite3</code>:
      sqlite3con (*.sqlite3-console)
    </li>
    <li>
      <code>mysql</code>:
      MySQL
    </li>
  </ul>
  <h5>
    <a name="python-jinja--django" class="anchor" href="#python-jinja--django"><span class="mini-icon mini-icon-link"></span></a>Python, jinja &amp; Django</h5>

  <ul>
    <li>
      <code>python</code>: Pure Python
    </li>
    <li>
      <code>python3</code>: Pure Python 3
    </li>
    <li>
      <code>xmldjango</code>, <code>xmljinja</code>: Django/Jinja embedded in XML
    </li>
    <li>
      <code>cssdjango</code>, <code>cssjinja</code>: Django/Jinja embedded in CSS
    </li>
    <li>
      <code>django</code>, <code>jinja</code>: Pure Django/Jinja
    </li>
    <li>
      <code>htmldjango</code>, <code>htmljinja</code>: Django/Jinja embedded in HTML
    </li>
    <li>
      <code>jsdjango</code>, <code>jsjinja</code>: Django/Jinja embedded in Javascript
    </li>
  </ul>
  <h5>
    <a name="java--groovy" class="anchor" href="#java--groovy"><span class="mini-icon mini-icon-link"></span></a>Java &amp; Groovy</h5>

  <ul>
    <li>
      <code>java</code>:
      Java (*.java)
    </li>
    <li>
      <code>groovy</code>:
      Groovy (*.groovy)
    </li>
    <li>
      <code>jsp</code>:
      Java Server Page (*.jsp)
    </li>
  </ul>
  <h5>
    <a name="c-c-objetive-c-c-sharp" class="anchor" href="#c-c-objetive-c-c-sharp"><span class="mini-icon mini-icon-link"></span></a>C, C++, Objetive-c, C Sharp</h5>

  <ul>
    <li>
      <code>cobjdump</code>: c-objdump (*.c-objdump)
    </li>
    <li>
      <code>c</code>: C (*.c, *.h, *.idc)
    </li>
    <li>
      <code>cpp</code>: C++ (*.cpp, *.hpp, *.c++, *.h++, *.cc, *.hh, *.cxx, *.hxx)
    </li>
    <li>
      <code>csharp</code>: C# (*.cs)
    </li>
    <li>
      <code>objectivec</code>: (*.m)
    </li>
  </ul>
  <h5>
    <a name="xml" class="anchor" href="#xml"><span class="mini-icon mini-icon-link"></span></a>XML</h5>

  <ul>
    <li>
      <code>xml</code>: (*.xml, *.xsl, *.rss, *.xslt, *.xsd, *.wsdl)
    </li>
    <li>
      <code>xslt</code>: (*.xsl, *.xslt)
    </li>
  </ul>
</article>
