<h3>
  Aide de MMS
</h3>
<p>
  Dans un environnement multilingue, MMS unifie la gestion de toutes les traductions de façon simultanée pour chaque bloc, noeud et tout contenu structurel sans utiliser de conteneurs distincts, et avec uniquement le module <em>Locale</em> du coeur de Drupal activé.</p>
<section> <!-- how -->
<h4 class="mms-toggler" id="how">
  Principe de fonctionnement
</h4>
<p>
  Brièvement résumé, MMS travaille avec tous les éléments <code>&lt;&nbsp;input type="text"&nbsp;&gt;</code> et <code>&lt;&nbsp;textarea&nbsp;&gt;</code>, en leur conférant la capacité à contenir le jeu complet de leur traductions.
</p>
<p>
  Dans le contexte de l'administration du site, chacune de ces zones est remplacée par un widget multilingue où l'utilisateur peut voir simultanément toutes les traductions du contenu, et les éditer à son gré. Un autre widget dédié est aussi disponible pour les zones gérées avec CKEditor.
</p>
<p>
  Cette méthode présente plusieurs avantages, notamment :
  <ul>
    <li>lorsqu'il saisit ou modifie un contenu, l'auteur conserve à tout moment une vision globale sur toutes les traductions existantes</li>
    <li>consequence directe, on réduit le risque d'oublier de mettre à jour une traduction lorsque le contenu original est modifié</li>
    <li>lorsqu'un contenu comporte une part de données qui ne changent pas avec la langue (des images, des liens, ou simplement du HTML avec de nombreux attributs), ces données n'ont pas besoin d'être dupliquées : seules les parties textuelles doivent être saisies comme "multi" et ré-écrites dans les différentes langues</li>
    <li>parce qu'elle émule la syntaxe SPIP <code>&lt;&nbsp;multi&nbsp;&gt;</code>, cette méthode est particulièrement adaptée pour une migration directe des contenus issus de ce CMS, évitant ainsi un pénible travail de restructuration</li>
    <li>en outre, dans ce cas, les auteurs peuvent conserver leurs habitudes de travail antérieures</li>
  </ul>
</p>
<p>
  L'intérêt de MMS est essentiellement que :
  <ul>
    <li>il gère tous les contenus sans nécessiter d'autre module dédié aux traductions</li>
    <li>il offre à l'utilisateur une interface unique pour visualiser et modifier toutes les traductions d'un contenu en même temps</li>
    <li>il consomme moins d'espace dans la base de données (moins de tables, moins de contenu dupliqué)</li>
  </ul>
</p>
<p>
  Par contre son utilisation présente certaines limitations:
  <ul>
    <li>il ne permet pas de gérer des noeuds séparés pour des langues distinctes</li>
    <li>dans sa version actuelle il réduit l'espace disponible pour les "petits" contenus (par exemple les titres), car la taille qui leur est allouée est partagée entre toutes les traductions<br />
    <em>NOTA : cette limitation sera levée dans une future version</em></li>
  </ul>
</p>
</section>
<section> <!-- install -->
<h4 class="mms-toggler" id="install">
  Installation/Configuration
</h4>
<p>
  MMS fonctionne sans recourir à un autre module voué aux traductions, dès l'instant où vous avez activé le module <em>Locale</em> du coeur de Drupal, et configuré les langages souhaités. Cependant vous pouvez tout de même utiliser simultanément une ou plusieurs méthodes de traduction alternatives, comme <em>Multilingual Content</em>, <em>Menu translation</em> etc.
</p>
<p>
  Le module est installé avec une configuration par défaut qui convient à la plupart des cas d'utilisation, et que vous pourrez régler finement selon vos souhaits (voir ci-après <a href="#advanced">Configuration avancée</a>). Pour utiliser immédiatement les possibilités offertes par MMS, vous devrez simplement :
  <ul>
    <li>dans "Administration > Configuration > Regionalisation et langue > Langues", activer la méthode de détection "URL" et la configurer pour utiliser "Préfixe de chemin"</li>
    <li>pour chaque langue activée, vous assurer d'avoir bien saisi le code standard correspondant dans la zone "Code de langue du préfixe de chemin" (ne la laissez pas vide)</li>
    <li>placer le bloc <em>Sélecteur de langue (Texte de l'interface utilisateur) </em> dans une région de votre choix</li>
  </ul>
</p>
<p>
  Si vous utilisez également des méthodes de traduction alternatives (options du module <em>i18n</em>), vous devrez également prêter attention aux points suivants :
  <ul>
    <li>pour chaque noeud dans lequel MMS est utilisé, vous DEVEZ sélectionner <strong>Independant de la langue</strong> dans le champ "Langue" (sinon le sélecteur de langue désactiverait toutes les langues autres que celle spécifiée)</li>
    <li>pour chaque bloc dans lequel MMS est utilisé est utilisé, vous DEVEZ <strong>ne sélectionner aucune des langues proposées</strong> dans la partie "Langues" du groupe "Paramètres de visibilité"</li>
  </ul>
</p>
</section>
<section> <!-- ui -->
<h4 class="mms-toggler" id="ui">
  Interface utilisateur
</h4>
<p>
  Outre son travail fondamental (interprêter les séquences de texte "multi" rencontrées, pour les rendre dans la langue actuelle lors de l'affichage des pages publiques), <em>MMS</em> gère aussi la façon dont ces textes sont présentés à l'utilisateur lorsqu'il crée ou met à jour le contenu des noeuds ou des blocs, leurs titres etc.<br />
  A ce moment l'utilisateur doit être en mesure de voir et modifier l'ensemble des différentes traductions du même texte : c'est ce qui est assuré par les widgets MMS.
  <ul>
    <li><strong>les champs de type "long-text"</strong> (comme "body"), qui représentent quantitativement la plus grosse part de texte et sont saisis par l'intermédiaire d'éléments <code>&lt; textarea &gt;</code>, peuvent contenir quantité de structures HTML, y compris complexes : c'est pourquoi ils sont généralement traités à l'aide d'un éditeur wysiwyg. Aussi, pour ce qui les concerne, <em>MMS</em> comporte un plugin <em>CKEditor</em> qui peut être activé pour n'importe quel format de texte (voir <a href="#ckeditor">Utilisation avec <em>CKEditor</em></a>).<br /></li>
      Si <em>CKEditor</em> n'est pas installé, ces champs bénéficient du widget MMS commun aux autres champs, décrit ci-dessous.
    <li><strong>tous les autres champs texte</strong> (incluant "text", "link" etc) sont présentés par Drupal par l'intermédiaire de simples éléments <code>&lt; input type="text" &gt;</code>. Dans ce cas :
      <ul>
        <li>lorsqu'un élément ne contient que du texte brut, un lien "Utiliser le gabarit MMS" est proposé. Lorsqu'on clique sur ce lien :
          <ul>
            <li>le contenu de l'élément est automatiquement normalisé, c'est-à-dire formaté avec un jeu complet des langues actuellement définies</li>
            <li>le texte original est affecté à la langue principale du site, ou à la langue actuellement sélectionnée, ou à une langue définie par l'utilisateur (voir <a href="#advanced-entry-multi">Processus de "Multification"</a>)</li>
            <li>le widget de visualisation/saisie est affiché comme décrit ci-dessous</li>
          </ul>
        </li>
        <li>lorsqu'un élément est déjà en format MMS, avec un bloc de langue pour chacune des langues actuellement définies, le widget est activé et offre l'affiche de chaque langage par survol, ainsi que la possibilité de saisir les différentes traductions dans des cellules dédiées. Un lien "Abandonner le format MMS" est disponible, qui permettra le cas échéant de revenir à un contenu monolingue</li>
        <li>lorsqu'un élément est déjà en format MMS, mais ne contient pas encore l'ensemble des langues définies (situation pour un texte pré-existant, lorsqu'une nouvelle langue vient d'être définie), un bloc pour la langue manquante est automatiquement ajouté</li>
        <li>chaque zone de saisie possède son propre historique des modifications, disponible pendant toute la durée de traitement du formulaire en cours : il est donc possible d'annuler/répéter les modifications, indépendamment pour chaque zone</li>
      </ul>
      Ainsi l'utilisateur reste libre de normaliser ou non la zone d'entrée (peut-être en ne conservant qu'un jeu limité de langages), tout en disposant d'un moyen facile de mise à jour lorsqu'un nouveau langage est ajouté au site.</li>
    <li>par extension, ce comportement est également disponible pour des textes qui ne sont pas réellement des champs, comme les <strong>étiquettes de champ</strong>, les <strong>titres de noeuds et de blocs</strong>, les <strong>liens de menus</strong>, les <strong>noms de vocabulaire</strong> et les <strong>termes de taxonomie</strong>, la définition des <strong>types de contenus</strong>...</li>
  </ul>
</p>
</section>
<section> <!-- advanced -->
<h4 class="mms-toggler" id="advanced">
  Configuration avancée
</h4>
<p>
  MMS propose un large éventail d'options de configuration, répartis en trois onglets consacrés respectivement à la saisie, au rendu, et à la gestion des jetons. Deux onglets supplémentaires permettent, l'un de revenir à la configuration par défaut pour tout ou partie des options, l'autre de gérer les traductions de l'interface utilisateur.
  <ul>
    <li>
      <h5 class="mms-toggler" id="advanced-entry">L'onglet SAISIE</h5>
      <ul>
        <li>
          <h6 class="mms-toggler" id="advanced-entry-iu">Groupe OPTIONS DE L'INTERFACE UTILISATEUR</h6>
          <ul>
            <li>Comportement de la sélection de langue.<br />
            Chaque zone de saisie comporte un sélecteur de langues local à cette zone : survoler une marque de langue provoque l'affichage du contenu correspondant, cliquer dessus ouvre la zone de saisie dans la langue choisie.<br />
            Vous pouvez définir si la sélection d'une langue s'applique uniquement à la zone actuelle, ou se propage à toutes les zones de saisie de la page (valeur par défaut)</li>
            <li>Possibilité de saisie en texte brut.<br />
            Par défaut, chaque zone de saisie se présente sous la forme du widget multilingue MMS, qui offre le maximum de confort en cachant la syntaxe "multi" sous-jacente. Cependant il peut être utile dans certains cas d'offrir un accès direct au texte brut, notamment lors de l'import de textes issus d'un CMS comme SPIP.<br />
            Dans le cas d'une zone de texte long gérée avec CKEditor, cette possibilité est nativement offerte (lien "Basculer en texte brut" ou bouton "Source"). Pour les autres zones, une option permet de définir si un bouton sera proposé pour basculer en mode accès direct, ou si le widget multilingue est seul disponible (valeur par défaut)</li>
          </ul>
        </li>
        <li>
          <h6 class="mms-toggler" id="advanced-entry-multi">Groupe PROCESSUS DE "MULTIFICATION"</h6>
          <p>Lorsqu'une zone ne contient qu'un texte en une seule langue (par exemple un texte déjà existant au moment de l'installation de MMS), le passage au multilingue nécessite de décider à quelle langue sera affecté le texte déjà existant. De même si l'on souhaite revenir à une langue unique à partir d'une zone déjà multilingue, il faut décider quelle langue sera conservée dans le texte monolingue.<br />
          Pour chacun de ces deux cas, vous pouvez définir si MMS utilisera automatiquement la langue principale du site (valeur par défaut), utilisera automatiquement la langue actuellement sélectionnée, ou demandera à l'utilisateur de choisir une langue</p>
        </li>
        <li>
          <h6 class="mms-toggler" id="advanced-entry-widget">Groupe ACTIVATION DU WIDGET DE SAISIE MULTILINGUE</h6>
          <p>Par défaut, le widget de saisie multilingue est activé pour toutes les zones de saisie : c'est ce qui est généralement souhaitable dans la plupart des formulaires. Cependant ce widget n'offre aucun intérêt dans certaines zones (par exemple une date ou un nombre).<br />
          Une liste de sélecteurs CSS recense ici toutes les occurrences de telles zones dans les modules les plus courants. Vous pouvez l'enrichir en fonction de vos besoins et de vos préférences.</p>
        </li>
      </ul>
    </li>
    <li>
      <h5 class="mms-toggler" id="advanced-rendering">L'onglet RENDU</h5>
      <ul>
        <li>
          <h6 class="mms-toggler" id="advanced-rendering-fallback">Groupe RÉPONSE EN CAS DE TRADUCTION MANQUANTE</h6>
          <p>Au sein d'une zone de texte multilingue, une traduction manquante correspond à plusieurs cas de figure distincts :
          <ol>
            <li>il n'existe pas de bloc pour la langue demandée (texte existant avant l'installation de MMS, ou nouvelle langue récemment définie)</li>
            <li>il existe un bloc pour la langue demandée, mais il contient quelque chose comme "...Langue...", où "Langue" est le nom vernaculaire de la langue (zone normalisée par MMS en l'absence d'un contenu textuel)</li>
            <li>il existe un bloc pour la langue demandée, mais il est vide (l'auteur a effacé le contenu du bloc)</li>
          </ol>
          <ul>
            <li>Les cas n° 1 et 2 sont toujours vus comme traduction manquante et traités en conséquence (voir points suivants).<br />
            Une option permet de définir si le cas n° 3 est également considéré comme traduction manquante (valeur par défaut), ou comme absence volontaire de texte (aucun contenu ne sera rendu).</li>
            <li>Lorsqu'une traduction est réellement manquante (compte tenu de l'option ci-dessus), une autre option permet de définir si elle sera remplacée par le texte correspondant dans la langue principale du site (valeur par défaut), ou remplacée par un message personnalisé.</li>
            <li>Le texte remplaçant une traduction manquante peut être muni d'une bulle d'aide également personnalisée, et mis en forme par des CSS librement définis.</li>
          </ul>
          </p>
        </li>
        <li>
          <h6 class="mms-toggler" id="advanced-rendering-pages">Groupe LOCALISATION DU CONTENU DES PAGES</h6>
          <p>Par défaut, MMS localise tous les contenus multilingues d'une page avant son affichage. Cependant la plupart des pages contenant des formulaires doit échapper à cette localisation pour permettre la saisie des textes multilingues.<br />
          Une liste d'urls (qui peuvent être exprimées sous forme d'expressions régulières) recense ici toutes les pages de ce type dans les modules les plus courants. Vous pouvez l'enrichir en fonction de vos besoins et de vos préférences.</p>
        </li>
        <li>
          <h6 class="mms-toggler" id="advanced-rendering-linebreaks">Groupe SAUTS DE LIGNE SUPPLÉMENTAIRES</h6>
          <p>MMS permet d'introduire des sauts de ligne lors de la saisie de certains textes courts (comme le titre d'un noeud ou d'une option de menu), là où ils ne sont normalement pas autorisés par Drupal. Par défaut, ces sauts de ligne sont simplement rendus en tant qu'espaces, comme s'ils n'avaient pas été saisis.<br />
          Une liste de sélecteurs CSS recense ici les zones dans lesquelles ils seront effectivement rendus en tant que sauts de ligne. Vous pouvez l'enrichir en fonction de vos besoins et de vos préférences.</p>
        </li>
      </ul>
    </li>
    <li>
      <h5 class="mms-toggler" id="advanced-tokens">L'onglet JETONS</h5>
      <p>MMS vous permet de disposer de jetons alternatifs assurant la traduction des textes multilingues qu'ils adressent. Chaque jeton est clôné en deux exemplaires : le premier rendra le texte localisé dans la langue actuellement sélectionnée, le second dans la langue principale (native) du site.
      <ul>
        <h6 class="mms-toggler" id="advanced-tokens-raw">Groupe CLÔNAGE DE JETONS BRUTS</h6>
        <p>Une liste recense ici les jetons "bruts" (par opposition aux jetons de champs, ci-dessous) à clôner. Vous pouvez l'enrichir en fonction de vos besoins et de vos préférences.<br />
        Un jeton comme <em>[type:token]</em> fournit les clônes <em>[type:mms-token]</em> et <em>[type:mms-native-token]</em>.</p>
      </ul>
      <ul>
        <h6 class="mms-toggler" id="advanced-tokens-fields">Groupe CLÔNAGE DE JETONS DE CHAMPS</h6>
        <p>Une liste de cases à cocher recense ici tous les types de champs actuellement définis. Vous pouvez sélectionner quels types doivent donner lieu à clônage des jetons relatifs aux champs de ce type.<br />
        Un jeton comme <em>[node:field_myfield]</em> fournit les clônes <em>[node:mms-field_myfield]</em> et <em>[node:mms-native-field_myfield]</em></p>
      </ul>
      </p>
    </li>
    <li>
      <h5 class="mms-toggler" id="advanced-default">L'onglet VALEURS PAR DEFAUT</h5>
      <p>
      Une liste de cases à cocher recense ici toutes les options des trois onglets précédents. L'exécution entraîne le retour à la configuration par défaut pour chacune des options sélectionnées.</p>
    </li>
    <li>
      <h5 class="mms-toggler" id="advanced-translate">L'onglet TRADUCTIONS DE L'IU</h5>
      <p>***
      </p>
    </li>
  </ul>
</p>
</section>
<section> <!-- ckeditor -->
<h4 class="mms-toggler" id="ckeditor">
  Utilisation avec <em>CKEditor</em>
</h4>
<p>
  Avec le module <em>CKEditor</em>, vous pouvez bénéficier d'un mode de saisie amélioré qui fait abstraction de la syntaxe ci-dessus et vous propose automatiquement des zones de saisie dédiées à chacune des langues définies pour le site.<br />
  Pour utiliser cette possibilité :
  <ul>
    <li>
      votre version de <em>CKEditor</em> doit inclure le plugin <em>Widget</em> (vous pouvez l'installer à partir du constructeur <em>CKEditor</em> à la page !CKEditorBuilderLink)</li>
    <li>dans la configuration de <em>CKEditor</em>, pour chaque profil dans lequel vous voulez l'autoriser, dans le groupe "APPARENCE DE L'EDITEUR" :
      <ol>
        <li>dans la section "Barre d'outils", ajoutez le bouton "MMS" à la barre d'outils</li>
        <li>dans la section "Plugins", cochez "MMS" dans la liste des plugins à activer</li>
      </ol>
    </li>
    <li>désormais, dans ce profil, toute partie de champ texte utilisant la syntaxe "multi" fait automatiquement apparaître un groupe "MMS", à l'intérieur duquel un sous-groupe est affecté à chaque langue définie</li>
    <li>à tout moment, vous pouvez cliquer sur le bouton "MMS" de la barre d'outils pour créer un nouveau groupe "MMS" vide</li>
    <li>notez que seuls les champs texte (ou le texte des blocs) peuvent être saisis sous <em>CKEDITOR</em> : dans les vues, et dans les titres de noeuds ou de blocs, vous devrez toujours utiliser la syntaxe "multi", ou l'interface améliorée proposée par le widget intégré (à partir de la version 7.x-2.0)</li>
  </ul>
</p>
</section>
<section> <!-- pathauto -->
<h4 class="mms-toggler" id="pathauto">
  Utilisation avec <em>Pathauto</em>
</h4>
<p>
  Avec le module <em>Pathauto</em>, si vous avez introduit la syntaxe "multi" dans le titre des noeuds, vous pouvez utiliser le jeton <code>[node:mms&#x2011;native&#x2011;title]</code> pour générer des alias d'URL, qui seront localisés dans la <strong>langue par défaut du site</strong>.<br />
  ATTENTION : utiliser le jeton <code>[node:title]</code> générerait les alias à partir du <strong>texte brut</strong> du titre, pour donner quelque chose comme <code>multienmy&#x2011;titlefrmon&#x2011;titremulti</code> !
</p>
</section>
<section> <!-- views -->
<h4 class="mms-toggler" id="views">
  Utilisation avec <em>Views</em>
</h4>
<p>
  Avec le module <em>Views</em>, vous pouvez utiliser la syntaxe "multi" au sein des textes saisis dans les formulaires de définition d'une vue.<br />
  Ils seront rendus selon les règles exposées ci-dessus, dans le sommaire des vues, dans les aperçus, et bien sûr dans les pages où elles sont incluses.
</p>
</section>
<section> <!-- syntax -->
<h4 class="mms-toggler" id="syntax">
  Syntaxe (pour saisie en mode texte brut)
</h4>
<p>Ce qui suit décrit la structure interne qui permet à MMS de fonctionner, telle qu'initialement définie par son prédécesseur, le module <em>Multilang</em>.<br />
Avec MMS vous pouvez totalement ignorer la syntaxe "multi". Cependant vous pouvez continuer à l'utiliser à votre convenance, dans le cadre d'une saisie directe en mode "texte brut", notamment en cas de migration depuis <em>SPIP</em>.<br />
Pour pouvoir saisir du texte dans ce mode, vous devez :
  <ul>
    <li>dans les zones gérées avec <em>CKEditor</em>, cliquer sur le lien "Basculer vers l'éditeur de texte brut" ou le bouton "Source" de <em>CKEditor</em></li>
    <li>dans toutes les autres zones, cliquer sur le bouton de bascule "MMS" situé en bas à droite du formulaire : les widgets MMS disparaissent et révèlent les zones de saisie originales, avec leur contenu brut<br />
    <em>NOTA : ce bouton de bascule n'est disponible que si vous avez activé l'option de configuration correspondante (voir <a href="#advanced-entry-iu">Possibilité de saisie en texte brut</a>)</em>
    </li>
  </ul>
</p>
<p>
  Lorsque vous saisissez des données dans un champ de type texte, vous pouvez insérer des portions de texte multilingues en les enfermant entre des balises <code>&#91;&nbsp;multi&nbsp;&#93;</code> et <code>&#91;&nbsp;/multi&nbsp;&#93;</code>.<br />
  Une telle portion de texte est un <em>segment</em> "multi", qui peut contenir un certain nombre de <em>blocs</em> "multi", chacun représentant le contenu souhaité, traduit dans une langue donnée, sous la forme <code>[marqueur&#x2011;de&#x2011;langue]&hellip;contenu&hellip;</code>, où :
  <ul>
    <li><code>marqueur&#x2011;de&#x2011;langue</code> est le code de langue concerné, par exemple <code>en</code>, <code>fr</code>, <code>es</code>&hellip;</li>
    <li><code>&hellip;contenu&hellip;</code> est le contenu traduit</li>
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
  Tous les espaces, caractères "nouvelle ligne", sauts de ligne ou sauts de paragraphe HTML sont ignorés lorsqu'il se trouvent juste après les balises ouvrantes, juste avant les balises fermantes, et précisément autour des marques de code de langue. En d'autres termes, chaque bloc de texte est rendu "nettoyé".
</p>
</section>
<section> <!-- tips -->
<h4 class="mms-toggler" id="tips">
  Conseils
</h4>
<ol>
  <li>Le code de langue choisi par le filtre pour extraire le texte dépend de la partie "langue" de l'URL actuelle (par exemple "fr" dans <code>http://exemple.com/fr/&hellip;</code>), qui est généralement définie par la situation actuelle du sélecteur de langue. Si aucune langue n'est actuellement définie (la langue actuelle pour Drupal est donc vide), c'est la langue du site qui est utilisée.<br />
    Cet "effet de bord" intéressant vous permet à tout moment de désactiver le sélecteur de langue, de telle sorte que tous les contenus utilisant le module MMS seront simplement rendus dans la langue du site.<br />
    ATTENTION: au moment où ce document est rédigé (Drupal 7.34), il semble que la désactivation du sélecteur de langue provoque la perte des traductions des termes de taxonomie !</li>
  <li>Si un segment "multi" ne possède aucune traduction pour la langue actuelle, c'est le texte disponible dans la langue du site qui sera affiché. Cependant vous pouvez configurer MMS pour qu'aucun texte ne soit affiché dans ce cas (voir <a href="#advanced-rendering-fallback">Traduction manquante</a>)</li>
  <li>Pour une migration simplifiée de données en provenance du CMS SPIP, une syntaxe alternative est également acceptée, utilisant des balises du type HTML comme <code>&lt;&nbsp;multi&nbsp;&gt;</code> et <code>&lt;&nbsp;/multi&nbsp;&gt;</code> à la place de <code>&#91;&nbsp;multi&nbsp;&#93;</code> et <code>&#91;&nbsp;/multi&nbsp;&#93;</code> (mais les blocs de langue emploient toujours les crochets, comme dans <code>&#91;&nbsp;fr&nbsp;&#93;</code>).<br />
    Vous pouvez aussi utiliser cette syntaxe alternative lorsque vous saisissez du texte manuellement (hors <em>CKEditor</em> ou widget MMS).</li>
  <li>Attention lorsque vous importez directement des données depuis une source externe : les règles internes du plugin <em>HTML purifier</em> amènent <em>CKEditor</em> à interrompre la reconnaissance automatique des blocs de langue lorsqu'ils contiennent des balises <code>&lt; div &gt;</code>. Vous pouvez envisager de les remplacer par des balises <code>&lt; section &gt;</code> tags.<br />
    Un autre solution pourrait consister à désactiver le plugin <em>HTML purifier</em>, mais cela est déconseillé, pour des raisons générales de sécurité.</li>
</ol>
</section>
<section> <!-- localization -->
<h4 class="mms-toggler" id="localization">
  Localisation
</h4>
<p>
  Du fait de sa vocation à intégrer plusieurs langues dans un même conteneur, le module <em>MMS</em> ne se conforme pas à la norme Drupal en matière de localisation : sur ce plan, il est auto-suffisant et embarque la totalité de ses propres traductions, rédigées avec la syntaxe "multi".<br />
  Toutes ces traductions (à l'exception de l'aide que vous lisez en ce moment) sont rassemblées dans le fichier <code>mms.data.inc</code> : si celui-ci ne comprend pas encore de version dans la langue de votre choix, vous pouvez aisément en ajouter une à l'aide des outils disponibles dans la page de traduction de l'interface utilisateur !MMSUILink.<br />
</p>
<p>
  Du fait de sa taille, le texte de l'aide n'est pas inclus dans le fichier <code>mms.data.inc</code>. A la place, il réside dans un jeu de fichiers indépendants et distincts par langue, chacun sous la forme <code>mms.help.<em>LANGUE</em>.inc</code>, où le contenu est du pur HTML, enfermé entre des balises PHP fermante/ouvrante.<br />
  Ainsi pour ajouter une version du texte d'aide dans le langage de votre choix, par exemple XX, vous devez créer un nouveau fichier nommé <code>mms.help.<em>XX</em>.inc</code> et situé dans le dossier <code>sites/all/mms</code>. Veuillez prendre soin de conserver intacte la structure HTML originale, notamment les en-têtes de sections avec leurs attributs <code>class</code> et <code>id</code>.
</p>
</section>
