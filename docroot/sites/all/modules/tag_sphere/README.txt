-- SUMMARY --

Tag sphere module provides integration with jQuery plug-in Tag Sphere
to create vocabulary blocks with rotating terms.

-- REQUIREMENTS --

Tagsphere jQuery plug-in.

-- INSTALLATION --

To Download Tagsphere jQuery plug-in.
1. Visit the following link.
   https://bitbucket.org/elbeanio/jquery.tagsphere/downloads.
2. Click on Tags tab, you can see all tag versions of jquery.tagsphere plug-in.
   And then download the latest version.
3. Extract the plug-in,
   copy the jquery.tagsphere.js and ./ext/jquery.mousewheel.min.js files,
   paste those files into sites/all/modules/tag_sphere/js folder.
4. Enable the Tag Sphere module by visiting the admin/modules page.

-- CONFIGURATION --

1. Visit admin/structure/taxonomy in you site.
2. Click on Add vocabulary to create new vocabulary.
3. You have a checkbox called "Tag sphere block". If you check this option,
   you will have the Tag sphere block for the particular vocabulary.
4. Visit the admin/structure/block, you can see created Tag sphere block
   with the name "Tag Sphere - <vocabularyname>".
5. Enable the available block in specific region.
6. You can configure the Tag sphere block,
   where you can enter your own Background color,
   Font color, etc., under the "Tag sphere settings".

-- TROUBLESHOOTING --

1. Tag sphere will work properly with the jQuery
   JavaScript Library v1.4.2 and above.
2. You need to have atleast one node to be created by selecting
   any of term related to your vocabulary.
   Then only the Tag sphere block will appear.
3. Clear the performance cache if problem persists.
