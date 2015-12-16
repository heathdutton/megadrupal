; $Id $

DESCRIPTION
--------------------------
The WordStream module integrates WordStream keyword research tools into your Drupal site. 

INSTALLATION
---------------
- You will need to create the "class.wordstream.inc" file and place it in the wordstream/lib directory. To create this file follow the instructions in wordstream/lib/README.txt

- Upload the WordStream files, including the added class.wordstream.inc to a modules directory on your server such as sites/all/modules/wordstream.

- Enable the WordStream module via the Admin > Site building > Modules

- Configure the module by going to Admin -> Settings -> WordStream
    - Follow the instruction for entering the API account login and/or free widgets embed id

- You can now do keyword research at Admin -> Content -> WordStream or if the Keyword Research module is installed at Admin -> Content -> Keyword Reserach

DEPENDENCIES & RECOMMENDATIONS
---------------
WordStream has no module dependencies. 

It is recommended to use use the module in conjunction with the Keyword Research module:
http://drupal.org/project/kwresearch


CREDITS
----------------------------
Authored and maintained by Tom McCracken <tom AT leveltendesign DOT com> twitter: @levelten_tom
Sponsored by LevelTen Interactive - http://www.leveltendesign.com