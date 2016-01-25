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
NOTE: if you are using <em>CKEditor</em>, look at !CKEditorHelpLink to install
the <em>CKEditor Widget</em> plugin.
[fr]Le module <em>MMS</em> a été installé avec succès.<br />
Vous pouvez désormais utiliser le widget MMS pour entrer des données
multilingues à l'intérieur des titres et champs de noeuds ou de blocs, ainsi
que dans toutes les autres zones de saisie de texte (taxonomie, menus, etc).
<br />
NOTA : si vous utilisez <em>CKEditor</em>, voyez la page !CKEditorHelpLink pour
installer le plugin <em>CKEditor Widget</em>.
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
will not be assured: we advise you to visit the MMS issues page at
!MMSIssuesLink to look for a solution and/or create a new issue.
[fr]Bien que le module <em>MMS</em> ait été installé avec succès, il
n\'a pas été possible d\'introduire les modifications nécessaires dans le
fichier includes/unicode.inc du coeur de Drupal.<br />
Dans ces conditions, MMS est globalement fonctionnel mais certaines
traductions ne seront pas assurées : nous vous conseillons de vous rendre sur
la page des problèmes MMS !MMSIssuesLink pour rechercher une solution et/ou
créer un rapport d'erreur.
[/multi]
<?php
  return _mms_process(ob_get_clean());
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
    'menu_reset_title' => '
[multi]
[en]Default values
[fr]Valeurs par défaut
[/multi]
    ',
    'menu_reset_description' => '
[multi]
[en]Reset some or all of the configuration options back to default values
[fr]Réinitialiser tout ou partie des options de configuration à leur valeur par
défaut
[/multi]
    ',
    'menu_uitranslate_title' => '
[multi]
[en]UI translations
[fr]Traductions de l\'IU
[/multi]
    ',
    'menu_uitranslate_description' => '
[multi]
[en]Export/import the MMS user interface translations to/from nodes where they
can be edited
[fr]Exporter/importer les traductions de l\'interface utiisateur MMS
vers/depuis des noeuds où elles peuvent être éditées
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
    // UIOPTIONS
    'uioptions_title' => '
[multi]
[en]User interface options
[fr]Options de l\'interface utilisateur
[/multi]
    ',
    // uioptions follow
    'uioptions_follow_title' => '
[multi]
[en]Language selection behaviour
[fr]Comportement de la sélection de langue
[/multi]
    ',
    'uioptions_follow_description' => '
[multi]
[en]Each entry area includes a local language selector for this area: hovering one language mark will display the corresponding content, click opens the input area in the chosen language.<br />
<em>WARNING: This option applies only to input fields; everything else in the page remains subject to the actual setting of the main language selector.</em>
[fr]Chaque zone de saisie comporte un sélecteur de langues local à cette zone : survoler une marque de langue provoque l\'affichage du contenu correspondant, cliquer ouvre la zone de saisie dans la langue choisie.<br />
<em>ATTENTION : cette option ne concerne que les zones de saisie ; tout le reste de la page reste soumis au réglage actuel du sélecteur de langue principal.</em>
[/multi]
    ',
    'follow_false' => '
[multi]
[en]selecting a language in an entry area applies only to this area
[fr]la sélection d\'une langue dans une zone de saisie s\'applique uniquement à cette zone
[/multi]
    ',
    'follow_true' => '
[multi]
[en]selecting a language in any entry area propagates this selection to all entry areas of the same page
[fr]la sélection d\'une langue dans une zone de saisie propage cette sélection à toutes les autres zones de saisie de la même page
[/multi]
    ',
    // uioptions raw
    'uioptions_raw_title' => '
[multi]
[en]Plain text input possibility
[fr]Possibilité de saisie en texte brut
[/multi]
    ',
    'uioptions_raw_description' => '
[multi]
[en]By default each entry area is in the form of the MMS multilingual widget, which offers maximum comfort by hiding the underlying "multi" syntax. However it may be useful in some cases to provide direct access to plain text, especially when importing texts from a CMS such as <em>SPIP</em>.
[fr]Par défaut, chaque zone de saisie se présente sous la forme du widget multilingue MMS, qui offre le maximum de confort en cachant la syntaxe "multi" sous-jacente. Cependant il peut être utile dans certains cas d\'offrir un accès direct au texte brut, notamment lors de l\'import de textes issus d\'un CMS comme <em>SPIP</em>.
[/multi]
    ',
    'raw_false' => '
[multi]
[en]user interface proposes only MMS view
[fr]l\'interface utilisateur ne propose que la vue MMS
[/multi]
    ',
    'raw_true' => '
[multi]
[en]offer an option to toggle btween MMS and raw view
[fr]proposer une option de bascule entre la vue MMS et la vue en texte brut
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
[en]When a data entry area contains only raw text (i.e. without "multi" syntax), a link "Use MMS template" is proposed. Click on the link causes the field content to be turned into a "multi" segment, with distinct areas for each of the languages ​​currently defined.<br />
For example, if the languages ​​are "en" and "fr", an area containing<br />
&nbsp;&nbsp;<code>This is a plain text in English</code><br />
may be converted into<br />
&nbsp;&nbsp;<code>[ multi ] [ en ] This is a plain text in English [ fr ] ...French... [ /multi ]</code><br />
where "...French..." is a simple visual cue while no translation exists yet, and will not be reproduced on the localized page in that language.
<p>In contrast, when a data entry area contains a "multi" segment, a link "Drop MMS format" is proposed. Click on this link causes the content to return to plain text, where only a single translation is preserved.</p>
<p> The following options specify which language should be considered when performing these two operations.</p>
[fr]Lorsqu\'une zone de saise ne contient qu\'un texte brut (i.e. sans syntaxe "multi"), un lien "Utiliser le gabarit MMS" est proposé. Cliquer sur ce lien provoque la transformation du contenu de la zone en un segment "multi", comportant des zones distinctes pour chacune des langues actuellement définies.<br />
Par exemple, si les langues sont "fr" et "en", une zone contenant<br />
&nbsp;&nbsp;<code>Ceci est un texte brut en français</code><br />
pourra être transformée en<br />
&nbsp;&nbsp;<code>[ multi ] [ fr ] Ceci est un texte brut en français [ en
] ...English... [ /multi ]</code><br />
où "...English..." est un simple repère visuel, en l\'absence provisoire de traduction, et ne sera pas reproduit sur la page localisée dans cette langue.
<p>A l\'opposé, lorsqu\'une zone de saise contient un segment "multi", c\'est un lien "Abandonner le format MMS" qui est proposé. Cliquer sur ce lien provoque le retour à un texte brut, dans lequel une seule traduction est conservée.</p>
<p>Les options ci-après définissent quelle langue doit être considérée pour exécuter ces deux opérations.</p>
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
[en]When a user clicks "Drop MMS format", the only text preserved in the input area is the one corresponding to the language defined above.
[fr]Lorsqu\'un utilisateur clique sur "Abandonner le format MMS", le seul texte conservé dans la zone de saisie est celui correspondant à la langue définie ci-dessus.
[/multi]
    ',
    // multify fromlang
    'multify_fromlang_title' => '
[multi]
[en]Choose which language to affect to a raw text when creating a "multi" segment
[fr]Choisir quelle langue affecter à un texte brut lors de la création d\'un segment "multi"
[/multi]
    ',
    'multify_fromlang_description' => '
[multi]
[en]When a user clicks "Use MMS template" for an input area which contains only raw text (i.e. without "multi" syntax), this text is affected to the language defined above.
[fr]Lorsqu\'un utilisateur clique sur "Utiliser le gabarit MMS" pour une zone de saisie ne contenant que du texte brut (i.e. sans syntaxe "multi"), ce texte est affecté à la langue définie ci-dessus.
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
[fr]demander à l\'utilisateur de choisir une langue (toutes les langues définies seront proposées)
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
[en]By default the multilingual input widget is activated for all input areas: this is generally expected for most of the forms. At the opposite though some areas must remain free of this widget, e.g.:
<ul>
  <li>in the pages offered to visitors, the search area</li>
  <li>in node edit forms, entry areas for a date or number</li>
</ul>
<p><em>
NOTE: you are not forced to register ALL areas where the input widget is not relevant, since its presence will not necessarily imply it will be applied. When in doubt as to its usefulness in a particular area, or simply if you want to avoid too dense census, limit yourself to the most obvious cases.
</em></p>
[fr]Par défaut, le widget de saisie multilingue est activé pour toutes les zones de saisie : c\'est ce qui est généralement souhaitable dans la plupart des formulaires. Cependant certaines zones doivent au contraire ne pas offrir ce widget, par exemple :
<ul>
  <li>dans les pages présentées à l\'internaute, la zone de recherche</li>
  <li>dans les formulaires de mise à jour de noeuds, les zones de saisie d\'une date, ou d\'un nombre</li>
</ul>
<p><em>
NOTA : rien ne vous oblige à recenser TOUTES les zones où le widget de saisie est sans intérêt, puisque sa présence n\'implique pas forcément sa mise en oeuvre. Dans le doute quant à son utilité pour telle ou telle zone, ou simplement si vous souhaitez éviter un recensement trop touffu, limitez-vous aux cas les plus évidents.
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
[en]You may add CSS selectors to target areas where to not activate widget.<br />
Note that the same area may sometimes be differently indicated depending on which theme is used (provided default values target the <em>Seven</em> theme).
[fr]Vous pouvez ajouter des sélecteurs CSS pour cibler des zones où le widget ne doit pas être activé.<br />
Notez que la même zone peut parfois être désignée de façon différente selon le thème utilisé (les valeurs par défaut fournies ciblent le thème <em>Seven</em>).
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
[en]Options below define how MMS must react when it encounters a segment that has no translation in the current language.
<p><em>NOTE: "no translation" means that the desired block (i.e. the involved language mark, such as </em>[ en ]<em>, followed by a string), either:
  <ul>
    <li>contains something like </em>...nativeLanguage...<em> where  "nativeLanguage" is the name of the language in this language itself, such as </em>...English...<em><br />
    (this is the default content set by MMS to each new block of a "multi"    segment)</li>
    <li>or is merely empty (no string following </em>[ en ]<em>)</li>
    <li>or is downright missing (e.g. when a new language was just defined)</li>
  </ul>
</em></p>
[fr]Les options ci-après définissent comment MMS doit réagir lorsqu\'il rencontre un segment qui ne possède pas de traduction dans la langue actuelle.
<p><em>NOTA : "pas de traduction" signifie que le bloc recherché (i.e. la marque de langue concernée, par exemple </em>[ fr ]<em>, suivie d\'une chaîne de caractères) :
  <ul>
    <li>contient quelque chose de la forme </em>...langueNative...<em>, où "langueNative" est le nom de la langue dans cette langue elle-même, par  exemple </em>...Français...<em><br />
    (c\'est le contenu par défaut donné par MMS à tout nouveau bloc d\'un segment "multi")</li>
    <li>ou est simplement vide (pas de chaîne suivant </em>[ fr ]<em>)</li>
    <li>ou est carrément manquant (par exemple lorsqu\'une nouvelle langue vient d\'être définie)</li>
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
[en]NOTE: this option only regards <strong>really empty</strong> translations, i.e. where the language mark (e. g. </em>[ fr ]<em>) is present but followed by no text. It is useful to allow some texts to be outright "invisible" in the given language.
[fr]NOTA : cette option concerne uniquement les traductions <strong>réellement vides</strong>, c\'est-à-dire où la marque de langue (par exemple </em>[ en ]<em>) est bien présente mais n\'est suivie d\'aucun texte. Elle est utile pour autoriser qu\'un texte soit purement et simplement "invisible" dans la langue choisie.
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
[en]NOTE: in both cases the replacement text will be wrapped in a </em>&lt; span &gt;<em> element that you can customize using the "title" attribute and CSS properties below.
[fr]NOTA : dans les deux cas le texte de remplacement sera emballé dans un élément </em>&lt; span &gt;<em> que vous pouvez personnaliser à l\'aide de l\'attribut "title" et des propriétés CSS ci-dessous.
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
[en]WARNING: this message is itself a "multi" segment, and will be localized in the current language before displaying. <strong>But unlike the text it replaces, it MUST include translations in each of the defined languages</strong> (otherwise it will be replaced by " ? ? ? ").
[fr]ATTENTION : ce message est lui-même un segment "multi", et sera localisé dans la langue courante avant affichage. <strong>Mais contrairement au texte qu\'il remplace, il DOIT comporter une traduction dans chacune des langues définies</strong> (sinon il sera remplacé par " ? ? ? ").
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
[en]WARNING: this message is itself a "multi" segment, and will be localized in the current language before displaying. You may choose to leave it empty (in whole, or for one or more languages) to prevent displaying a tip.
[fr]ATTENTION : ce message est lui-même un segment "multi", et sera localisé dans la langue courante avant affichage. Vous pouvez choisir de le laisser vide (en totalité, ou pour une ou plusieurs langues) pour éviter l\'affichage d\'une bulle d\'aide.
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
[en]You may enter several CSS properties (once per line, the final ";" is optional). They will be affected to the &lt; span &gt; element wrapping the replacement text.
[fr]Vous pouvez saisir plusieurs propriétés CSS (une par ligne, le ";" final est facultatif). Elles seront affectées à l\'élément &lt; span &gt englobant le texte de remplacement.
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
[en]MMS is designed to look at any page to render texts localized, everywhere it encounters "multi" segments. However, to allow entering multilingual content, it must keep these pieces of text untouched when they are part of an input area offered to the user during the site administration process.
<p>This is achieved by instructing MMS to exclude some given pages from the global localization process (actually all pages which are forms including data entry areas like &lt; input &gt; or &lt; textarea &gt; elements) whose content may include "multi" segments.<br />
In those pages, all other elements are then automatically localized at the moment they are displayed.</p>
[fr]MMS est conçu pour inspecter toutes les pages, en vue de rendre les textes localisés chaque fois qu\'il rencontre des segments "multi". Cependant, pour autoriser la saisie de contenus multilingues, il doit conserver intactes ces portions de texte lorsqu\'elles font partie d\'une zone de saisie offerte à l\'utilisateur dans le cadre de l\'administration du site.
<p>Pour ce faire on donne pour instruction à MMS d\'exclure certaines pages du processus global de localisation (en fait, toutes les pages qui sont des formulaires comportant des zones de saisie comme des éléments &lt; input &gt; ou &lt; textarea &gt;) dont le contenu peut inclure des segments "multi".<br />
Au sein de ces pages, tous les autres éléments sont ensuite localisés automatiquement au moment où elles sont affichées.</p>
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
[en]You may use regular expressions to add paths of pages to exclude from the localization process.<br />
<strong>WARNING: don not modify the above default provided list unless you know full well what you are doing.</strong>
[fr]Vous pouvez utiliser des expressions régulières pour ajouter des chemins de pages à exclure du processus de localisation.<br />
<strong>ATTENTION : ne modifiez pas ci-dessus la liste fournie par défaut, sauf si vous savez pertinemment ce que vous faites.</strong>
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
[en]MMS allows to introduce line breaks when typing some short texts (like the title of a node or a menu item), where they are not normally allowed by Drupal. By default, these line breaks are simply rendered as spaces, as if they had not been entered. You can define in which location of the page you want them to be actually rendered as line breaks.
[fr]MMS permet d\'introduire des sauts de ligne lors de la saisie de certains textes courts (comme le titre d\'un noeud ou d\'une option de menu), là où ils ne sont normalement pas autorisés par Drupal. Par défaut, ces sauts de ligne sont simplement rendus en tant qu\'espaces, comme s\'ils n\'avaient pas été saisis.<br />
Vous pouvez définir dans quels emplacements de la page vous souhaitez qu\'ils soient effectivement rendus en tant que sauts de ligne.
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
[en]You may add CSS selectors to target areas where you want the line breaks are effective.<br />
Alternatively you can safely let this list empty: in this case existing line breaks in short texts will never turn effective.
<p><em>NOTE: inserting line breaks in places where they have not been provided for in the theme may cause distortions in the appearance of the page. In such a case you will have to change the theme CSS accordingly.</em></p>
[fr]Vous pouvez ajouter des sélecteurs CSS pour cibler les zones où vous souhaitez que les sauts de lignes soient effectifs.<br />
A l\'opposé, vous pouvez sans risque laisser cette liste vide : dans ce cas les sauts de ligne figurant dans les textes courts ne seront jamais effectifs.
<p><em>NOTA : l\'insertion de sauts de ligne dans des emplacements où ils n\'ont pas été prévus par le thème peut entraîner des déformations dans l\'aspect de la page. Dans un tel cas vous devrez modifier les CSS du thème en conséquence.
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
[en]Some of the existing tokens may be cloned so for an original <em>[type:token]</em> token resolving in <em>value</em>, two new tokens will be created:
<ul>
  <li><em>[type:mms-token]</em> resolving in current language localized <em>value</em></li>
  <li><em>[type:mms-native-token]</em> resolving in site default language localized <em>value</em> whatever the current language is</li>
</ul>
[fr]Certains des jetons existants peuvent être clônés de sorte que, pour un jeton original comme <em>[type:token]</em> se résolvant en <em>valeur</em>, deux nouveaux jetons seront créés :
<ul>
  <li><em>[type:mms-token]</em>, se résolvant comme <em>valeur</em> localisée dans la langue actuelle</li>
  <li><em>[type:mms-native-token]</em>, se résolvant comme <em>valeur</em> dans la langue du site par défaut, quelle que soit la langue actuelle</li>
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
[en]You may add tokens to the list above, in the form <em>[type:token]</em> or merely <em>type:token</em>.<br />
<strong>WARNING: here don\'t add tokens like <em>[node:field_...]</em> (see below).</strong>
[fr]Vous pouvez ajouter des jetons à la liste ci-dessus, sous la forme <em>[type:token]</em> ou simplement <em>type:token</em>.<br />
<strong>ATTENTION : n\'ajoutez pas ici de jetons du type <em>[node:field_...]</em> (voir ci-dessous).</strong>
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
[en]By default the Token module creates a token for each of the user- or   module-defined field. E.g. for a field whose machine-name is <em>myfield</em> it creates a token <em>[node:field_myfield]</em>.<br />
MMS can clone those field tokens as well as raw tokens. In the above example: <em>[node:mms-field_myfield]</em> and <em>[node:mms-native-field_myfield]</em>.
<p>Below you may define for which field types MMS must create clones. Check   or uncheck to allow/disallow each of those field types. <strong>You should only check field types which can contain textual data (strings). </strong>
</p>
[fr]
Par défaut, le module Token crée un jeton pour chacun des champs définis par   l\'utilisateur ou par un module. Par exemple, pour un champ de nom-système <em>myfield</em>, il crée un jeton <em>[node:field_myfield]</em>.<br />
MMS peut clôner ces jetons de champs, de la même façon que les jetons bruts. Dans l\'exemple ci-dessus : <em>[node:mms-field_myfield]</em> et <em>[node:mms-native-field_myfield]</em>.
<p>Vous pouvez définir ci-dessous pour quels types de champ MMS doit créer   des clônes. Cochez ou décochez pour autoriser/interdire chacun de ces types de champs. <strong>Vous ne devriez cocher que des types de champs pouvant contenir du texte (chaînes de caractères).</strong>
</p>
[/multi]
    ',
    // -------------------------------------------------mms_reset_form()
    // reset
    'reset_title' => '
[multi]
[en]Options to reset
[fr]Options à réinitialiser
[/multi]
    ',
    'reset_description' => '
<br />
[multi]
[en]Check the options you want to reset to their default values.
[fr]Cochez les options que vous voulez réinitialiser à leurs valeurs par défaut.
[/multi]
    ',
    'reset_submit_button' => '
[multi]
[en]Start reset
[fr]Lancer la réinitialisation
[/multi]
    ',
    'reset_submit_title' => '
[multi]
[en]Resetting MMS configuration
[fr]Réinitialisation de la configuration de MMS
[/multi]
    ',
    'reset_submit_init' => '
[multi]
[en]Reset
[fr]Réinitialisation
[/multi]
    ',
    'reset_submit_none' => '
[multi]
[en]No category to process!
[fr]Aucune catégorie à traiter !
[/multi]
    ',
    'reset_finished_ok' => '
[multi]
[en]MMS configuration reset for
[fr]Configuration de MMS réinitialisée pour
[/multi]
    ',
    'reset_finished_error' => '
[multi]
[en]An error happened. The following categories have not been reset:
[fr]Une erreur s\'est produite. Les catégories suivantes n\'ont pas été 
réinitialisées :
[/multi]
    ',
    // -------------------------------------------------mms_uitranslate_form()
    // operation
    'uitranslate_title' => '
[multi]
[en]User Interface translation
[fr]Traduction de l\'Interface Utilisateur
[/multi]
    ',
    'uitranslate_markup' => '
[multi]
[en]The tools below help you to create new translations for the MMS user
interface or to modify some of the existing translations:
<ol>
  <li>Launch the <strong>export</strong> operation to automatically create pages
  which contain the different components of the MMS user interface, each with
  its current set of translations.</li>
  <li>Edit these pages: with the MMS widget you can easily modify the existing
  translations, or add new translations.<br />
  <em>NOTE: to add a new translation, you must have previously defined the
  involved language at !languageAddLink.</em></li>
  <li>When you are satisfied of your changes, launch the <strong>import</strong>
  operation to update the MMS user interface from your new version.</li>
</ol>
[fr]Les outils ci-dessous vous assistent pour la création de nouvelles
traductions pour l\'interface utilisateur MMS, ou pour la modification de
traductions déjà existantes :
<ol>
  <li>Lancez l\'opération d\'<strong>export</strong> pour créer automatiquement
  des pages contenant les différents composants de l\'interface utilisateur MMS,
  chacun avec son jeu de traductions actuel.</li>
  <li>Editez ces pages : avec l\'aide du widget MMS vous pouvez facilement
  modifier les traductions existantes, ou créer de nouvelles traductions.<br />
  <em>NOTA : pour ajouter une nouvelle traduction, vous devez avoir
  préalablement défini la langue concernée, sur la page !languageAddLink.</em>
  </li>
  <li>Lorsque vous êtes satisfait de vos modifications, lancez l\'option
  d\'<strong>import</strong> pour mettre à jour l\'interface utilisateur MMS à
  partir de votre nouvelle version.</li>
</ol>
[/multi]
    ',
    'uitranslate_operation_title' => '
[multi]
[en]Choose the operation to execute
[fr]Choisissez l\'opération à exécuter
[/multi]
    ',
    /*
    'uitranslate_operation_description' => '
[multi]
[en]uitranslateoperation_description
[fr]uitranslateoperation_description
[/multi]
    ',
    */
    'uitranslate_export' => '
[multi]
[en]export the MMS user interface to dedicated (not published) pages where you
can add new translations or handle existing translations
[fr]exporter l\'interface utilisateur MMS vers des pages dédiées (non publiées)
dans lesquelles vous pourrez ajouter des traductions ou gérer les traductions
existantes
[/multi]<br />
    ',
    'uitranslate_export_advice' => '
<em>[multi]
[en]NOTE: this is currently the only available operation because there is
actually no dedicated page
[fr]NOTA : ceci est actuellement la seule opération disponible car il n\'existe
pour l\'instant aucune page dédiée
[/multi]</em>
    ',
    'uitranslate_export_warning' => '
<em>[multi]
[en]CAUTION: this will overwrite the current set of dedicated pages!
[fr]ATTENTION : ceci écrasera le jeu actuel de pages dédiées !
[/multi]</em>
    ',
    'uitranslate_show' => '
[multi]
[en]show the list of currently existing dedicated pages
[fr]montrer la liste des pages dédiées actuellement existantes
[/multi]
    ',
    'uitranslate_import' => '
[multi]
[en]import the MMS user interface from the dedicated pages.<br />
<em>CAUTION: this will overwrite the whole current UI!</em>
[fr]importer les traductions de l\'interface utilisateur MMS depuis les pages
dédiées<br />
<em>ATTENTION : ceci écrasera la totalité de l\'IU actuelle !</em>
[/multi]
    ',
    'uitranslate_submit_button' => '
[multi]
[en]Start execution
[fr]Lancer l\'exécution
[/multi]
    ',
    'uitranslate_submit_title' => '
[multi]
[en]Operations on UI translations
[fr]Opérations sur les traductions de l\'IU
[/multi]
    ',
    'uitranslate_submit_init' => '
[multi]
[en]Operation running: @operation.
[fr]Opération en cours : @operation.
[/multi]
    ',
    'uitranslate_submit_none' => '
[multi]
[en]No operation selected!
[fr]Aucune opération n\'a été sélectionnée !
[/multi]
    ',
    'uitranslate_finished_ok' => '
[multi]
[en]Operation achieved: @operation.
[fr]Opération terminée : @operation.
[/multi]
    ',
    'uitranslate_finished_error' => '
[multi]
[en]An error happened. Operation cancelled: @operation.
[fr]Une erreur s\'est produite. L\'opération est annulée : @operation.
[/multi]
    ',
    'uitranslate_pages_title' => '
[multi]
[en]Here is the list of the user interface dedicated pages. Click to edit (opens
a new tab).
[fr]Voici la liste des pages dédiées à l\'interface utilisateur. Cliquez pour
éditer (ouvre un nouvel onglet). 
[/multi]
    ',
    'uitranslate_pages_footer' => '
[multi]
[enCAUTION: within each page above:<br />
 - do NOT MODIFY the page title<br >
 - edit ONLY localized parts of text, and DO NOT CHANGE ANYTHING in the text that may appear above, below, or between them<br />
 - when the text includes HTML tags, scrupulously keep them by entering them like in a code editor<br />
 - do NOT INSERT LINE-BREAKS with {Enter} key: instead use the pseudo-tag < LF ><br />
 - ESCAPE QUOTES like this: \\\'<
[fr]ATTENTION : au sein de chaque page ci-dessus:<br />
  - NE MODIFIEZ PAS LE TITRE de la page<br />
  - éditez UNIQUEMENT les portions de texte localisées, et NE CHANGEZ RIEN au texte qui peut apparaître au-dessus, en-dessous ou entre elles<br />
  - lorsque le texte comporte des balises HTML, conservez-les scrupuleusement en les saisissant comme dans un éditeur de code<br />
 - N\'INSEREZ PAS DE RETOUR LIGNE avec la touche {Entrée} : utilisez la pseudo-balise < LF ><br />
  - ECHAPPEZ LES QUOTES comme ceci : \\\'
</ul>
[/multi]
    ',
    'uitranslate_pages_none' => '
[multi]
[en]The user interface dedicated pages are absent. Launch export operation to
create them.
[fr]Les pages dédiées à l\'interface utilisateur sont absentes. Lancez
l\'opération d\'export pour les créer.
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
/* ========================================================================== */
