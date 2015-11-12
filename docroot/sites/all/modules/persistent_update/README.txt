The Persistent Update API module adds a hook (HOOK_persistent_update()) for
persistent updates, updates that are run every time you access update.php or run
'drush updb'.

The module was created for use with a Features driver deployment model so that
features would be reverted without human interaction, but can be used for other
solitions.

Persistent Update API was written by Stuart Clark (deciphered) and is maintained
Realityloop Pty Ltd.
- http://realityloop.com
- http://twitter.com/Realityloop



Example
--------------------------------------------------------------------------------

See persistent_update.api.php for an example implementation.
