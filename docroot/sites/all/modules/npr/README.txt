Introduction
  These are a set of closely-related modules that can push/pull content to/from the NPR API.
  The npr_api module will not add features to your site but provides a framework on which to
  build your own modules. The other submodules in this project depend heavily on npr_api.
  Please see the project homepage for details on what individual modules do.

Quick Start
  0) The modules will do ALMOST NOTHING without an NPR API key. Get one at http://api.npr.org
  1) Download and install modules as usual.
  2) Set your API key at admin/config/services/npr
  3) npr_pull module will retrieve stories from the API:
     3a) either one at a time, at: admin/content/npr
     3b) or automatically, via cron, at: admin/config/services/npr/cron
  4) npr_push allows you to push locally created stories up into the NPR API.
     It requires a special API key + registration of your IP address with NPR
     (you can only push with that key, from that IP). Contact NPR Digital Services
     for more help: http://info.ds.npr.org/contact.html

Pulling Stories
  npr_pull will retrieve stories and populate the npr_story content type.
  Tweak the default settings of the pull by:
  *) editing the npr_story content type directly
  *) setting configuration parameters at admin/config/services/npr/cron

Images
  *) By default, field_npr_image has cardinality = 1, meaning it will only accept
     1 image during a pull. However, this can be set to any value.
  *) To alter the directory in which images are saved, simply edit the
     "File directory" value of field_npr_image on npr_story.

Audio
  *) If you're pulling down stories from NPR that include audio, be sure to
     check out the NPR Player Pack module (audio is fairly minimal-looking
	 without some sort of additional player module).
	 Link: https://drupal.org/project/npr_player_pack

Tags
  Tagging is not enabled out of the box. To configure tagging on pulls:
  1) Create a vocabulary to be used for tagging NPR stories.
  2) Turn on tagging and select which tags are to be used during pulls at
     admin/config/services/npr/tags
  3) Add a term_reference field to the npr_story content type. Make sure it
     3a) matches the vocabulary set in step 2
     3b) is not a required field
     3c) can accept multiple values
     3d) uses the "Autocomplete term widget (tagging)" widget

Text Format
  To set a text format for all pulled stories:
  1) Create a text_format that filters the body appropriately at
     admin/config/content/formats/add
  2) Edit the npr_story content type body field at
     admin/structure/types/manage/npr-story/fields/body
  3) Assign the text_format to the field using the "DEFAULT VALUE" form.
     3a) Enter placeholder text in the body form. This is necessary for
         Drupal to remember what happens next.
     3b) Select the text_format you created in step 1.
         Ensure that the user pulling NPR articles has permission to use
         this text_format. Otherwise, the text_format with the lowest weight
         of the remaining permitted formats will be used.
     3c) Save the settings

  If you want to set a default text_format without placeholder text,
  you probably want the better_formats module:
  https://drupal.org/project/better_formats
  Please see issue [#2056763] for context.

Pushing Stories

  1) npr_push will push a node of any given content type to the API.
  2) Configure your push settings at admin/config/services/npr/push
  3) Any/all content types and fields can be mapped to NPRML data
  4) BE SURE to check the 'Push XXXX nodes to the NPR API' checkbox (a.k.a. the
     killswitch).
  5) BE SURE to map a field to the NPRML ID field
  6) This ID field should be LEFT BLANK; upon successful push the module will
     automatically fill it out with the new ID sent back from the API.
  7) There are also push flags, i.e., fields that are associated with certain actions
     on a per-node basis. If a node's mapped 'Push flag' is in any way truthy,
     it will be pushed to the API. If not, it won't. The same goes for the
     'NPR One flag'.
  8) Title, teaser, and body will automatically be mapped to the corresponding NPRML elements.
  9) In general, a story's NPRML requires a title, a teaser, and a pubDate in order to be successfully pushed to the API.
