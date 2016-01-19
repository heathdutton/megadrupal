(function ($) {
  Drupal.behaviors.blocksMassCache = {
    attach: function (context) {
      jQuery("table#blocks input:checkbox").click(function () {
        var block_name = (jQuery(this).parents("tr").find("td").attr("id"));
        var class_name = jQuery(this).attr("class");
        var cache_type = class_name.match(/rid-(.*?)\s/);

        // Get the number only.
        cache_type = cache_type[1];

        // Class.
        class_name = "rid-" + cache_type;

        switch (cache_type) {
          case "-1":
          case "-2":
          case "8":
            jQuery("table#blocks" + " ." + block_name + " input:checkbox:not(." + class_name + ")").attr('checked', false);
            jQuery("table#blocks" + " ." + block_name + " input:checkbox." + class_name).attr('checked', 'checked');
            break;

          case "1":
            jQuery("table#blocks" + " ." + block_name + " input:checkbox:not(." + class_name + ", .rid-4)").attr('checked', false);
            break;

          case "2":
            jQuery("table#blocks" + " ." + block_name + " input:checkbox:not(." + class_name + ", .rid-4)").attr('checked', false);
            break;

          case "4":
              jQuery("table#blocks" + " ." + block_name + " input:checkbox:not(." + class_name + ", .rid-1, .rid-2)").attr('checked', false);
            break;

          default:
        }
      });
    }
  };

})(jQuery);

