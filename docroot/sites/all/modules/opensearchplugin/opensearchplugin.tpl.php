<?php

/**
 * @file
 * Template for the opensearch.xml file.
 */

?><OpenSearchDescription xmlns="http://a9.com/-/spec/opensearch/1.1/">
	<ShortName><?php print $shortname; ?></ShortName>
	<Description><?php print $description; ?></Description>
	<Tags><?php print $tags; ?></Tags>
	<Contact><?php print $contact; ?></Contact>
	<Url type="text/html" method="get" template="<?php print $htmlsearch; ?>{searchTerms}"/>
	<SearchForm><?php print $searchform; ?></SearchForm>
	<LongName><?php print $longname; ?></LongName>
	<?php if (!empty($variables['icon'])) : ?>
	<Image
		<?php if (!empty($variables['iconheight'])) : ?> height="<?php print $variables['iconheight']; ?>" <?php endif; ?>
		<?php if (!empty($variables['iconwidth'])) : ?> width="<?php print $variables['iconwidth']; ?>" <?php endif; ?>
		>
		<?php print $icon; ?>
	</Image>
	<?php endif; ?>
	<Developer><?php print $developer; ?></Developer>
	<Attribution><?php print $attribution; ?></Attribution>
	<SyndicationRight><?php print $syndicationright; ?></SyndicationRight>
	<AdultContent><?php print $adultcontent; ?></AdultContent>
	<Language><?php print $language; ?></Language>
	<OutputEncoding>UTF-8</OutputEncoding>
	<InputEncoding>UTF-8</InputEncoding>
</OpenSearchDescription>
