

Introduction
-------------------

CodeMagic is an advanced source code editor plugin for TinyMCE.

It integrates the CodeMirror library for syntax coloring, and the JSBeautifier library for code formating and indentation.



Dependencies
-------------------

CodeMagic: http://github.com/tinymce-plugins/codemagic
WYSIWYG: http://drupal.org/project/wysiwyg
TinyMCE: http://tinymce.moxiecode.com



Installation
-------------------
1. Install the WYSIWYG module as normal
 - http://drupal.org/documentation/install/modules-themes/modules-7



2. Download the TinyMCE editor and extract the archive into a new folder in the following location:

sites/all/libraries/tinymce 
So the actual library can be found at:
sites/all/libraries/tinymce/jscripts/tiny_mce/tiny_mce.js



3. Download the CodeMagic plugin into:

sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/codemagic

So the actual plugin can be found at:
sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/codemagic/editor_plugin.js



4. Go to your WYSIWYG profiles under /admin/config/content/wysiwyg, and enable "CodeMagic" under "Buttons and Plugins". This will give you a new button in your TinyMCE toolbar.



Languages
-------------------

If you'd like to create a language translation for CodeMagic, follow these steps:

	1. Download your TinyMCE language pack from http://www.tinymce.com/i18n/index.php?ctrl=lang&act=download&pr_id=7

	2. Unzip the language pack in sites/all/libraries/tinymce/jscripts/tiny_mce
	   For example, if you downloaded the French language pack, the French files should be in:

		 sites/all/libraries/tinymce/jscripts/tiny_mce/langs/fr.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/advhr/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/advimage/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/advlink/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/emotions/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/fullpage/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/media/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/paste/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/searchreplace/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/style/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/table/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/template/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/xhtmlxtras/langs/fr_dlg.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/themes/simple/langs/fr.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/themes/advanced/langs/fr.js
		 sites/all/libraries/tinymce/jscripts/tiny_mce/themes/advanced/langs/fr_dlg.js

	3. Copy the language file in 
		 sites/all/libraries/tinymce/jscripts/tiny_mce/langs
	   to
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/codemagic/langs

	   For example, if you downloaded the French language pack, the French file should be in:
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/codemagic/langs/fr.js

	4. Copy the English dialog file sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/codemagic/langs/en_dlg.js to your language.
	   For example, if you downloaded the French language pack, the French dialog file should be in:
		 sites/all/libraries/tinymce/jscripts/tiny_mce/plugins/codemagic/langs/fr_dlg.js

	5. Edit the dialog file to translate the English words to your language.

	6. Go to one of your Drupal WYSIWYG profiles (/admin/settings/wysiwyg) and choose your language in "Interface language" under "Basic setup".





Development is sponsored by TinymceSupport.com

