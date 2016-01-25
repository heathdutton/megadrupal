var form_maker_show_preview;
jQuery(document).ready(function() {
  jQuery('.form_maker_preview_button').click(function(event) {
    form_maker_show_preview = 0;
    form_maker_update_ajax_data(event);
  });
})

function form_maker_update_ajax_data(event) {
  form_maker_show_preview++;
// if (document.getElementById('selected_theme_css').value) {
    jQuery.post(
      Drupal.settings.form_maker.preview_url,
      {
        theme_id: document.getElementById('selected_theme_css').value
      },
      function (data) {
        jQuery('#sliding_popup').html(data);
        if (form_maker_show_preview == 1) {
          form_maker_update_ajax_data(event);
        }
        // jQuery('#form_preview').html(jQuery(data).find('#form_preview').html());
        // jQuery('.form_maker_preview_button').click(function(event) {
          // form_maker_update_ajax_data(event);
        // });
      }
    );
  // }
  // if (event) {
    // event.preventDefault();
  // }
  // return false;
}
