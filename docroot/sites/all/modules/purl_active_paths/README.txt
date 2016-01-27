Purl Active Paths 1.x
=========
Allows admin or module to define paths to be included or excluded from purl's outbound url rewrites on a global or provider level.

This module is useful if certain purl providers are only relevant for specific pages and not others.

This module can function in two modes, "Include" or "Exclude". In include mode, only paths defined in the admin or hook_purl_active_paths will be rewritten by purl in outbound links. In exclude mode, these paths will be excluded from rewrites.