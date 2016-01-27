Locale automatic import
===============================================================================
-------------------------------------------------------------------------------
Search within your modules/features for .po files and import them in the DB
within the right text group.

When you install a new modules with translations, the Locale module
automatically import them in the "Built-in interface" text group and choose the
mode "Existing strings and the plural format are kept, only new strings are
added.". It's ok for mostly all modules but not for Features that can contain
Content types, Fields or Views.

This module recognize the system name of the text group in the name of your .po
file and import it to the right text group automatically.

The .po files will be searched in an "auto_transaltions" folder within your
modules/features instead of "translations".
sites/your_site/modules/your_module/auto_translations

Naming your .po file
-------------------------------------------------------------------------------
The way you name your .po file will determine for which text group and language
they'll be imported.

    These .po files will be imported for the specified text group and the
    specified language.

        {text_group_system_name}.{langcode}.po
        {prefix}.{text_group_system_name}.{langcode}.po

    E.g.: my_module.views.fr.po, will be imported in the Views text group
          for french. 

    These .po files will be imported for the default text group and the
    specified language.

        {langcode}.po
        {prefix}.{langcode}.po

    E.g.: my_module.fr.po, will be imported in the Built-in interface text
          group for french. 

* If you don't know the system name of a certain text group, visit this page:
admin/config/regional/translate/auto_import

Overide "Default mode"
-------------------------------------------------------------------------------
The default value of mode when importing .po file with this module is set to
"Strings in the uploaded file replace existing ones, new ones are added. The
plural format is updated."

To overide this setting when new modules are installed add this line to your
settings.php file:
$conf['locale_auto_import_default_mode'] = 1;

0: Strings in the uploaded file replace existing ones, new ones are added.
   The plural format is updated.
1: Existing strings and the plural format are kept, only new strings are added.
