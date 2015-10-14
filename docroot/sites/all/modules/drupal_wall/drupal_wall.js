jQuery(document).ready(function($) {

  jQuery('.drupal_wall_post_status #edit-shout-box-status-format').hide();
  jQuery('.drupal_wall_post_status #switch_edit-shout-box-status-value').hide();
  jQuery('.drupal_wall_post_status fieldset').hide();
  jQuery(document).ajaxComplete(function() {
    // Clear comment text field after save !
    jQuery('*[id^=edit-drupal-wall-comment]').val('');
  });
});

// Auto upload image & hide upload button.
(function ($) {
  Drupal.behaviors.autoUpload = {
    attach: function(context, settings) {
      $('.form-item input.form-submit[value=Upload]', context).hide();
      $('.form-item input.form-submit[value=Post]', context).hide();
      $('.form-item input.form-file', context).change(function() {
        $parent = $(this).closest('.form-item');

        setTimeout(function() {
          if(!$('.error', $parent).length) {
            $('input.form-submit[value=Upload]', $parent).mousedown();
          }
        }, 100);
      });
    }
  };
})(jQuery);
