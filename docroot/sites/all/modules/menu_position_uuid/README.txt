This module adds deploy and UUID support to the menu_position module.  There are no changes to the UI.

If you are using custom menu_position plugins be sure to implement
hook_menu_position_uuid_inbound_$PLUGIN and hook_menu_position_uuid_outbound_$PLUGIN if you are using
serial id's so it's compatible with deploy/services/uuid

See menu_position_uuid_menu_position_uuid_outbound_user_role and menu_position_uuid_menu_position_uuid_inbound_user_role