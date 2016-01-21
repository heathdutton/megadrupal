This modules allows you to set the exposed filters forms in views to use method POST instead of GET.

----------------------------------------------
Why?

To be honest, there aren't many cases where you would need that, but here are the main 2:
1- Even after filtering, the URL will still look nice to the end-user.
2- At some point, the query string in the URL might become so long, that it exceeds the URL limit set by the server. This module provides a solution for this issue.

----------------------------------------------
Integration With better_exposed_filters (BEF)

If you need BEF on your website, it is totally possible to use both these modules together! The Views ExPost will automatically provide all features from BEF if it's enabled!

Simply put: If BEF is enabled, the ExPost plugin will extend the plugin from BEF :)

----------------------------------------------
Watch Out!

Using method POST for exposed filters means that users will NOT be able to bookmark their filter results.