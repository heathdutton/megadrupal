-- SUMMARY --

This is a smart module that basically extends Drupal's display date to provide Hijri Date integration with Drupal core date field and with other Drupal contributions.

This module is integrated very well with Views module. You can use it to display Hijri date instead of Gregorian date or you can have them both by mention "الموافق".
Also this module coming with Hijri block that will showing Hijri today date on your website wherever you want to show today date in your header for example.

For some Hijri issues, sometime the day by the algorithm be not exact the same day in the current period because of the moon cycle, so we resolve this issue by adding custom Hijri increment field so you can increment or decrement the Hijri date to meet the real day.


-- REQUIREMENTS --

None.

-- INSTALLATION --

* Install as usual, see https://www.drupal.org/documentation/install/modules-themes/modules-7 for further information.


-- CONFIGURATION --

 - You can reach Hijri settings by going to this path: Administration » Configuration » Regional and language » Date and time » Hijri Settings.
 - Also you can change the Hijri format by editing the Drupal date format for (long, medium and short) types in this path: Administration » Configuration » Regional and language » Date and time » Types.
 - You Can give permissions to specific role by giving them 'administer hijri.
 - You have a block installed within the module ,, all you have to do is show it in the wanted region.


-- CUSTOMIZATION --

 - If you need to change the output of Hijri date block ,, you can find a tpl file in hijri/theme ,, so enjoy styling :).



-- CONTACT --

Current maintainers:
* Abdullah Bamelhes (drpl) - https://www.drupal.org/u/drpl
* Saud Alfadhli (samaphp) - https://www.drupal.org/u/samaphp
* Essam AlQaie (3ssom) - https://www.drupal.org/u/3ssom

