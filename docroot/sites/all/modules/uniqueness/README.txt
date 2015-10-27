
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation

INTRODUCTION
------------

Uniqueness module provides a way to avoid duplicate content on your site by
informing a user about similar or related content during creation of new
content.

INSTALLATION
------------

1. Copy this uniqueness/ directory to your sites/all/modules directory.

2. Enable the module and configure admin/config/content/uniqueness

3. Configure permissions for the module at admin/people/permissions. Assign
   the "Use uniqueness widget" to any roles who should be able to use the
   uniqueness search.

4. Enable uniqueness for each content type for which you would like to provide
   the uniqueness search at admin/structure/types/manage/CONTENT_TYPE_NAME

5. For further help visit admin/help/uniqueness
