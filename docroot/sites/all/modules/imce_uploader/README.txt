Have you ever felt annoyed for uploading only one file at a time to IMCE? This
module gives you a new way to upload files using IMCE.

Basically, this module provides a simple API to allow other modules register to
IMCE Upload form, so user can choose the needed uploader in IMCE Profile form.
That means you can have lot of uploaders in the system and you can choose which
profile uses which uploader.

Besides that, I also provide the Plupload uploader as a plugin (in this module)
to allow uploading multiple files at a time. If you want to use it, just select
"IMCE Uploader - Plupload" the module list and enable it.

--------------------------------------------------------------------------------
Implementation:
--------------------------------------------------------------------------------

This module uses hook_form_FORM_ID_alter() to alter the IMCE Profile form
(imce_profile_form) to manage the uploaders.

--------------------------------------------------------------------------------
Integration:
--------------------------------------------------------------------------------

Feel free to integrate your uploader using my API. There are only 2 functions
you should know
- hook_imce_uploader(): Register the list of your uploaders.
- hook_imce_uploader_alter(): Change the list of registered uploaders.

You can see the file imce_uploader.api.php and my plugin for more information.

--------------------------------------------------------------------------------
Dependencies:
--------------------------------------------------------------------------------

- IMCE
- Plupload (for the Plupload plugin inside this module)

--------------------------------------------------------------------------------
Installation:
--------------------------------------------------------------------------------

Download the module and simply copy it into your contributed modules folder:
[for example, your_drupal_path/sites/all/modules] and enable it from the
modules administration/management page.
More information at: Installing contributed modules (Drupal 7).

--------------------------------------------------------------------------------
Configuration
--------------------------------------------------------------------------------

After successful installation, you need to go the profile that need to use
new file uploader. Then choose the needed uploader in the list. You can choose
any uploaders you want, even the default of IMCE.
