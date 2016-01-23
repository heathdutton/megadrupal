<h3>
  MMS help
</h3>
<p>
  In a multilingual environment, MMS unifies handling of all translations at once for any block, node, or structural content without using separate containers, and with only the Drupal core <em>Locale</em> module activated.
</p>
<section> <!-- how -->
<h4 class="mms-toggler" id="how">
  How it works
</h4>
<p>
  Briefly said, MMS works upon all <code>&lt;&nbsp;input type="text"&nbsp;&gt;</code> and <code>&lt;&nbsp;textarea&nbsp;&gt;</code> elements, giving them the ability to contain their own whole set of translations.
</p>
<p>
  In administration context, each of these areas is replaced by a multilingual widget where the user can instantly look at all translations of the content, and edit them at its option. Another, dedicated widget is also available for areas managed with CKEditor.
</p>
<p>
  The benefits of using this method are mainly:
  <ul>
    <li>when entering new content or updating an existing one, authors keep a total visibility upon all the translations at the same time</li>
    <li>a direct consequence is to reduce the risk of omitting to update the translations when an original content has been modified</li>
    <li>when a content includes a lot of lang-insensitive data (like images, links, or simply verbous HTML with a number of attributes), these parts don't have to be duplicated: only the textual parts must be entered as "multi" and rewritten in the different languages</li>
    <li>since it emulates the SPIP <code>&lt;&nbsp;multi&nbsp;&gt;</code> syntax, this method is nicely suitable to allow the direct migration of contents from this CMS, without the need of a painful data restructuration</li>
    <li>moreover, in the latter case, authors can maintain their previous work habits unchanged</li>
  </ul>
</p>
<p>
  The advantages of using MMS are essentially:
  <ul>
    <li>it works for any content without needing other dedicated translation modules</li>
    <li>it offers a unique interface for the user to watch and manage all translations at the same time for each content</li>
    <li>it uses less database space (less tables, less duplicated content)</li>
  </ul>
</p>
<p>
  At the opposite it implies some limitations:
  <ul>
    <li>it does not allow to manage separate nodes for different languages</li>
    <li>currently it reduces the available space for "small" contents (such as titles), because their allowed size is shared by all translations<br />
    <em>NOTE: this limitation will be lifted in a future version</em></li>
  </ul>
</p>
</section>
<section> <!-- install -->
<h4 class="mms-toggler" id="install">
  Installation/Configuration
</h4>
<p>
  <em>MMS</em> works without the need of any other translation module, assumed you have activated the <em>Locale</em> Drupal core module, and configured the desired languages. Nevertheless you can simultaneously use alternative translation methods, like <em>Multilingual Content</em>, <em>Menu translation</em> etc.
</p>
<p>
  The module is installed with a default configuration that is suitable for most use cases, and that you can fine tune to your wishes (see <a href="#advanced">Advanced configuration</a> below). To immediatlely use the MMS capabilities you simply:
  <ul>
    <li>in "Administer > Configuration > Regional and language > Languages", enable the "URL" detection method, and configure it to "Path prefix"</li>
    <li>for each enabled language, ensure to provide the corresponding standard "Path prefix language code" (don't leave it empty)</li>
    <li>position the <em>Language switcher (User interface text)</em> block in a region, anywhere you like</li>
  </ul>
</p>
<p>
  If you're also using some alternative translation methods (options of the <em>i18n</em> module), there are some additional points you must pay attention to:
  <ul>
    <li>for each node where MMS is used, you MUST select the <strong>Neutral language</strong> in the "Language" field (or the lang switcher would deactivate any lang other than the specified one)</li>
    <li>for each block where MMS is used, you MUST <strong>keep unchecked all the proposed languages</strong> in the "Languages" part of "Visibility settings"</li>
  </ul>
</p>
</section>
<section> <!-- ui -->
<h4 class="mms-toggler" id="ui">
  User interface
</h4>
<p>
  While <em>MMS</em> fundamental work is to interpret the "multi" parts of text found to render it in the current language when displaying public pages, it also manages how these parts are presented to the user when it is creating or updating nodes or blocks contents, their titles, and so on.<br />
  At this moment the user must be able to see and modify all the different translations of the same text: this is provided by the MMS widgets.
  <ul>
    <li><strong>"long-text" fields</strong> (such as body), which represent quantitatively the largest part of text and are input through <code>&lt; textarea &gt;</code> elements, may contain a lot of (even complex) HTML structures: it's why they're usually processed through a wysiwyg editor. So for them <em>MMS</em> includes a <em>CKEditor</em> plugin that can be activated for any text format (see <a href="#ckeditor">Using with <em>CKEditor</em></a>).</li>
      If <em>CKEditor</em> is not installed, these fields are managed by the common MMS plugin described below.
    <li><strong>all other text fields</strong> (including "text", "link" and so on) are presented by Drupal through simple <code>&lt; input type="text" &gt;</code> elements. In this case:
      <ul>
        <li>when an element contains only raw text, a "Use MMS template" link is proposed. When clicking this link:
          <ul>
            <li>the element content is automatically normalized, i.e. formatted with the complete set of currently defined languages</li>
            <li>the original text is affected to the site main language, or to the currently selected language, or to a user-choosen language (see <a href="#advanced-entry-multi">"Multify" process</a>)</li>
            <li>the show/entry widget is displayed as described below</li>
          </ul>
        </li>
        <li>when an element is already in MMS format, with language blocks for each of the currently defined languages, the widget is activated and offers displaying each language part when hovering, as well as the ability to input the different translations in dedicated cells. A "Drop MMS format" link is available, which allows to turn back to a single-language content if desired</li>
        <li>when an element is already in MMS format, but does not yet contains all the currently defined languages (situation for a pre-existing text, when a new language has just been defined), a block is automatically added for the missing language</li>
        <li>each entry area has its own changes history, available during the whole working process for the current form: so you can cancel/repeat all changes, independently for each area</li>
      </ul>
      This way the user remains free to normalize input or not (may be keeping only a limited set of defined languages), while he has an easy way to update when a new language is added to the site.
    </li>
    <li>by extension this behaviour is also available for texts which are not really fields, such as <strong>field labels</strong>, <strong>node or block titles</strong>, <strong>menu links</strong>, <strong>vocabulary names</strong> and <strong>taxonomy terms</strong>, <strong>content types</strong> definitions...</li>
  </ul>
</p>
</section>
<section> <!-- advanced -->
<h4 class="mms-toggler" id="advanced">
  Advanced configuration
</h4>
<p>
  MMS offers a wide range of configuration options, divided into three tabs devoted respectively to the entry, rendering, and management fees. Two additional tabs allow, one to return to the default configuration for all or part of the options, the other to manage the translation of the user interface.
  <ul>
    <li>
      <h5 class="mms-toggler" id="advanced-entry">DATA ENTRY tab</h5>
      <ul>
        <li>
          <h6 class="mms-toggler" id="advanced-entry-iu">USER INTERFACE OPTIONS group</h6>
          <ul>
            <li>Language selection behaviour.<br />
            Each input area includes a language selector, local to this area: hovering a language mark makes the corresponding content to display, clicking it opens the entry area in the choosen language.<br />
            You can define if a language selection applies only to the current area, or is propagated to all input areas in the the page (default value)</li>
            <li>Plain text input possibility.<br />
            By default each entry area is in the form of the MMS multilingual widget, which offers maximum comfort by hiding the underlying "multi" syntax. However it may be useful in some cases to provide direct access to plain text, especially when importing texts from a CMS such as <em>SPIP</em>.<br />
            In the case of a "text-long" area handled with CKEditor, this possibility is natively offered ("Switch to plain text editor" link, or "Source" button). For the other areas, an option allows to define if a button will be proposed to toggle to direct access mode, or if only the multilingual widget is available (default value)</li>
          </ul>
        </li>
        <li>
          <h6 class="mms-toggler" id="advanced-entry-multi">"MULTIFY" PROCESS group</h6>
          <p>When an area contains only text in a single language (for instance a text already existing when MMS was installed), turn to multilingual mode needs to decide to which language the existing text will be affected. Likewise if one wants to turn back to single language from an already multilingual area, it must be decided which language will be retained in the single-language text.<br />
          For both cases above, you can define if MMS will automatically use the site main language (default value), will automatically use the currently selected language, or will prompt user for choose a language</p>
        </li>
        <li>
          <h6 class="mms-toggler" id="advanced-entry-widget">MULTILINGUAL INPUT WIDGET ACTIVATION group</h6>
          <p>By default the multilingual input widget is activated for all input areas: this is generally expected for most of the forms. However this widget is of no interest in some areas (for instance a date or a number).<br />
          A list of CSS selectors registers here all occurrences of such areas in the most popular modules. You can enrich it depending on your needs and preferences.</p>
        </li>
      </ul>
    </li>
    <li>
      <h5 class="mms-toggler" id="advanced-rendering">RENDERING tab</h5>
      <ul>
        <li>
          <h6 class="mms-toggler" id="advanced-rendering-fallback">MISSING TRANSLATION RESPONSE group</h6>
          <p>Inside a multilingual text area, a missing translation corresponds to several distincts cases:
          <ol>
            <li>there is no block for the required language (text existing before MMS was installed, or new language just defined)</li>
            <li>there is a block for the required language, but it contains something like "...Language...", where "Language" is the native language name (area normalized by MMS in the absence of a text content)</li>
            <li>there is a block for the required language, but it is empty (the author erased the block content)</li>
          </ol>
          <ul>
            <li>Cases #1 and #2 are always seen as missing translation and processed consequently (see next points).<br />
            An option allows to define if case #3 is also considered as missing translation (default value), or as voluntary absence of texte (no content will be rendered).</li>
            <li>When a translation is really missing (considering the above option), another option allows to define if it will be replaced by the corresponding text in the site main language (default value), or replaced by a customized message.</li>
            <li>The text that replaces a missing translation may be provided with a tip (title attribute) also customized, and presented through freely defined CSS.</li>
          </ul>
          </p>
        </li>
        <li>
          <h6 class="mms-toggler" id="advanced-rendering-pages">PAGES CONTENT LOCALIZATION group</h6>
          <p>By default, MMS localizes all multilingual contents of a page before displaying it. However most pages containing forms  must escape this localization for typing multilingual texts.<br />
          A list of urls (which can be expressed as regular expressions) registers here all pages of this kind for the most popular modules. You may enrich it depending on your needs and preferences.</p>
        </li>
        <li>
          <h6 class="mms-toggler" id="advanced-rendering-linebreaks">EXTRA LINEBREAKS group</h6>
          <p>MMS allows you to include linebreaks in some short texts (such as a node title or a menu link), where they are normally not allowed by Drupal. By default, these linebreaks are merely rendered as spaces, as if they had not been entered.<br />
          A list of CSS selectors registers here the areas where they will be effectively rendered as linebreaks. You may enrich it depending on your needs and preferences.</p>
        </li>
      </ul>
    </li>
    <li>
      <h5 class="mms-toggler" id="advanced-tokens">TOKENS tab</h5>
      <p>MMS allows you to have alternative tokens that translates multilingual texts they address. Each token is cloned in two copies : first one will render text localized in the currently selected language, second one in the site main language.
      <ul>
        <h6 class="mms-toggler" id="advanced-tokens-raw">RAW TOKENS CLONING group</h6>
        <p>A list registers here the "raw" tokens (as opposed to field tokens, below) that must be cloned. You may enrich it depending on your needs and preferences.<br />
        A token such as <em>[type:token]</em> provides the clones <em>[type:mms-token]</em> and <em>[type:mms-native-token]</em>.</p>
      </ul>
      <ul>
        <h6 class="mms-toggler" id="advanced-tokens-fields">FIELD TOKENS CLONING group</h6>
        <p>A list of checkboxes registers here all the field types currently defined. You may check the types which must cause related fields to be cloned.<br />
        A token such as <em>[node:field_myfield]</em> provides the clones <em>[node:mms-field_myfield]</em> and <em>[node:mms-native-field_myfield]</em></p>
      </ul>
      </p>
    </li>
    <li>
      <h5 class="mms-toggler" id="advanced-default">DEFAULT VALUES tab</h5>
      <p>
      A list of checboxes registers here all options of the three preceding tabs. Launching execution makes each of the checked options to be reset to their default values.</p>
    </li>
    <li>
      <h5 class="mms-toggler" id="advanced-translate">UI TRANSLATIONS tab</h5>
      <p>***
      </p>
    </li>
  </ul>
</p>
</section>
<section> <!-- ckeditor -->
<h4 class="mms-toggler" id="ckeditor">
  Using with <em>CKEditor</em>
</h4>
<p>
  With the <em>CKEditor</em> module, you can benefit from improved input method which gets you rid of the above syntax and automatically offers input areas dedicated to each of the languages defined for the site.<br />
  For this to work:
  <ul>
    <li>your version of <em>CKEditor</em> must include the <em>Widget</em> plugin (you may install it from the <em>CKEditor</em> builder at !CKEditorBuilderLink)</li>
    <li>in the <em>CKEditor</em> configuration, for each profile where you want to allow it, in the "EDITOR APPEARANCE" group:
      <ol>
        <li>in the "Tools bar" section, add the "MMS" button to the tools bar</li>
        <li>in the "Plugins" section, check "MMS" in the list of plugins to be activated</li>
      </ol>
    </li>
    <li>then with this profile any text field part where the "multi" syntax is used automatically displays a "MMS" group, with a subgroup inside for each defined language</li>
    <li>at any time you may click the "MMS" button in the tools bar to create a new empty "MMS" group</li>
    <li>note that only text fields (or blocks text) can be entered through <em>CKEDITOR</em>: in views, and in nodes or blocks titles, you still must use the "multi" syntax, or the improved interface offered by the integrated widget (as of 7.x-2.0)</li>
  </ul>
</p>
</section>
<section> <!-- pathauto -->
<h4 class="mms-toggler" id="pathauto">
  Using with <em>Pathauto</em>
</h4>
<p>
  With the <em>Pathauto</em> module, if you have introduced "multi" syntax in the node titles, you may use the <code>[node:mms&#x2011;native&#x2011;title]</code> token to generate URL aliases, which will be localized using the <strong>site default language</strong>.<br />
  WARNING: using the <code>[node:title]</code> token would generate aliases from the <strong>raw</strong> title, resulting in something like <code>multienmy&#x2011;titlefrmon&#x2011;titremulti</code>!
</p>
</section>
<section> <!-- views -->
<h4 class="mms-toggler" id="views">
  Using with <em>Views</em>
</h4>
<p>
  With the <em>Views</em> module, you can use the "multi" syntax inside of the texts you enter in the definition forms of a view.<br />
  They will be rendered like explained above, in the views summary, in the previews, and of course in the pages where they are included.
</p>
</section>
<section> <!-- syntax -->
<h4 class="mms-toggler" id="syntax">
  Syntax (for raw text mode input)
</h4>
<p>The following describes the raw internal structure which makes the MMS mechanism to work, as initially designed by its predecessor, the <em>Multilang</em> module.<br />
With MMS you can totally ignore the "multi" syntax. However you can continue to use it at your convenience, as part of a direct entry in raw text mode, especially in case of migration from <em>SPIP</em>.<br />
To enter text in this mode, you must:
  <ul>
    <li>in areas handled with <em>CKEditor</em>, click the "Switch to plain text editor" link or the "Source" button of <em>CKEditor</em></li>
    <li>in any other area, click the toggle button "MMS" at the right bottom of the form: then all the MMS widgets disappear and let you see the original entry areas, with their raw content<br />
    <em>NOTE: this toggle button is available only if you have activated the corresponding configuration option (see <a href="#advanced-entry-iu">Plain text input possibility</a>)</em>
    </li>
  </ul>
</p>
<p>
  When entering data in a text area, you may insert multilingual pieces of text by enclosing them between <code>&#91;&nbsp;multi&nbsp;&#93;</code> and <code>&#91;&nbsp;/multi&nbsp;&#93;</code> tags.<br />
  Such a piece of text is a "multi" <em>segment</em>, which may contain a number of "multi" <em>blocks</em>, each representing the desired content translated in a given language, like <code>[language&#x2011;mark]&hellip;content&hellip;</code>, where:
  <ul>
    <li><code>language&#x2011;mark</code> is the involved language code, such as <code>en</code>, <code>fr</code>, <code>es</code>&hellip;</li>
    <li><code>&hellip;content&hellip;</code> is the translated content</li>
  </ul>
</p>
<p>
  Example:<br />
  <blockquote class="mms-code"><code>
    &#91;&nbsp;multi&nbsp;&#93;<br />
    &#91;&nbsp;en&nbsp;&#93; This is an english text<br />
    &#91;&nbsp;fr&nbsp;&#93; Ceci est un texte en fran&ccedil;ais<br />
    &#91;&nbsp;/multi&nbsp;&#93;
  </code></blockquote>
</p>
<p>
  Any spaces, newlines, line-breaks or paragraph-breaks are ignored when they are located just after the opening tags, juste before the closing tags, and just around the language-marks. In other words, each text block is rendered trimmed.
</p>
</section>
<section> <!-- tips -->
<h4 class="mms-toggler" id="tips">
  Tips
</h4>
<ol>
  <li>Which language code is used to render depends on the lang part of the current URL (such as "en" in <code>http://example.com/en/&hellip;</code>), which is generally defined by how the lang switcher is currently set. If no language is currently defined (so Drupal language is empty), the site lang is used.<br />
    As a fortunate side effect, at any moment you may deactivate the lang switcher, and all contents including the "multi" syntax are simply rendered in the site lang.<br />
    CAUTION, though: at the time this document is written (Drupal 7.34), deactivate the lang switcher seems to drop already registered vocabulary terms translations!</li>
  <li>If a "multi" segment does not contain translation for the current lang, the available text in the site lang will be rendered instead. However you may configure MMS in order to not display anything at all in this case(see <a href="#advanced-rendering-fallback">Missing translation</a>)</li>
  <li>In order to allow a simple migration of contents from the Spip CMS, an alternative syntax is also accepted, using HTML-fashion tags such as <code>&lt;&nbsp;multi&nbsp;&gt;</code> and <code>&lt;&nbsp;/multi&nbsp;&gt;</code>, rather than <code>&#91;&nbsp;multi&nbsp;&#93;</code> and <code>&#91;&nbsp;/multi&nbsp;&#93;</code> (but language marks keep using square brackets, like in <code>&#91; en  &#93;</code>).<br />
    You may also use this alternative syntax when manually entering text (when not in <em>CKEditor</em> nor in MMS widget).</li>
  <li>Becareful when directly importing data from an external source: due to the internal rules of the <em>HTML purifier</em> plugin, <em>CKEditor</em> breaks the <em>MMS</em>'s automatic recognition of language blocks when they include <code>&lt; div &gt;</code> tags. You may consider to replace them by <code>&lt; section &gt;</code> tags.<br />
    Another solution could be to deactivate the <em>HTML purifier</em> plugin, so the &lt;div&gt; tags can be used, but this is not recommended, for general security reasons.</li>
</ol>
</section>
<section> <!-- localization -->
<h4 class="mms-toggler" id="localization">
  Localization
</h4>
<p>
  Because of its mission to integrate multiple languages in the same container, the <em>MMS</em> module does not conform to the standard Drupal localization system: in this domain, it is self-sufficient and embeds all of its own translations, written with the "multi" syntax.<br />
  All these translations (except the help text you are reading now) are gathered in the file <code>mms.data.inc</code> : if it does not include a version in your favorite language you can easily add it using the tools available at the user interface translation page !MMSUILink.<br />
</p>
<p>
  Due to its size, the help text is not included in the <code>mms.data.inc</code> file. Instead it resides in a set of independent files (one for each language) like <code>mms.help.<em>LANGUAGE</em>.inc</code>, where the contents are pure HTML.<br />
  So to add a version of the help text in your favorite language, say XX, you have to create a new file named <code>mms.help.<em>XX</em>.inc</code> and located in the <code>sites/all/mms</code> folder. Please be careful to keep unchanged the original HTML structure, especially the sections headings with their <code>class</code> and <code>id</code> attributes.
</p>
<p>
  <em>NOTE FOR ENGLISH SPEAKERS: the creator of this module is not fluent in English, so he is unsure of the quality of writing. <b>All corrections and improvements are welcome.</b></em>
</p>
</section>
