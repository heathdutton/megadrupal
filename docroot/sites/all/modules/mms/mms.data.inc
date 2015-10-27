<?php
/**
 * @file
 * MMS module text data.
 *
 * Contains all texts translations, written using "multi" syntax.
 */

/* ================================================================== INSTALL */
/**
 * Informative message after module install.
 */
function _mms_install_data() {
  module_load_include('inc', 'mms', 'mms.core');
  ob_start();
?>
[multi]
[en]The <em>MMS</em> module was successfully installed.<br />
Now you may use the MMS widget to enter multilingual data in titles and fields
of nodes and blocks, as well as in any other text data entry area (taxonomy,
menus, and so on).<br />
NOTE: in order to use the
<a href="<?php echo MMS_BASE_PATH; ?>en/admin/help/mms#ckeditor">
advanced features</a>
available with CKEditor, you must install the CKEditor <em>Widget</em> plugin.
[fr]Le module <em>MMS</em> a été installé avec succès.<br />
Vous pouvez désormais utiliser le widget MMS pour entrer des données
multilingues à l'intérieur des titres et champs de noeuds ou de blocs, ainsi
que dans toutes autres les zones de saisie de texte (taxonomie, menus, etc).
<br />
NOTA : pour utiliser les
<a href="<?php echo MMS_BASE_PATH; ?>fr/admin/help/mms#ckeditor">
possibilités avancées</a> disponibles avec CKEditor, vous devez installer le
plugin CKEditor <em>Widget</em>.
[/multi]
<?php
  return _mms_process(ob_get_clean());
}

/**
 * Error message when cannot hack Drupal core.
 */
function _mms_install_error() {
  module_load_include('inc', 'mms', 'mms.core');
  ob_start();
?>
[multi]
[en]Though the <em>MMS</em> module was successfully installed, needed
changes in the Drupal core includes/unicode.inc file could not be inserted.<br />
Under these conditions, overall MMS is functional but some translations
will not be assured: we advise you to visit the
<a href="https://www.drupal.org/project/issues/search/mms">Issues for
MMS page</a> to look for a solution and/or create a new issue.
[fr]Bien que le module <em>MMS</em> ait été installé avec succès, il
n\'a pas été possible d\'introduire les modifications nécessaires dans le
fichier includes/unicode.inc du coeur de Drupal.<br />
Dans ces conditions, MMS est globalement fonctionnel mais certaines
traductions ne seront pas assurées : nous vous conseillons de vous rendre sur
la <a href="https://www.drupal.org/project/issues/search/mms">page des
problèmes MMS</a> pour rechercher une solution et/ou créer un
rapport d'erreur.
[/multi]
<?php
  return _mms_process(ob_get_clean());
}

/**
 * Informative message after module uninstall.
 */
function _mms_uninstall_data() {
  module_load_include('inc', 'mms', 'mms.core');
  ob_start();
?>
The <em>MMS</em> module was successfully uninstalled.<br />
Nevertheless "multi" data previously recorded remain unchanged.
<!--
TODO: offer an "unmulti" function to turn "multi" segments into single language.
-->
<?php
  return ob_get_clean();
}
/* ================================================================= FALLBACK */
/**
 * Default title attribute for the replacement text when a "multi" segment
 * doesn't contain the required language.
 *
 * WARNING: returns this raw "multi" segment, used by JS widget.
 */
function _mms_fallback_message_data() {
  ob_start();
?>
[multi]
[en]Missing translation
[fr]Traduction manquante
[/multi]
<?php
  return ob_get_clean();
}
/**
 * Default tip displayed when a "multi" segment doesn't contain the required
 * language.
 *
 * WARNING: returns this raw "multi" segment, used by JS widget.
 */
function _mms_fallback_tip_data() {
  ob_start();
/* TODO: enlarge Drupal allowed length for certain input data.
?>
[multi]
[en]This part is not translated yet. We apologize for any inconvenience.
[fr]Cette partie n'a pas encore été traduite. Merci de votre compréhension.
[/multi]
<?php
*/
?>
[multi]
[en]This part is not translated yet
[fr]Cette partie n'a pas encore été traduite
[/multi]
<?php
  return ob_get_clean();
}
/* ===================================================================== MENU */
/**
 * Localized menu messages.
 *
 * WARNING: returns the raw "multi" segment for the given $message_id.
 * This is because the menu structure is cached, so it is not re-evaluated each
 * time it is inserted. With raw "multi" segments cached the actual HTML
 * is lastly rendered localized by mms_preprocess_html().
 */
function _mms_menu_data($message_id) {
  $messages = array(
    'menu_description' => '
[multi]
[en]MMS settings
[fr]Configuration de MMS
[/multi]
    ',
    'menu_entry_title' => '
[multi]
[en]Data entry
[fr]Saisie
[/multi]
    ',
    'menu_entry_description' => '
[multi]
[en]Configure the MMS widget behaviour for data entry
[fr]Configurer le comportement du widget MMS pour la saisie des données
[/multi]
    ',
    'menu_rendering_title' => '
[multi]
[en]Rendering
[fr]Rendu
[/multi]
    ',
    'menu_rendering_description' => '
[multi]
[en]Configure where and how MMS must localize text
[fr]Configurer où et comment MMS doit localiser le texte
[/multi]
    ',
    'menu_tokens_title' => '
[multi]
[en]Tokens
[fr]Jetons
[/multi]
    ',
    'menu_tokens_description' => '
[multi]
[en]Choose which tokens MMS must clone
[fr]Choisir les jetons que MMS doit clôner
[/multi]
    ',
    'menu_default_title' => '
[multi]
[en]Default values
[fr]Valeurs par défaut
[/multi]
    ',
    'menu_default_description' => '
[multi]
[en]Reset some or all of the configuration options back to default values
[fr]Réinitialiser tout ou partie des options de configuration à leur valeur par
défaut
[/multi]
    ',
  );
  return trim($messages[$message_id]);
}
/* ==================================================================== ADMIN */
/**
 * Localized administration form messages.
 */
function _mms_admin_data($message_id) {
  module_load_include('inc', 'mms', 'mms.core');
  $messages = array(
    // -----------------------------------------------mms_rendering_form()
    // SELECTION
    'selection_title' => '
[multi]
[en]Language selection inside of an entry area
[fr]Sélection de langue dans une zone de saisie
[/multi]
    ',
    'selection_markup' => '
[multi]
[en]Each entry area includes a local language selector for this area: hovering
one language mark will display the corresponding content, click opens the input
area in the chosen language.
[fr]Chaque zone de saisie comporte un sélecteur de langues local à cette zone :
survoler une marque de langue provoque l\'affichage du contenu correspondant,
cliquer ouvre la zone de saisie dans la langue choisie.
[/multi]
    ',
    // selection follow
    'selection_follow_title' => '
[multi]
[en]Language selection behaviour
[fr]Comportement de la sélection de langue
[/multi]
    ',
    'selection_follow_description' => '
<em>[multi]
[en]WARNING: This option applies only to input fields; everything else in the
page remains subject to the actual setting of the main language selector.
[fr]ATTENTION : cette option ne concerne que les zones de saisie ; tout le reste
de la page reste soumis au réglage actuel du sélecteur de langue principal.
[/multi]</em>
    ',
    'follow_false' => '
[multi]
[en]selecting a language in an entry area applies only to this area
[fr]la sélection d\'une langue dans une zone de saisie s\'applique uniquement à
cette zone
[/multi]
    ',
    'follow_true' => '
[multi]
[en]selecting a language in any entry area propagates this selection to all
entry areas of the same page
[fr]la sélection d\'une langue dans une zone de saisie propage cette sélection à
toutes les autres zones de saisie de la même page
[/multi]
    ',
    // MULTIFY
    'multify_title' => '
[multi]
[en]"Multify" process
[fr]Processus de "multification"
[/multi]
    ',
    'multify_markup' => '
[multi]
[en]
<p>When a data entry area contains only raw text (i.e. without "multi" syntax),
a link "Use MMS template" is proposed. Click on the link causes the field
content to be turned into a "multi" segment, with distinct areas for each of the
languages ​​currently defined.<br />
For example, if the languages ​​are "en" and "fr", an area containing<br />
&nbsp;&nbsp;<code>This is a plain text in English</code><br />
may be converted into<br />
&nbsp;&nbsp;<code>[ multi ] [ en ] This is a plain text in English [ fr
] ...French... [ /multi ]</code><br />
where "...French..." is a simple visual cue while no translation exists yet,
and will not be reproduced on the localized page in that language.
</p>In contrast, when a data entry area contains a "multi" segment, a link
"Drop MMS format" is proposed. Click on this link causes the content to
return to plain text, where only a single translation is preserved.
<p>
</p>
<p> The following options specify which language should be considered when
performing these two operations.
</p>
[fr]
<p>Lorsqu\'une zone de saise ne contient qu\'un texte brut (i.e. sans
syntaxe "multi"), un lien "Utiliser le gabarit MMS" est proposé. Cliquer
sur ce lien provoque la transformation du contenu de la zone en un segment
"multi", comportant des zones distinctes pour chacune des langues actuellement
définies.<br />
Par exemple, si les langues sont "fr" et "en", une zone contenant<br />
&nbsp;&nbsp;<code>Ceci est un texte brut en français</code><br />
pourra être transformée en<br />
&nbsp;&nbsp;<code>[ multi ] [ fr ] Ceci est un texte brut en français [ en
] ...English... [ /multi ]</code><br />
où "...English..." est un simple repère visuel, en l\'absence provisoire de
traduction, et ne sera pas reproduit sur la page localisée dans cette langue.
</p>A l\'opposé, lorsqu\'une zone de saise contient un segment "multi", c\'est
un lien "Abandonner le format MMS" qui est proposé. Cliquer sur ce lien
provoque le retour à un texte brut, dans lequel une seule traduction est
conservée.
<p>
</p>
<p>Les options ci-après définissent quelle langue doit être considérée pour
exécuter ces deux opérations.
</p>
[/multi]
    ',
    // multify backlang
    'multify_backlang_title' => '
[multi]
[en]Choose which language to preserve when dropping a "multi" segment
[fr]Choisir quelle langue conserver lorsqu\'un segment "multi" est abandonné
[/multi]
    ',
    'multify_backlang_description' => '
[multi]
[en]
When a user clicks "Drop MMS format", the only text preserved in the
input area is the one corresponding to the language defined above.
[fr]
Lorsqu\'un utilisateur clique sur "Abandonner le format MMS", le seul
texte conservé dans la zone de saisie est celui correspondant à la langue
définie ci-dessus.
[/multi]
    ',
    // multify fromlang
    'multify_fromlang_title' => '
[multi]
[en]Choose which language to affect to a raw text when creating a "multi"
segment
[fr]Choisir quelle langue affecter à un texte brut lors de la création d\'un
segment "multi"
[/multi]
    ',
    'multify_fromlang_description' => '
[multi]
[en]
When a user clicks "Use MMS template" for an input area which contains
only raw text (i.e. without "multi" syntax), this text is affected to the
language defined above.
[fr]
Lorsqu\'un utilisateur clique sur "Utiliser le gabarit MMS" pour une
zone de saisie ne contenant que du texte brut (i.e. sans syntaxe "multi"), ce
texte est affecté à la langue définie ci-dessus.
[/multi]
    ',
    // multify backlang & fromlang
    'multify_default' => '
[multi]
[en]use the default site language
[fr]utiliser la langue par défaut du site
[/multi]
    ',
    'multify_current' => '
[multi]
[en]use the current language
[fr]utiliser la langue actuelle
[/multi]
    ',
    'multify_ask' => '
[multi]
[en]ask user to choose a language (all defined languages will be offered)
[fr]demander à l\'utilisateur de choisir une langue (toutes les langues définies
seront proposées)
[/multi]
    ',
    // WIDGET
    'widget_title' => '
[multi]
[en]Multilingual input widget activation
[fr]Activation du widget de saisie multilingue
[/multi]
    ',
    'widget_markup' => '
[multi]
[en]
By default the multilingual input widget is activated for all input areas:
this is generally expected for most of the forms. At the opposite though some
areas must remain free of this widget, e.g.:
<ul>
  <li>in the pages offered to visitors, the search area</li>
  <li>in node edit forms, entry areas for a date or number</li>
</ul>
<p><em>
NOTE: you are not forced to register ALL areas where the input widget is not
relevant, since its presence will not necessarily imply it will be applied.
When in doubt as to its usefulness in a particular area, or simply if you want
to avoid too dense census, limit yourself to the most obvious cases.
</em></p>
[fr]
Par défaut, le widget de saisie multilingue est activé pour toutes les zones
de saisie : c\'est ce qui est généralement souhaitable dans la plupart des
formulaires. Cependant certaines zones doivent au contraire ne pas offrir ce 
widget, par exemple :
<ul>
  <li>dans les pages présentées à l\'internaute, la zone de recherche</li>
  <li>dans les formulaires de mise à jour de noeuds, les zones de saisie d\'une
  date, ou d\'un nombre</li>
</ul>
<p><em>
NOTA : rien ne vous oblige à recenser TOUTES les zones où le widget de saisie
est sans intérêt, puisque sa présence n\'implique pas forcément sa mise en
oeuvre. Dans le doute quant à son utilité pour telle ou telle zone, ou
simplement si vous souhaitez éviter un recensement trop touffu, limitez-vous aux
cas les plus évidents.
</em></p>
[/multi]
    ',
    'widget_no_widget_title' => '
[multi]
[en]Areas where to NOT activate widget
[fr]Zones où le widget NE doit PAS être activé
[/multi]
    ',
    'widget_no_widget_description' => '
[multi]
[en]
You may add CSS selectors to target areas where to not activate widget.<br />
Note that the same area may sometimes be differently indicated depending on
which theme is used (provided default values target the <em>Seven</em> theme).
[fr]
Vous pouvez ajouter des sélecteurs CSS pour cibler des zones où le widget ne
doit pas être activé.<br />
Notez que la même zone peut parfois être désignée de façon différente selon le
thème utilisé (les valeurs par défaut fournies ciblent le thème <em>Seven</em>).
[/multi]
    ',
    // FALLBACK
    'fallback_title' => '
[multi]
[en]Missing translation response
[fr]Réponse en cas de traduction manquante
[/multi]
    ',
    'fallback_markup' => '
[multi]
[en]
<p>Options below define how MMS must react when it encounters a
segment that has no translation in the current language.
</p>
<p><em>NOTE: "no translation" means that the desired block (i.e. the involved 
language mark, such as </em>[ en ]<em>, followed by a string), either:
  <ul>
    <li>contains something like </em>...nativeLanguage...<em> where 
    "nativeLanguage" is the name of the language in this language itself, such
    as </em>...English...<em><br />
    (this is the default content set by MMS to each new block of a "multi"
    segment)
    </li>
    <li>or is merely empty (no string following </em>[ en ]<em>)</li>
    <li>or is downright missing (e.g. when a new language was just defined)</li>
  </ul>
</em></p>
[fr]
<p>Les options ci-après définissent comment MMS doit réagir lorsqu\'il
rencontre un segment qui ne possède pas de traduction dans la langue actuelle.
</p>
<p><em>NOTA : "pas de traduction" signifie que le bloc recherché (i.e. la marque
de langue concernée, par exemple </em>[ fr ]<em>, suivie d\'une chaîne de 
caractères) :
  <ul>
    <li>contient quelque chose de la forme </em>...langueNative...<em>, où
    "langueNative" est le nom de la langue dans cette langue elle-même, par 
    exemple </em>...Français...<em><br />
    (c\'est le contenu par défaut donné par MMS à tout nouveau bloc d\'un
    segment "multi")</li>
    <li>ou est simplement vide (pas de chaîne suivant </em>[ fr ]<em>)</li>
    <li>ou est carrément manquant (par exemple lorsqu\'une nouvelle langue vient
    d\'être définie)</li>
  </ul>
</em></p>
[/multi]
    ',
    // fallback empty
    'fallback_empty_title' => '
[multi]
[en]What if empty translation
[fr]Que faire en cas de traduction vide
[/multi]
    ',
    'fallback_empty_description' => '
<em>[multi]
[en]NOTE: this option only regards <strong>really empty</strong> translations,
i.e. where the language mark (e. g. </em>[ fr ]<em>) is present but followed by
no text. It is useful to allow some texts to be outright "invisible" in the
given language.
[fr]NOTA : cette option concerne uniquement les traductions <strong>réellement
vides</strong>, c\'est-à-dire où la marque de langue (par exemple
</em>[ en ]<em>) est bien présente mais n\'est suivie d\'aucun texte. Elle est
utile pour autoriser qu\'un texte soit purement et simplement "invisible" dans
la langue choisie.
[/multi]</em>
    ',
    'fallback_empty_replace' => '
[multi]
[en]replace missing translation according to options below
[fr]remplacer la traduction manquante selon les options ci-dessous
[/multi]
    ',
    'fallback_empty_empty' => '
[multi]
[en]keep it empty: no text will be displayed for the involved language
[fr]laisser vide : aucun texte ne sera affiché pour la langue concernée
[/multi]
    ',
    // fallback option
    'fallback_option_title' => '
[multi]
[en]Missing translation replacement text
[fr]Texte de remplacement pour une traduction manquante
[/multi]
    ',
    'fallback_option_description' => '
<em>[multi]
[en]NOTE: in both cases the replacement text will be wrapped in a
</em>&lt; span &gt;<em> element that you can customize using the "title"
attribute and CSS properties
below.
[fr]NOTA : dans les deux cas le texte de remplacement sera emballé dans un
élément </em>&lt; span &gt;<em> que vous pouvez personnaliser à l\'aide de
l\'attribut "title" et des propriétés CSS ci-dessous.
[/multi]</em>
    ',
    'fallback_option_default' => '
[multi]
[en]its translation in the site default language
[fr]sa traduction dans la langue du site par défaut
[/multi]
    ',
    'fallback_option_message' => '
[multi]
[en]a customized informative message
[fr]un message d\'information personnalisé
[/multi]
    ',
    // fallback message
    'fallback_message_title' => '
[multi]
[en]Customized informative message
[fr]Message d\'information personnalisé
[/multi]
    ',
    'fallback_message_description' => '
[multi]
[en]
WARNING: this message is itself a "multi" segment, and will be localized
in the current language before displaying. <strong>But unlike the text it 
replaces, it MUST include translations in each of the defined
languages</strong> (otherwise it will be replaced by " ? ? ? ").
[fr]
ATTENTION : ce message est lui-même un segment "multi", et sera localisé
dans la langue courante avant affichage. <strong>Mais contrairement au texte
qu\'il remplace, il DOIT comporter une traduction dans chacune des langues
définies</strong> (sinon il sera remplacé par " ? ? ? ").
[/multi]
    ',
    // fallback tip
    'fallback_tip_title' => '
[multi]
[en]Replacement text "title" attribute
[fr]Attribut "title" du texte de remplacement
[/multi]
    ',
    'fallback_tip_description' => '
[multi]
[en]
WARNING: this message is itself a "multi" segment, and will be localized
in the current language before displaying. You may choose to leave it empty
(in whole, or for one or more languages) to prevent displaying a tip.
[fr]
ATTENTION : ce message est lui-même un segment "multi", et sera localisé
dans la langue courante avant affichage. Vous pouvez choisir de le laisser
vide (en totalité, ou pour une ou plusieurs langues) pour éviter l\'affichage
d\'une bulle d\'aide.
[/multi]
    ',
    // fallback css
    'fallback_css_title' => '
[multi]
[en]Replacement text CSS properties
[fr]Propriétés CSS pour le texte de remplacement
[/multi]
    ',
    'fallback_css_description' => '
[multi]
[en]You may enter several CSS properties (once per line, the final ";" is
optional). They will be affected to the &lt; span &gt; element wrapping the
replacement text.
[fr]Vous pouvez saisir plusieurs propriétés CSS (une par ligne, le ";" final
est facultatif). Elles seront affectées à l\'élément &lt; span &gt englobant le
texte de remplacement.
[/multi]
    ',
    // PAGES
    'pages_title' => '
[multi]
[en]Pages content localization
[fr]Localisation du contenu des pages
[/multi]
    ',
    'pages_markup' => '
[multi]
[en]
<p>
MMS is designed to look at any page to render texts localized, everywhere
it encounters "multi" segments. However, to allow entering multilingual
content, it must keep these pieces of text untouched when they are part of an
input area offered to the user during the site administration process.
</p>
<p>
  This is achieved by instructing MMS to exclude some given pages from
  the global localization process (actually all pages which are forms including
  data entry areas like &lt; input &gt; or &lt; textarea &gt; elements) whose
  content may include "multi" segments.<br />
  In those pages, all other elements are then automatically localized at the
  moment they are displayed.
</p>
[fr]
<p>
MMS est conçu pour inspecter toutes les pages, en vue de rendre les textes
localisés chaque fois qu\'il rencontre des segments "multi". Cependant, pour
autoriser la saisie de contenus multilingues, il doit conserver intactes ces
portions de texte lorsqu\'elles font partie d\'une zone de saisie offerte à
l\'utilisateur dans le cadre de l\'administration du site.
</p>
<p>
  Pour ce faire on donne pour instruction à MMS d\'exclure certaines
  pages du processus global de localisation (en fait, toutes les pages qui sont
  des formulaires comportant des zones de saisie comme des éléments
  &lt; input &gt; ou &lt; textarea &gt;) dont le contenu peut inclure des
  segments "multi".<br />
  Au sein de ces pages, tous les autres éléments sont ensuite localisés
  automatiquement au moment où elles sont affichées.
</p>
[/multi]
    ',
    'pages_paths_title' => '
[multi]
[en]Pages to exclude from global localisation
[fr]Pages à exclure de la localisation globale
[/multi]
    ',
    'pages_paths_description' => '
[multi]
[en]
You may use regular expressions to add paths of pages to exclude from the
localization process.<br />
<strong>WARNING: don not modify the above default provided list unless you know
full well what you are doing.</strong>
[fr]
Vous pouvez utiliser des expressions régulières pour ajouter des chemins de
pages à exclure du processus de localisation.<br />
<strong>ATTENTION : ne modifiez pas ci-dessus la liste fournie par défaut, sauf
si vous savez pertinemment ce que vous faites.</strong>
[/multi]
    ',
    // LINEBREAKS
    'linebreaks_title' => '
[multi]
[en]Extra line breaks
[fr]Sauts de ligne supplémentaires
[/multi]
    ',
    'linebreaks_markup' => '
[multi]
[en]
<p>
MMS allows to introduce line breaks when typing some short texts (like
the title of a node or a menu item), where they are not normally allowed by
Drupal. By default, these line breaks are simply rendered as spaces, as if they
had not been entered.
You can define in which location of the page you want them to be actually
rendered as line breaks.
</p>
[fr]
<p>
MMS permet d\'introduire des sauts de ligne lors de la saisie de certains
textes courts (comme le titre d\'un noeud ou d\'une option de menu), là où ils
ne sont normalement pas autorisés par Drupal. Par défaut, ces sauts de ligne
sont simplement rendus en tant qu\'espaces, comme s\'ils n\'avaient pas été
saisis.<br />
Vous pouvez définir dans quels emplacements de la page vous souhaitez qu\'ils
soient effectivement rendus en tant que sauts de ligne.
</p>
[/multi]
    ',
    'linebreaks_use_title' => '
[multi]
[en]Areas where line breaks must be effective
[fr]Zones où les sauts de ligne doivent être effectifs
[/multi]
    ',
    'linebreaks_use_description' => '
[multi]
[en]
You may add CSS selectors to target areas where you want the line breaks are
effective.<br />
Alternatively you can safely let this list empty: in this case existing line
breaks in short texts will never turn effective.
<p><em>NOTE: inserting line breaks in places where they have not been provided
for in the theme may cause distortions in the appearance of the page. In such a
case you will have to change the theme CSS accordingly.</em></p>
[fr]
Vous pouvez ajouter des sélecteurs CSS pour cibler les zones où vous souhaitez
que les sauts de lignes soient effectifs.<br />
A l\'opposé, vous pouvez sans risque laisser cette liste vide : dans ce cas les
sauts de ligne figurant dans les textes courts ne seront jamais effectifs.
<p><em>NOTA : l\'insertion de sauts de ligne dans des emplacements où ils n\'ont
pas été prévus par le thème peut entraîner des déformations dans l\'aspect de
la page. Dans un tel cas vous devrez modifier les CSS du thème en conséquence.
</em></p>
[/multi]
    ',
    // --------------------------------------------------mms_tokens_form()
    // raw tokens
    'tokens_title' => '
[multi]
[en]Raw tokens cloning
[fr]Clônage de jetons bruts
[/multi]
    ',
    'tokens_markup' => '
[multi]
[en]
Some of the existing tokens may be cloned so for an original
<em>[type:token]</em> token resolving in <em>value</em>, two new tokens will
be created:
<ul>
  <li><em>[type:mms-token]</em> resolving in current language localized
  <em>value</em></li>
  <li><em>[type:mms-native-token]</em> resolving in site default 
  language localized <em>value</em> whatever the current language is</li>
  </ul>
[fr]
Certains des jetons existants peuvent être clônés de sorte que, pour un jeton
original comme <em>[type:token]</em> se résolvant en <em>valeur</em>, deux
nouveaux jetons seront créés :
<ul>
  <li><em>[type:mms-token]</em>, se résolvant comme <em>valeur</em> 
  localisée dans la langue actuelle</li>
  <li><em>[type:mms-native-token]</em>, se résolvant comme
  <em>valeur</em> dans la langue du site par défaut, quelle que soit la langue
  actuelle</li>
</ul>
[/multi]
    ',
    'tokens_tokens_title' => '
[multi]
[en]Raw tokens to clone
[fr]Jetons bruts à clôner
[/multi]
    ',
    'tokens_tokens_description' => '
[multi]
[en]You may add tokens to the list above, in the form <em>[type:token]</em> or
merely <em>type:token</em>.<br />
<strong>WARNING: here don\'t add tokens like <em>[node:field_...]</em> (see
below).</strong>
[fr]Vous pouvez ajouter des jetons à la liste ci-dessus, sous la forme
<em>[type:token]</em> ou simplement <em>type:token</em>.<br />
<strong>ATTENTION : n\'ajoutez pas ici de jetons du type 
<em>[node:field_...]</em> (voir ci-dessous).</strong>
[/multi]
    ',
    // field-type tokens
    'field_types_title' => '
[multi]
[en]Field tokens cloning
[fr]Clônage de jetons de champs
[/multi]
    ',
    'field_types_markup' => '
[multi]
[en]<p>
  By default the Token module creates a token for each of the user- or
  module-defined field. E.g. for a field whose machine-name is <em>myfield</em>
  it creates a token <em>[node:field_myfield]</em>.<br />
  MMS can clone those field tokens as well as raw tokens. In the above
  example: <em>[node:mms-field_myfield]</em> and
  <em>[node:mms-native-field_myfield]</em>.
</p>
<p>
  Below you may define for which field types MMS must create clones. Check
  or uncheck to allow/disallow each of those field types.
  <strong>
    You should only check field types which can contain textual data (strings).
  </strong>
</p>
[fr]<p>
  Par défaut, le module Token crée un jeton pour chacun des champs définis par
  l\'utilisateur ou par un module. Par exemple, pour un champ de nom-système
  <em>myfield</em>, il crée un jeton <em>[node:field_myfield]</em>.<br />
  MMS peut clôner ces jetons de champs, de la même façon que les jetons
  bruts. Dans l\'exemple ci-dessus : <em>[node:mms-field_myfield]</em> et
  <em>[node:mms-native-field_myfield]</em>.
</p>
<p>
  Vous pouvez définir ci-dessous pour quels types de champ MMS doit créer
  des clônes. Cochez ou décochez pour autoriser/interdire chacun de ces types de
  champs.
  <strong>
    Vous ne devriez cocher que des types de champs pouvant contenir du texte
    (chaînes de caractères).
  </strong>
</p>
[/multi]
    ',
    // -------------------------------------------------mms_default_form()
    // default
    'default_markup' => '
[multi]
[en]Here you may reset all or part of the MMS configuration.<br .>
[fr]Ici vous pouvez réinitialiser tout ou partie de la configuration de
MMS.<br />
[/multi]
    ',
    'default_title' => '
[multi]
[en]Options to reset
[fr]Options à réinitialiser
[/multi]
    ',
    'default_description' => '
<br />
[multi]
[en]Check the options you want to reset to their default values.
[fr]Cochez les options que vous voulez réinitialiser à leurs valeurs par défaut.
[/multi]
    ',
    'default_submit_button' => '
[multi]
[en]Start reset
[fr]Lancer la réinitialisation
[/multi]
    ',
    'default_submit_title' => '
[multi]
[en]Resetting MMS configuration
[fr]Réinitialisation de la configuration de MMS
[/multi]
    ',
    'default_submit_init' => '
[multi]
[en]Reset
[fr]Réinitialisation
[/multi]
    ',
    'default_submit_none' => '
[multi]
[en]No category to process!
[fr]Aucune catégorie à traiter !
[/multi]
    ',
    'default_finished_ok' => '
[multi]
[en]MMS configuration reset for
[fr]Configuration de MMS réinitialisée pour
[/multi]
    ',
    'default_finished_error' => '
[multi]
[en]An error happened. The following categories have not been reset:
[fr]Une erreur s\'est produite. Les catégories suivantes n\'ont pas été 
réinitialisées :
[/multi]
    ',
  );
  return trim(
    _mms_process($messages[$message_id], ['can_highlight' => FALSE])
  );
}
/* =================================================================== TOKENS */
/**
 * Localized tokens characteristics.
 */
function _mms_token_adds_data() {
  return array(
    'signature' => '
[multi]
[en]with MMS
[fr]avec MMS
[/multi]
    ',
    'locale-short' => '
[multi]
[en](current language)
[fr](langue actuelle)
[/multi]
    ',
    'locale-long' => '
[multi]
[en]in the current language
[fr]dans la langue actuelle
[/multi]
    ',
    'native-short' => '
[multi]
[en](site language)
[fr](langue du site)
[/multi]
    ',
    'native-long' => '
[multi]
[en]in the default site language
[fr]dans la langue du site par défaut
[/multi]
    ',
  );
}
/* ================================================================ JS WIDGET */
/**
 * Informative text used in mms.js widget.
 */
function _mms_js_widget_data() {
  module_load_include('inc', 'mms', 'mms.core');
  ob_start();
  // CAUTION: this is JSON data.
?>
{
  "cancel":
[multi]
[en]"Cancel"
[fr]"Annuler"
[/multi],
  "curLang":
[multi]
[en]"current language"
[fr]"langue actuelle"
[/multi],
  "defLang":
[multi]
[en]"default site language"
[fr]"langue du site par défaut"
[/multi],
  "dropLang":
[multi]
[en]"Return to a single language.<br />Choose which language must be preserved."
[fr]"Retour à une langue unique.<br />Choisissez quelle langue doit être conservée."
[/multi],
  "dropText":
[multi]
[en]"Drop MMS format"
[fr]"Abandonner le format MMS"
[/multi],
  "dropTitle":
[multi]
[en]"Back to content with only one language"
[fr]"Revenir à un contenu ne comportant qu'une seule langue"
[/multi],
  "inputTitle":
[multi]
[en]"Click to display a per-language entry area"
[fr]"Cliquer pour afficher une zone de saisie par langue"
[/multi],
  "noticeEmpty":
[multi]
[en]"empty @langNative text"
[fr]"texte @langNative vide"
[/multi],
  "ok":
[multi]
[en]"Ok"
[fr]"Ok"
[/multi],
  "redoText":
[multi]
[en]"Redo"
[fr]"Refaire"
[/multi],
  "redoTitle":
[multi]
[en]"Redo next change"
[fr]"Refaire la modification suivante"
[/multi],
  "undoText":
[multi]
[en]"Undo"
[fr]"Défaire"
[/multi],
  "undoTitle":
[multi]
[en]"Restore previous content"
[fr]"Restaurer le contenu précédent"
[/multi],
  "useLang":
[multi]
[en]"Choose which language is to be assigned to the text already entered."
[fr]"Choisissez quelle langue doit être affectée au texte déjà saisi."
[/multi],
  "useText":
[multi]
[en]"Use MMS template"
[fr]"Utiliser le gabarit MMS"
[/multi],
  "useTitle":
[multi]
[en]"Format this area depending on currently defined languages"
[fr]"Formater cette zone en fonction des langues actuellement définies"
[/multi],
  "widgetTitle":
[multi]
[en]"Strike {Esc} to cancel, strike {Ctrl-Enter} or click anywhere else (including on Submit) to validate"
[fr]"Tapez {Echap} pour abandonner, tapez {Ctrl-Entrée} ou cliquez n'importe où ailleurs (y compris sur Enregistrer) pour valider"
[/multi]
}
<?php
  return trim(_mms_process(
    ob_get_clean(), ['can_replace' => FALSE, 'can_highlight' => FALSE]
  ));
}
/* ================================================================= CKEDITOR */
/**
 * Informative text displayed in CKEditor config plugins list.
 */
function _mms_ckeditor_plugin_data() {
  module_load_include('inc', 'mms', 'mms.core');
  ob_start();
?>
[multi]
[en]MMS - Plugin to embed multiple languages translations in the same 
field
[fr]MMS - Un plugin pour intégrer des traductions en plusieurs langues 
dans le même champ
[/multi]
<?php
  return trim(_mms_process(ob_get_clean()));
}
/* ===================================================================== HELP */
/**
 * Complete documentation.
 */
function _mms_help_data() {
  module_load_include('inc', 'mms', 'mms.core');
  ob_start();
?>
[multi]
[en]
<h3>
  MMS help
</h3>
<p>
  This module is intended to allow entering all desired translations of a given
  content inside of a unique node, instead of having to create a separate node
  (with <em>Multilingual Content</em>) or a distinct field version (with
  <em>Entity Translation</em>) for each translation.
</p>
<p>
  The benefits of using this method are mainly:
  <ul>
    <li>when entering new content or updating an existing one, authors keep a
    total visibility upon all the translations at the same time</li>
    <li>a direct consequence is to reduce the risk of omitting to update the
    translations when an original content has been modified</li>
    <li>when a content includes a lot of lang-insensitive data (like images,
    links, or simply verbous HTML with a number of attributes), these parts
    don't have to be duplicated: only the textual parts must be entered as
    "multi" and rewritten in the different languages</li>
    <li>since it emulates the SPIP <code>&lt;&nbsp;multi&nbsp;&gt;</code>
    syntax, this method is nicely suitable to allow the direct migration of
    contents from this CMS, without the need of a painful data restructuration
  </li>
    <li>moreover, in the latter case, authors can maintain their previous work
    habits unchanged</li>
  </ul>
</p>
<section>
<h4 class="mms-toggler" id="install">
  Installation/Configuration
</h4>
<p>
  <em>MMS</em> works without the need of other translation module,
  assumed you have set the "Internationalization" option of <em>i18n</em>, and
  configured the desired languages. Nevertheless you can simultaneously use
  alternative translation methods, like <em>Multilingual Content</em>,
  <em>Menu translation</em> etc.
</p>
<p>
  By itself, the module contains no configuration option. By cons, to use the
  "multi" syntax you must:
  <ul>
    <li>
      In "Administer > Configuration > Regional and language > Languages",
      enable the "URL" detection method, and configure it to "Path prefix"</li>
    <li>
      For each enabled language, ensure to provide the corresponding standard
      "Path prefix language code" (don't leave it empty)</li>
    <li>
      Position the <em>Language switcher (User interface text)</em> block in a
      region, anywhere you like</li>
  </ul>
</p>
<p>
  If you're also using other options of the <em>i18n</em> module, there are
  some additional configuration options you must pay attention to:
  <ul>
    <li>
      for each node where the "multi" syntax is used, you MUST select the
      <strong>Neutral language</strong> in the "Language" field (or the lang
      switcher would deactivate any lang other than the specified one)</li>
    <li>
      for each block where the the "multi" syntax is used, you MUST
      <strong>keep unchecked all the proposed languages</strong> in the
      "Languages" part of "Visibility settings"</li>
  </ul>
</p>
</section><section>
<h4 class="mms-toggler" id="syntax">
  Syntax
</h4>
<p>
  NOTE: this is the raw internal structure which makes the <em>MMS</em>
  mechanism to work, and was initially designed to easily migrate data from
  the <em>SPIP</em> CMS.<br />
  As of 7.x-2.0 you can get totally rid of directly managing the "multi"
  syntax (look at <strong>User interface</strong> below). However you can
  continue to use whenever you want.
</p>
<p>
  When entering data in a text area, you may insert multilingual pieces of
  text by enclosing them between <code>&#91;&nbsp;multi&nbsp;&#93;</code> and
  <code>&#91;&nbsp;/multi&nbsp;&#93;</code> tags.<br />
  Such a piece of text is a "multi" <em>segment</em>, which may contain a
  number of "multi" <em>blocks</em>, each representing the desired content
  translated in a given language, like
  <code>[language&#x2011;mark]&hellip;content&hellip;</code>, where:
  <ul>
    <li>
      <code>language&#x2011;mark</code> is the involved language code, such as
      <code>en</code>, <code>fr</code>, <code>es</code>&hellip;</li>
    <li>
      <code>&hellip;content&hellip;</code> is the translated content</li>
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
  Any spaces, newlines, line-breaks or paragraph-breaks are ignored when they
  are located just after the opening tags, juste before the closing tags, and
  just around the language-marks. In other words, each text block is rendered
  trimmed.
</p>
</section><section>
<h4 class="mms-toggler" id="user-interface">
  User interface
</h4>
<p>
  While <em>MMS</em> fundamental work is to interpret the "multi" parts
  of text found to render it in the current language when displaying public
  pages, it also manages how these parts are presented to the user when it is
  creating or updating nodes or blocks contents.<br />
  Obviously at this moment the user must be able to look at all the available
  different translations of the same text: this is primarily achieved by
  merely displaying the raw "multi" syntax, as explained above, but it often
  may be a painful job to navigate through a lot of &#91;&nbsp;multi&nbsp;&#93;
  and &#91;&nbsp;lang&nbsp;&#93; tags.
</p>
<p>
  To allow a more friendly way to manage this, <em>MMS</em> includes
  different UI facilities, depending on which input type is involved:
  <ul>
    <li>
      <strong>"long-text" fields</strong> (such as body), which represent 
      quantitatively the largest part of text and are input through
      <code>&lt; textarea &gt;</code> elements, may contain a lot of (even
      complex) HTML structures: it's why they're usually processed through a
      wysiwyg editor.<br />
      So for them <em>MMS</em> includes a <em>CKEditor</em> plugin that
      can be activated for any text format (see
      <strong>Using with <em>CKEditor</em></strong> below).
    </li>
    <li>
      <strong>all other text fields</strong> (including "text", "link" and so
      on) are presented by Drupal through simple
      <code>&lt; input type="text" &gt;</code> elements.<br />
      As of 7.x-2.0, <em>MMS</em> works as follows:
      <ol>
        <li>
          when the element content is already using "multi" syntax, with
          language blocks for each of the currently defined languages,
          attaches a widget (activated on focus) which displays each language
          part in a dedicated cell.
        </li>
        <li>
          when the element contains only raw text, or is already using "multi"
          syntax but with a limited set of languages, offers a "Use MMS
          template" link, which on click:
          <ul>
            <li>
              automatically normalizes the element content, i.e. formats it 
              with the complete set of currently defined languages (if only raw
              text was present, it is affected to the default site language)
            </li>
            <li>attaches widget as explained above</li>
            <li>offers a "Back" link to restaure the original content</li>
          </ul>
        </li>
      </ol>
      This way the user remains free to normalize input or not (may be keeping
      only a limited set of defined languages), while he has an easy way to
      update when a new language is added to the site.
    </li>
    <li>
      by extension this behaviour is also available for texts which are not 
      really fields, such as <strong>field labels</strong> and <strong>node or 
      block title</strong>.
    </li>
  </ul>
</p>
</section><section>
<h4 class="mms-toggler" id="ckeditor">
  Using with <em>CKEditor</em>
</h4>
<p>
  With the <em>CKEditor</em> module, you can benefit from improved input method
  which gets you rid of the above syntax and automatically offers input areas
  dedicated to each of the languages defined for the site.<br />
  For this to work:
  <ul>
    <li>
      your version of <em>CKEditor</em> must include the <em>Widget</em> plugin
      (you may install it from the <a href="http://ckeditor.com/builder">
      CKEditor builder</a>)</li>
    <li>
      in the <em>CKEditor</em> configuration, for each profile where you want
      to allow it, in the "EDITOR APPEARANCE" group:
      <ol>
        <li>
          in the "Tools bar" section, add the "MMS" button to the tools
          bar</li>
        <li>
          in the "Plugins" section, check "MMS" in the list of plugins
          to be activated</li>
      </ol>
    </li>
    <li>
      then with this profile any text field part where the "multi" syntax is
      used automatically displays a "MMS" group, with a subgroup inside
      for each defined language</li>
    <li>
      at any time you may click the "MMS" button in the tools bar to
      create a new empty "MMS" group</li>
    <li>
      note that only text fields (or blocks text) can be entered through
      <em>CKEDITOR</em>: in views, and in nodes or blocks titles, you still
      must use the "multi" syntax, or the improved interface offered by the
      integrated widget (as of 7.x-2.0)</li>
</p>
</section><section>
<h4 class="mms-toggler" id="pathauto">
  Using with <em>Pathauto</em>
</h4>
<p>
  With the <em>Pathauto</em> module, if you have introduced "multi" syntax in
  the node titles, you may use the
  <code>[node:mms&#x2011;native&#x2011;title]</code> token to generate URL
  aliases, which will be localized using the <strong>site default
  language</strong>.
  <br />
  WARNING: using the <code>[node:title]</code> token would generate aliases
  from the <strong>raw</strong> title, resulting in something like
  <code>multienmy&#x2011;titlefrmon&#x2011;titremulti</code>!
</p>
</section><section>
<h4 class="mms-toggler" id="views">
  Using with <em>Views</em>
</h4>
<p>
  With the <em>Views</em> module, you can use the "multi" syntax inside of the
  texts you enter in the definition forms of a view.<br />
  They will be rendered like explained above, in the views summary, in the
  previews, and of course in the pages where they are included.
</p>
</section><section>
<h4 class="mms-toggler" id="tips">
  Tips
</h4>
<ol>
  <li>
    Which language code is used to render depends on the lang part of the
    current URL (such as "en" in <code>http://example.com/en/&hellip;</code>),
    which is generally defined by how the lang switcher is currently set. If no 
    language is currently defined (so Drupal language is empty), the site lang 
    is used.<br />
    As a fortunate side effect, at any moment you may deactivate the lang
    switcher, and all contents including the "multi" syntax are simply rendered
    in the site lang.<br />
    CAUTION, though: at the time this document is written (Drupal 7.34),
    deactivate the lang switcher seems to drop already registered vocabulary
    terms translations!</li>
  <li>
    If a "multi" segment does not contain translation for the current lang, the
    available text in the site lang will be rendered instead.</li>
  <li>
    You may use the "multi" syntax not only in any text field or block body, but
    also in the node or block title, and in field labels.</li>
  <li>
    In order to allow a simple migration of contents from the Spip CMS, an
    alternative syntax is also accepted, using HTML-fashion tags such as
    <code>&lt;&nbsp;multi&nbsp;&gt;</code> and
    <code>&lt;&nbsp;/multi&nbsp;&gt;</code>, rather than
    <code>&#91;&nbsp;multi&nbsp;&#93;</code> and
    <code>&#91;&nbsp;/multi&nbsp;&#93;</code> (but language marks keep using
    square brackets, like in <code>&#91; en  &#93;</code>).<br />
    You may also use this syntax when manually entering text (using plain text
    editor).</li>
  <li>
    Becareful when directly importing data from an external source: due to the
    internal rules of the <em>HTML purifier</em> plugin, <em>CKEditor</em>
    breaks the <em>MMS</em>'s automatic recognition of language blocks
    when they include <code>&lt; div &gt;</code> tags. You may consider to
    replace them by <code>&lt; section &gt;</code> tags.<br />
    Another solution could be to deactivate the <em>HTML purifier</em> plugin,
    so the &lt;div&gt; tags can be used, but this is not recommended, for
    general security reasons.</li>
</ol>
</section><section>
<h4 class="mms-toggler" id="localization">
  Localization
</h4>
<p>
  Because of its mission to integrate multiple languages in the same container,
  the <em>MMS</em> module does not conform to the standard Drupal
   localization system: in this domain, it is self-sufficient and embeds all
   of its own translations, written with the "multi" syntax.<br />
   All these translations (including the text you are reading now) are
   gathered in the file <code>mms.data.inc</code>.
</p>
<p>
  To add a language, you just have to extend this file, in which the various
  "multi" segments required are divided into a few number of functions,
  standardized according to the following scheme:
  <blockquote class="mms-code"><code>
    function _mms_FUNCTION() {<br />
    &nbsp;&nbsp;module_load_include('inc', 'mms', 'mms.core');<br />
    &nbsp;&nbsp;ob_start();<br />
    ?&gt;<br />
    <strong>
    &#91;&nbsp;multi&nbsp;&#93;<br />
    &#91;&nbsp;en&nbsp;&#93; This is an english text<br />
    &#91;&nbsp;fr&nbsp;&#93; Ceci est un texte en fran&ccedil;ais<br />
    &#91;&nbsp;/multi&nbsp;&#93;<br />
    </strong>
    &lt;?php<br />
    &nbsp;&nbsp;return _mms_process(ob_get_clean());<br />
    }
  </code></blockquote>
</p>
<p>
  In the example above, to add a Spanish translation will require you insert it
  in the "multi" segment, which will become:
  <blockquote class="mms-code"><code>
    <strong>
    &#91;&nbsp;multi&nbsp;&#93;<br />
    &#91;&nbsp;en&nbsp;&#93; This is an english text<br />
    &#91;&nbsp;fr&nbsp;&#93; Ceci est un texte en fran&ccedil;ais<br />
    &#91;&nbsp;es&nbsp;&#93; Este es un texto en español<br />
    &#91;&nbsp;/multi&nbsp;&#93;<br />
    </strong>
  </code></blockquote>
</p>
<p>
  The <em>_mms_token_info()</em> function is an exception to the above
  diagram, because the text it contains is distributed in a
  two-dimensional array, where each element is a "multi" segment. It is within 
  these that you will insert the new translations.
</p>
<p>
  <em>NOTE FOR ENGLISH SPEAKERS: the creator of this module is not 
  fluent in English, so he is unsure of the quality of writing. <b>All 
  corrections and improvements are welcome.</b></em>
</p>
</section>
[fr]
<h3>
  Aide de MMS
</h3>
<p>
  Ce module est destiné à permettre la saisie de toutes les traductions 
  souhaitées, pour un texte donné, au sein d'un noeud unique, au lieu de 
  devoir créer pour chaque traduction un nouveau noeud (avec
  <em>Multilingual Content</em>) ou une version séparée de champ (avec
  <em>Entity Translation</em>).
</p>
<p>
  Cette méthode présente plusieurs avantages, notamment :
  <ul>
    <li>
      lorsqu'il saisit ou modifie un contenu, l'auteur conserve à tout moment 
      une vision globale sur toutes les traductions existantes</li>
    <li>
      consequence directe, on réduit le risque d'oublier de mettre à jour une 
      traduction lorsque le contenu original est modifié</li>
    <li>
      lorsqu'un contenu comporte une part de données qui ne changent pas avec 
      la langue (des images, des liens, ou simplement du HTML avec de nombreux 
      attributs), ces données n'ont pas besoin d'être dupliquées : seules les 
      parties textuelles doivent être saisies comme "multi" et ré-écrites dans
      les différentes langues</li>
    <li>
      parce qu'elle émule la syntaxe SPIP
      <code>&lt;&nbsp;multi&nbsp;&gt;</code>, cette méthode est particulièrement
      adaptée pour une migration directe des contenus issus de ce CMS, évitant
      ainsi un pénible travail de restructuration</li>
    <li>
      en outre, dans ce cas, les auteurs peuvent conserver leurs habitudes de 
      travail antérieures</li>
  </ul>
</p>
<section>
<h4 class="mms-toggler" id="install">
  Installation/Configuration
</h4>
<p>
  <em>MMS</em> fonctionne sans recourir à un autre module voué aux 
  traductions, dès l'instant où vous avez activé l'option
  "Internationalisation" de <em>i18n</em>, et configuré les langages souhaités.
  Cependant vous pouvez tout de même utiliser simultanément une ou plusieurs
  méthodes de traduction alternatives, comme <em>Multilingual Content</em>,
  <em>Menu translation</em> etc.
</p>
<p>
  Par lui-même, le module ne nécessite aucune configuration. Par contre, pour
  utiliser la syntaxe "multi" vous devrez :
  <ul>
    <li>
      dans "Administration > Configuration > Regionalisation et langue > 
      Langues", activer la méthode de détection "URL" et la configurer pour 
      utiliser "Préfixe de chemin"</li>
    <li>
      pour chaque langue activée, vous assurer d'avoir bien saisi le code
      standard correspondant dans la zone "Code de langue du préfixe de 
      chemin" (ne la laissez pas vide)</li>
    <li>
      placer le bloc <em>Sélecteur de langue (Texte de l'interface utilisateur)
      </em> dans une région de votre choix</li>
  </ul>
</p>
<p>
  Si vous utilisez également d'autres options du module <em>i18n</em>, vous
  devrez également prêter attention aux options de configuration suivantes :
  <ul>
    <li>
      pour chaque noeud dans lequel la syntaxe "multi" est utilisée, vous DEVEZ
      sélectionner <strong>Independant de la langue</strong> dans le champ
      "Langue" (sinon le sélecteur de langue désactiverait toutes les langues
      autres que celle spécifiée)</li>
    <li>
      pour chaque bloc dans lequel la syntaxe "multi" est utilisée, vous DEVEZ
      <strong>ne sélectionner aucune des langues proposées</strong> dans la
      partie "Langues" du groupe "Paramètres de visibilité"</li>
  </ul>
</p>
</section><section>
<h4 class="mms-toggler" id="syntax">
  Syntaxe
</h4>
<p>
  NOTA: ceci est la structure interne qui permet à <em>MMS</em> de
  fonctionner, initialement définie pour permettre une migration facile de
  données issues du CMS <em>SPIP</em>.<br />
  A partir de la version 7.x-2.0 vous pouvez totalement éviter de manipuler
  directement la syntaxe "multi" (voir <strong>Interface utilisateur</strong>
  ci-après). Cependant vous pouvez continuer à l'utiliser à votre convenance.
</p>
<p>
  Lorsque vous saisissez des données dans un champ de type texte, vous pouvez 
  insérer des portions de texte multilingues en les enfermant entre des 
  balises <code>&#91;&nbsp;multi&nbsp;&#93;</code> et
  <code>&#91;&nbsp;/multi&nbsp;&#93;</code>.<br />
  Une telle portion de texte est un <em>segment</em> "multi", qui peut
  contenir un certain nombre de <em>blocs</em> "multi", chacun représentant le
  contenu souhaité, traduit dans une langue donnée, sous la forme
  <code>[marqueur&#x2011;de&#x2011;langue]&hellip;contenu&hellip;</code>, où :
  <ul>
    <li>
      <code>marqueur&#x2011;de&#x2011;langue</code> est le code de langue
      concerné, par exemple <code>en</code>, <code>fr</code>,
      <code>es</code>&hellip;</li>
    <li>
      <code>&hellip;contenu&hellip;</code> est le contenu traduit</li>
  </ul>
</p>
<p>
  Exemple:<br />
  <blockquote class="mms-code"><code>
    &#91;&nbsp;multi&nbsp;&#93;<br />
    &#91;&nbsp;en&nbsp;&#93; This is an english text<br />
    &#91;&nbsp;fr&nbsp;&#93; Ceci est un texte en fran&ccedil;ais<br />
    &#91;&nbsp;/multi&nbsp;&#93;
  </code></blockquote>
</p>
<p>
  Tous les espaces, caractères "nouvelle ligne", sauts de ligne ou sauts de 
  paragraphe HTML sont ignorés lorsqu'il se trouvent juste après les balises 
  ouvrantes, juste avant les balises fermantes, et précisément autour des 
  marques de code de langue. En d'autres termes, chaque bloc de texte est 
  rendu "nettoyé".
</p>
</section><section>
<h4 class="mms-toggler" id="user-interface">
  Interface utilisateur
</h4>
<p>
  Outre son travail fondamental (interprêter les séquences de texte "multi"
  rencontrées, pour les rendre dans la langue actuelle lors de l'affichage des
  pages publiques), <em>MMS</em> gère aussi la façon dont ces textes sont
  présentés à l'utilisateur lorsqu'il crée ou met à jour le contenu des noeuds
  ou des blocs.<br />
  Bien entendu, à ce moment l'utilisateur doit être en mesure de voir l'ensemble
  des différentes traductions du même texte : c'est ce qui est assuré de façon
  primaire par l'affichage du contenu brut de la syntaxe "multi", comme expliqué
  ci-avant, mais il peut souvent être pénible de devoir naviguer à travers les
  balises &#91;&nbsp;multi&nbsp;&#93; et &#91;&nbsp;lang&nbsp;&#93;.
</p>
<p>
  Pour rendre cette tâche plus confortable, l'interface utilisateur de
  <em>MMS</em> comporte différentes facilités, selon le type d'entrée
  concerné :
  <ul>
    <li>
      <strong>les champs de type "long-text"</strong> (comme "body"), qui
      représentent quantitativement la plus grosse part de texte et sont saisis
      par l'intermédiaire d'éléments <cdoe>&lt; textarea &gt;</code>, peuvent
      contenir quantité de structures HTML, y compris complexes : c'est pourquoi
      ils sont généralement traités à l'aide d'un éditeur wysiwyg.<br />
      Aussi, pour ce qui les concerne, <em>MMS</em> comporte un plugin
      <em>CKEditor</em> qui peut être activé pour n'importe quel format de texte
      (voir <strong>Utilisation avec <em>CKEditor</em></strong> ci-après).
    </li>
    <li>
      <strong>tous les autres champs texte</strong> (incluant "text", "link"
      etc) sont présentés par Drupal par l'intermédiaire de simples éléments
      <code>&lt; input type="text" &gt;</code>.<br />
      A partir de la version 7.x-2.0, <em>MMS</em> fonctionne comme suit :
      <ol>
        <li>
          lorsque le contenu de l'élément utilise déjà la syntaxe "multi", avec
          un bloc de langue pour chacun des langages actuellement définis, il
          attache un widget (activé à la prise du focus) qui affiche chaque
          langage dans une cellule dédiée.
        </li>
        <li>
          lorsque l'élément ne contient que du texte brut, ou bien utilise déjà
          la syntaxe "multi" mais avec un ensemble limité de langages, il
          propose un lien "Utiliser le gabarit MMS", qui au clic :
          <ul>
            <li>
              normalise automatiquement le contenu de l'élément, c'est-à-dire le
              formate avec un jeu complet des langages actuellement définis
              (si du texte brut seul était présent, il est affecté au langage
              par défaut du site)
            </li>
            <li>attache un widget comme expliqué ci-dessus</li>
            <li>propose un lien "Annuler" pour restaurer le contenu original
            </li>
          </ul>
        </li>
      </ol>
      Ainsi l'utilisateur reste libre de normaliser ou non la zone d'entrée
      (peut-être en ne conservant qu'un jeu limité de langages), tout en
      disposant d'un moyen facile de mise à jour lorsqu'un nouveau langage est
      ajouté au site.
    </li>
    <li>
      par extension, ce comportement est également disponible pour des textes 
      qui ne sont pas réellement des champs, comme les
      <strong>étiquettes de champ</strong> et les
      <strong>titres de noeuds et de blocs</strong>.
    </li>
  </ul>
</p>
</section><section>
<h4 class="mms-toggler" id="ckeditor">
  Utilisation avec <em>CKEditor</em>
</h4>
<p>
  Avec le module <em>CKEditor</em>, vous pouvez bénéficier d'un mode de saisie
  amélioré qui fait abstraction de la syntaxe ci-dessus et vous propose
  automatiquement des zones de saisie dédiées à chacune des langues définies
  pour le site.<br />
  Pour utiliser cette possibilité :
  <ul>
    <li>
      votre version de <em>CKEditor</em> doit inclure le plugin <em>Widget</em>
      (vous pouvez l'installer à partir du
      <a href="http://ckeditor.com/builder">constructeur <em>CKEditor</em></a>)
      </li>
    <li>
      dans la configuration de <em>CKEditor</em>, pour chaque profil dans
      lequel vous voulez l'autoriser, dans le groupe "APPARENCE DE L'EDITEUR" :
      <ol>
        <li>
          dans la section "Barre d'outils", ajoutez le bouton "MMS" à
          la barre d'outils</li>
        <li>
          dans la section "Plugins", cochez "MMS" dans la liste des
          plugins à activer</li>
      </ol>
    </li>
    <li>
      désormais, dans ce profil, toute partie de champ texte utilisant la
      syntaxe "multi" fait automatiquement apparaître un groupe "MMS",
      à l'intérieur duquel un sous-groupe est affecté à chaque langue
      définie</li>
    <li>
      à tout moment, vous pouvez cliquer sur le bouton "MMS" de la barre
      d'outils pour créer un nouveau groupe "MMS" vide</li>
    <li>
      notez que seuls les champs texte (ou le texte des blocs) peuvent être
      saisis sous <em>CKEDITOR</em> : dans les vues, et dans les titres de
      noeuds ou de blocs, vous devrez toujours utiliser la syntaxe "multi",
      ou l'interface améliorée proposée par le widget intégré (à partir de la
      version 7.x-2.0)</li>
  </ul>
</p>
</section><section>
<h4 class="mms-toggler" id="pathauto">
  Utilisation avec <em>Pathauto</em>
</h4>
<p>
  Avec le module <em>Pathauto</em>, si vous avez introduit la syntaxe "multi"
  dans le titre des noeuds, vous pouvez utiliser le jeton
  <code>[node:mms&#x2011;native&#x2011;title]</code> pour générer des
  alias d'URL, qui seront localisés dans la
  <strong>langue par défaut du site</strong>.<br />
  ATTENTION : utiliser le jeton <code>[node:title]</code> générerait les alias
  à partir du <strong>texte brut</strong> du titre, pour donner quelque chose
  comme <code>multienmy&#x2011;titlefrmon&#x2011;titremulti</code> !
</p>
</section><section>
<h4 class="mms-toggler" id="views">
  Utilisation avec <em>Views</em>
</h4>
<p>
  Avec le module <em>Views</em>, vous pouvez utiliser la syntaxe "multi" au
  sein des textes saisis dans les formulaires de définition d'une vue.<br />
  Ils seront rendus selon les règles exposées ci-dessus, dans le sommaire des
  vues, dans les aperçus, et bien sûr dans les pages où elles sont incluses.
</p>
</section><section>
<h4 class="mms-toggler" id="tips">
  Conseils
</h4>
<ol>
  <li>
    Le code de langue choisi par le filtre pour extraire le texte dépend de la
    partie "langue" de l'URL actuelle (par exemple "fr" dans
    <code>http://exemple.com/fr/&hellip;</code>), qui est généralement définie
    par la situation actuelle du sélecteur de langue. Si aucune langue n'est 
    actuellement définie (la langue actuelle pour Drupal est donc vide), c'est
    la langue du site qui est utilisée.<br />
    Cet "effet de bord" intéressant vous permet à tout moment de désactiver le 
    sélecteur de langue, de telle sorte que tous les contenus utilisant le
    module MMS seront simplement rendus dans la langue du site.<br />
    ATTENTION: au moment où ce document est rédigé (Drupal 7.34), il semble que
    la désactivation du sélecteur de langue provoque la perte des traductions
    des termes de taxonomie !</li>
  <li>
    Si un segment "multi" ne possède aucune traduction pour la langue actuelle,
    c'est le texte disponible dans la langue du site qui sera affiché.</li>
  <li>
    Vous pouvez utiliser la syntaxe "multi" dans n'importe quel champ texte ou
    corps de bloc, mais aussi dans le titre d'un noeud ou d'un bloc, ainsi que
    dans les étiquettes de champs.</li>
  <li>
    Pour une migration simplifiée de données en provenance du CMS SPIP, une 
    syntaxe alternative est également acceptée, utilisant des balises du type
    HTML comme <code>&lt;&nbsp;multi&nbsp;&gt;</code> et
    <code>&lt;&nbsp;/multi&nbsp;&gt;</code> à
    la place de <code>&#91;&nbsp;multi&nbsp;&#93;</code> et
    <code>&#91;&nbsp;/multi&nbsp;&#93;</code> (mais les blocs de langue
    emploient toujours les crochets, comme dans <code>
    &#91;&nbsp;fr&nbsp;&#93;</code>).<br />
    Vous pouvez aussi utiliser cette syntaxe alternative lorsque vous
    saisissez du texte manuellement (hors <em>CKEditor</em>).</li>
  <li>
    Attention lorsque vous importez directement des données depuis une source
    externe : les règles internes du plugin <em>HTML purifier</em> amènent
    <em>CKEditor</em> à interrompre la reconnaissance automatique des blocs de
    langue lorsqu'ils contiennent des balises <code>&lt; div &gt;</code>.
    Vous pouvez envisager de les remplacer par des balises
    <code>&lt; section &gt;</code> tags.<br />
    Un autre solution pourrait consister à désactiver le plugin
    <em>HTML purifier</em>, mais cela est déconseillé, pour des raisons
    générales de sécurité.</li>
</ol>
</section><section>
<h4 class="mms-toggler" id="localization">
  Localisation
</h4>
<p>
  Du fait de sa vocation à intégrer plusieurs langues dans un même conteneur,
  le module <em>MMS</em> ne se conforme pas à la norme Drupal en matière
  de localisation : sur ce plan, il est auto-suffisant et embarque la totalité
  de ses propres traductions, rédigées avec la syntaxe "multi".<br />
  Toutes ces traductions (y compris l'aide que vous lisez en ce moment) sont
  rassemblées dans le fichier <code>mms.data.inc</code>.
</p>
<p>
  Pour ajouter une langue, il suffit donc d'enrichir ce fichier, dans lequel
  les différents segments "multi" nécessaires sont répartis dans un petit nombre
  de fonctions, standardisées selon le schéma suivant :
  <blockquote class="mms-code"><code>
    function _mms_FUNCTION() {<br />
    &nbsp;&nbsp;module_load_include('inc', 'mms', 'mms.core');<br />
    &nbsp;&nbsp;ob_start();<br />
    ?&gt;<br />
    <strong>
    &#91;&nbsp;multi&nbsp;&#93;<br />
    &#91;&nbsp;en&nbsp;&#93; This is an english text<br />
    &#91;&nbsp;fr&nbsp;&#93; Ceci est un texte en fran&ccedil;ais<br />
    &#91;&nbsp;/multi&nbsp;&#93;<br />
    </strong>
    &lt;?php<br />
    &nbsp;&nbsp;return _mms_process(ob_get_clean());<br />
    }
  </code></blockquote>
</p>
<p>
  Dans l'exemple ci-dessus, pour ajouter une traduction en espagnol, il faudra
  l'insérer dans le segment "multi", qui deviendra :
  <blockquote class="mms-code"><code>
    <strong>
    &#91;&nbsp;multi&nbsp;&#93;<br />
    &#91;&nbsp;en&nbsp;&#93; This is an english text<br />
    &#91;&nbsp;fr&nbsp;&#93; Ceci est un texte en fran&ccedil;ais<br />
    &#91;&nbsp;es&nbsp;&#93; Este es un texto en español<br />
    &#91;&nbsp;/multi&nbsp;&#93;<br />
    </strong>
  </code></blockquote>
</p>
<p>
  La fonction <em>_mms_token_info()</em> constitue une exception au
  schema ci-dessus, car les textes qu'elle contient sont répartis dans un
  tableau à deux dimensions, où chaque élément est un segment "multi". C'est à 
  l'intérieur de ceux-ci qu'il faudra insérer les nouvelles traductions.
</p>
</section>
[/multi]
<section id="mms-alert">
[multi]
[en]<strong>CAUTION</strong>: all tags mentioned in this page show spaces 
between square brackets and the tag content, such as <strong>[ multi ]</strong>, 
<strong>[ /multi ]</strong>, <strong>[ en ]</strong>, etc. Here these spaces
are only to prevent tags to be functional in the current context.<br />
When directly entering text in "multi" mode, <strong>never insert spaces 
inside of the tag body</strong>.
[fr]<strong>ATTENTION</strong> : toutes les balises citées dans cette page
font apparaître des espaces entre les crochets et le contenu de la balise,
comme dans <strong>[ multi ]</strong>, <strong>[ /multi ]</strong>,
<strong>[ en ]</strong>, etc. Ces espaces ne figurent ici que pour empêcher les
balises d'être fonctionnelles dans ce contexte.<br />
Lorsque vous saisissez directement du texte en mode "multi", <strong>n'insérez
jamais d'espaces dans le corps des balises</strong>.
[/multi]
</section>
<script>
(function($) {
$(document).ready(function() {
  $('head').append('\
<style>\
.mms-toggler {\
  cursor: pointer;\
}\
.mms-toggler::before {\
  content: "♦";\
}\
.mms-toggler:hover {\
  background-color: #ddd;\
}\
code {\
  background-color: #e8e8e8;\
  margin: 0 .3em;\
}\
.mms-code {\
  margin: 1em;\
  border-left: 5px solid #ccc;\
  padding-left: 1em;\
}\
.mms-code * {\
  background-color: #fff;\
  margin: 0;\
}\
#mms-alert {\
  position: fixed;\
  right: 0;\
  top: 70px;\
  margin: .5em 1em;\
  border-radius: .5em;\
  box-shadow: .5em .5em .3em #888;\
  background-color: #fcc;\
  padding: 1em;\
  width: 45em;\
}\
</style>\
  ');
  // set toggle capabilities to each chapter:
  $('.mms-toggler').each(function() {
    $(this).siblings().hide();
    $(this).click(function() {
      $(this).siblings().toggle();
    });
  });
  // auto-expand a chapter if it is given as an anchor in the url:
  $(document.location.hash).siblings()
    .css({backgroundColor:'#ff0'}).show(1000, 'linear', function() {
      $(this).css({backgroundColor:'inherit'})
    });
});
})(jQuery);
</script>
<?php
  return _mms_process(ob_get_clean());
}
/* ========================================================================== */
