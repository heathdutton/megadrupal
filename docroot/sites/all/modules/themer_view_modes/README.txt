This module allows you to define view modes for nodes or other entity types using your theme's .info file.
The module will scan all *enabled* themes for declared view modes.

To use, enable the module and then place the following snippet in your theme's .info file:
; View Modes
; To add view modes for NODES (and not other entity types) in the simplest way possible:
; viewmodes[] = The label of the node view mode
; For advanced control of the view mode's machine name, label, and entity type, use the form:
; viewmodes[machine_name][entity_type] = The label of the entity's view mode

Some examples:
; View mode for nodes (machine name will be my_node_view_mode_label)
viewmodes[] = My Node View Mode Label

; View mode for an entity type of 'my_entity_type' with machine name 'my_mode' and human-readable label of "My entity mode"
viewmodes[my_mode][my_entity_type] = My entity mode