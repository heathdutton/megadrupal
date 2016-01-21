Drupal.behaviors.CmsExposedFilters = function (context) {
  //Add class to select based on multiple attribute
  $(".views-widget select.form-select").each(function(i) {
    if ($(this).attr('multiple')) {
      $(this).addClass('select-multiple');
    }
    else {
      $(this).addClass('select-single');
    }
  });

  //Add custom class if not present (after ajax call)
  $(".views-exposed-widget").each(function(i) {
    //Find button area
    if ($(this).find('input.form-submit').length > 0) {
      //Add custom theme class
      if (!$(this).hasClass('cms-filter-btns')) {
        $(this).addClass('cms-filter-btns');
      }
      if ($(this).find('.btn-cms-reset').length < 1) {
        //Add reset button
        var resetButton = '<input class="btn-cms-reset" type="reset" value="' + Drupal.t("Reset") + '" />';
        $(this).append(resetButton);
      }
    }
  });

  //Reset form function (useful after submit)
  $(".btn-cms-reset").click(function() {
    var formid = '#' + $(this).parents("form").attr('id');
    // Use a whitelist of fields to minimize unintended side effects.
    $(formid + ' > :text, :password, :file').val('');
    // De-select any checkboxes, radios and drop-down menus
    $(':input', formid).each(function() {
      //Exclude hidden fields
      if ($(this).attr('id') != '') {
        //Select
        $('#' + $(this).attr('id') + " > option").each(function() {
          $(this).removeAttr('selected').removeAttr('checked');
        });
        //Radio buttons and checkboxes
        //TODO : test if it works
        $(this).removeAttr('selected').removeAttr('checked');
      }
    });
  });
}
