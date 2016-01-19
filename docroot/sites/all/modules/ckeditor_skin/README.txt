You're wondering why do you need this module because you can download any skins
you want and put them in the skins folder. Yes that's totally true. But what if
you don't know how to do that, or too lazy too do that, or ckeditor is a
submodule in your repo or simply don't want to touch the ckeditor library? There
are so many many reasons to use this module.

This module offers:
- Download and install solution, you don't have to care about ckeditor library.
- Stay away from git submodule (if you're using it).
- Easily create a new skin and register it by using the API in this module. As I
said, you don't have to care about ckeditor.
- You can easily modify the skin list by using the alter function, an usual way
for Drupal developers.

--------------------------------------------------------------------------------
Supported Skins:
--------------------------------------------------------------------------------

- CKEditor core skins.
- Grappelli (CKEditor v3.x): http://invisiblehands.ca/mezzanine/.
- Moono (CKEditor v4.x): http://ckeditor.com/addon/moono.
- Moono Color (CKEditor v4.x): http://ckeditor.com/addon/moonocolor.
- Kama (CKEditor v4.x): http://ckeditor.com/addon/kama.

--------------------------------------------------------------------------------
Integration:
--------------------------------------------------------------------------------

Feel free to integrate your skins using my API. There are only 2 functions
you should know
- hook_ckeditor_skin(): Register the list of your skins.
- hook_ckeditor_skin_alter(): Change the list of registered skins.

You can see the file ckeditor_skin.api.php for more information.

--------------------------------------------------------------------------------
Dependencies:
--------------------------------------------------------------------------------

- CKEditor

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

After successful installation, you just need to go to CKEditor Global Profile
settings page (admin/config/content/ckeditor/editg) and choose the skin you want
