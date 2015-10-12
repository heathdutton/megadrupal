jQuery(document).ready(function() {
  jQuery("input[name=googlechat_rel]").change(function() {
    if (jQuery("input[name=googlechat_rel]:checked").val() == 2) {
      jQuery('#edit-googlechat-ur-name').removeAttr('disabled');
      jQuery('#edit-googlechat-ur-name').attr('required', 'true');
      jQuery('.form-item-googlechat-ur-name').slideDown();
    } else {
      jQuery('#edit-googlechat-ur-name').attr('disabled', 'disabled');
      jQuery('#edit-googlechat-ur-name').removeAttr('required');
      jQuery('.form-item-googlechat-ur-name').slideUp();
    }
  });

  var googlechat_rel = jQuery("input[name=googlechat_rel]:checked").val();
  if (googlechat_rel != 2) {
    jQuery('#edit-googlechat-ur-name').attr('disabled', 'disabled');
    jQuery('#edit-googlechat-ur-name').removeAttr('required');
    jQuery('.form-item-googlechat-ur-name').hide();
  }
});
