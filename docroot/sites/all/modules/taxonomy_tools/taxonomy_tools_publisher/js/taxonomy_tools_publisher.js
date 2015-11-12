(function ($) {
  Drupal.behaviors.TaxonomyPublisher = {
    attach: function () {
      $(document).ready(function () {
        // set date fields as read-only
        $("input[name='field_taxonomy_term_publish_on[und][0][value][date]']").attr("readonly", true);
        $("input[name='field_taxonomy_term_unpublish_on[und][0][value][date]']").attr("readonly", true);
        // publish on field maximum date depends on unpublish on field value;
        // unpublish on field minimum date depends on publish on field value
        var dateNow = new Date();
        var currentMonth = dateNow.getMonth();
        var currentDate = dateNow.getDate();
        var currentYear = dateNow.getFullYear();
        var dates = $("input[name='field_taxonomy_term_publish_on[und][0][value][date]'], input[name='field_taxonomy_term_unpublish_on[und][0][value][date]']").datepicker({
          dateFormat: "dd.mm.yy",
          minDate: new Date(currentYear, currentMonth, currentDate),
          onSelect: function(selectedDate) {
            var option = $(this).attr("name") == "field_taxonomy_term_publish_on[und][0][value][date]" ? "minDate" : "maxDate",
            instance = $(this).data("datepicker"),
            date = $.datepicker.parseDate(instance.settings.dateFormat || $.datepicker._defaults.dateFormat, selectedDate, instance.settings);
            dates.not(this).datepicker("option", option, date);
            var time_name = $(this).attr("name").substr(0, $(this).attr("name").length - 6) + "[time]";
            var time_value = $(this).attr("name") == "field_taxonomy_term_publish_on[und][0][value][date]"
            ?
            $.datepicker.formatDate("dd.mm.yy", Date.today()) == selectedDate ? new Date().addMinutes(10).toString("H:mm:ss") : "00:00:00"
            :
            "23:59:59";
            $("input[name='" + time_name + "']").attr("value", time_value);
          }
        });
        // datepicker min & max dates when editing term with existing date values
        if ($("input[name='field_taxonomy_term_publish_on[und][0][value][date]']").val() != "") {
          var unpublishMinDate = new Date($("input[name='field_taxonomy_term_publish_on[und][0][value][date]']").val().substr(3,2) + "/" + $("input[name='field_taxonomy_term_publish_on[und][0][value][date]']").val().substr(0,2) + "/" + $("input[name='field_taxonomy_term_publish_on[und][0][value][date]']").val().substr(6,4));
          $("input[name='field_taxonomy_term_unpublish_on[und][0][value][date]']").datepicker("option", "minDate", unpublishMinDate);
        }
        if ($("input[name='field_taxonomy_term_unpublish_on[und][0][value][date]']").val() != "") {
          var publishMaxDate = new Date($("input[name='field_taxonomy_term_unpublish_on[und][0][value][date]']").val().substr(3,2) + "/" + $("input[name='field_taxonomy_term_unpublish_on[und][0][value][date]']").val().substr(0,2) + "/" + $("input[name='field_taxonomy_term_unpublish_on[und][0][value][date]']").val().substr(6,4));
          $("input[name='field_taxonomy_term_publish_on[und][0][value][date]']").datepicker("option", "maxDate", publishMaxDate);
        }
      });
    }
  }
})(jQuery);

function clearpub() {
  var items = jQuery('form input:text');
  for (var i in items) {
    if (typeof(items[i]) == 'object' && items[i].name && items[i].name.indexOf("field_taxonomy_term_publish_on") != -1) {
      items[i].value = '';
    }
  };
}

function clearunpub() {
  var items = jQuery('form input:text');
  for (var i in items) {
    if (typeof(items[i]) == 'object' && items[i].name && items[i].name.indexOf("field_taxonomy_term_unpublish_on") != -1) {
      items[i].value = '';
    }
  };
}
