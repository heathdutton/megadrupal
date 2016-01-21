FormAssembly is a module to synchronize form metadata from FormAssembly.com to the drupal database.

The forms are retrieved via rest api at the time of display.  They may be embeded in content as an
entityreference field:
    Target: FormAssembly Form
    Mode: Simple (with optional filter by bundle)
     -- Sort property: name
     -- Sort direction: ascending

    Configure the display to:
    Label: Hidden
    Format: Rendered Entity
      -- View mode: FormAssembly Markup
      -- Show links: no

Forms are also available on their own url at formassembly/%entity_id%

The user will need to configure the module to use the appropriate OAuth credentials
(provided by FormAssembly.com).

Module configuration page: /admin/config/services/formassembly

