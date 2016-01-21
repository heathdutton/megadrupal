<?php
/**
 * @file
 * The page template for the language selection screen.
 *
 * Variables used:
 * - head: the relevant head tag
 * - styles: the relevant CSS tags
 * - scripts: the relevant Javascript tags
 * - site_name: the name of the site
 * - urls: array with urls to the page we are trying to access.
 *
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en" dir="ltr">
  <head>
    <?php print $head ?>
    <title><?php print $site_name; ?> | Taalkeuze | Choix de langue | Sprachauswahl | Language Choice</title>
    <?php print $styles; ?>
    <style type="text/css">
      .language-selection-page {
        background: transparent none;
      }
      .language-selection-page #page {
        margin: 100px auto;
        background-color: #fafafa !important;
        background-image: -moz-linear-gradient(bottom, #fafafa, #e6e6e6) !important;
        background-image: -webkit-gradient(linear, 0 0, 0 100%, from(#e6e6e6), to(#fafafa) ) !important;
        background-image: -webkit-linear-gradient(bottom, #fafafa, #e6e6e6) !important;
        background-image: -o-linear-gradient(bottom, #fafafa, #e6e6e6) !important;
        background-image: linear-gradient(to top, #fafafa, #e6e6e6) !important;
        background-repeat: repeat-x !important;
        filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#e6e6e6', endColorstr='#fafafa', GradientType=0 ) !important;
        border-top: 1px solid #ccc !important;
        border-bottom: 1px solid #ccc !important;
      }
      .language-selection-page #header {
        border: none;
        background: transparent none;
        filter: none;
      }
      .language-selection-page #main {
        padding-bottom: 180px;
      }
      .language-selection-page #logo-title {
        background: transparent url(../images/bg-shadow-top-light.png) no-repeat center top !important;
        padding: 100px 0 75px 0;
        margin-top: -15px;
        text-align: center;
      }
      .language-selection-page #footer-wrapper {
        background: transparent url(../images/bg-shadow-bottom-light.png) no-repeat center bottom !important;
        padding: 75px 0 0px 0;
        margin-bottom: -15px;
      }
      .language-selection-page #langchoice-wrapper {
          width: 960px;
          margin: 0 auto;
          clear: both;
      }
      .language-selection-page ul.langchoice_list {
          text-align: center;
      }
      .language-selection-page .langchoice_list li {
          margin: 0px;
          padding: 0px;
          width: 232px;
          text-align: center;
          display:inline-block;
          *display:inline; /* ie7 fix */
          zoom:1; /* hasLayout ie7 trigger */
      }
      .language-selection-page span.langchoice_label {
          margin: 0px;
          padding-top: 60px;
          padding-bottom: 20px;
          width: 100%;
          text-align: center;
          float: left;
      }
    </style>
    <?php print $scripts; ?>
  </head>
  <body class="language-selection-page">
	  <div id="page">
    	<div id="logo-title" >
        <img src="<?php print $logo; ?>" alt="Logo" title="Logo" />
	    </div>
	    <div id="header" class="container">
	    </div>
     	<div class="container">
      	<div id="main" class="column">
        	<div id="main-squeeze">
          	<div id="content">
              <ul class="langchoice_list">
                <?php if (isset($urls['items']['nl'])) : ?>
                <li class="nl" lang="nl">
                  <span class="langchoice_label">Kies uw taal</span>
                  <a class="langchoice_link button" href="<?php print $urls['items']['nl']; ?>">Nederlands</a>
                </li>
                <?php endif; ?>
                <?php if (isset($urls['items']['fr'])) : ?>
                <li class="fr" lang="fr">
                  <span class="langchoice_label">Choissisez votre langue</span>
                  <a class="langchoice_link button" href="<?php print $urls['items']['fr']; ?>">Fran&ccedil;ais</a>
                </li>
                <?php endif; ?>
                <?php if (isset($urls['items']['de'])) : ?>
                <li class="de" lang="de">
                  <span class="langchoice_label">W&auml;hlen Sie ihre Sprache</span>
                  <a class="langchoice_link button" href="<?php print $urls['items']['de']; ?>">Deutsch</a>
                </li>
                <?php endif; ?>
                <?php if (isset($urls['items']['en'])) : ?><li class="en" lang="en">
                  <span class="langchoice_label">Choose your language</span>
                  <a class="langchoice_link button" href="<?php print $urls['items']['en']; ?>">English</a>
                </li>
                <?php endif; ?>
              </ul>
          	</div>
	        </div>
	      </div>
	    </div>
	    <div id="footer-wrapper">
	      <div id="footer">
	      </div>
	    </div>
	  </div>
	</body>
</html>
