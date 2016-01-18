CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation


INTRODUCTION
------------

Current Maintainer: Marie Wendel <caligan@gmail.com>

Minor Edit provides a toggle in node edit forms to mark an edit as 'minor.'
This prevents its updated timestamp from changing, so that minor-edited 
nodes don't float to the top of time-sorted lists.  New nodes cannot be
marked minor.


INSTALLATION
------------

1. Download the archive to the appropriate modules directory and unarchive.
2. Enable at admin/modules.
3. Configure the content types which should support this toggle at
   admin/config/minor_edit/settings.
4. Enable the 'mark minor edits' permission on roles which should be able
   to prevent timestamping.
