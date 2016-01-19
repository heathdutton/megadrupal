How do you use this?

Place the sar directory in ~/.drush/commands (create it if you don't have one)
and run `drush help sar'.

Use `drush sar "replace me" "new text"' to replace all occurrences of
"replace me" with "new text" in all text fields (node body and field API) and
all custom blocks on the site.

Be sure to backup your database before running this command, as that is the
only way to undo changes.
