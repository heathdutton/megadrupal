/**
 * Show/hide user list on change of radio button.
 */

jQuery(document).ready(function() {
    jQuery("input[name=user_cancel_method]:radio").change(function() {
        if (this.value == 'user_cancel_reassign_to_superadmin') {
            jQuery('#userlist').show();
        } else {
            jQuery('#userlist').hide();
        }
    });
});
