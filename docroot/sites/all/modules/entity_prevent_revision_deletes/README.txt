*************************************
***   READ ME                      **
*************************************

This module is a big hammer. A big hammer that alters your site's entities ability to delete revisions.

This could be useful to you for a number of reasons.

You may:

* wish to preserve field collection items but do not wish to apply either patch that supports revisions and field collections.
* Need to grant very high levels of revision access to users (say because of workbench) but you do not wish them to delete revisions.

Entity revision deletes will simply not happen. It will not be graceful. Hence the hammer.

